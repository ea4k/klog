/***************************************************************************
                          mapqmlfile.qml  -  description
                             -------------------
    begin                : May 2021
    copyright            : (C) 2021 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                                *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *                                                                           *
 *    You should have received a copy of the GNU General Public License
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.
 *                                                                           *
 *****************************************************************************/



import QtQuick
import QtQuick.Window
import QtLocation
import QtPositioning
import QtQuick.Controls


Rectangle {
    id: root
    width: 640
    height: 480
    visible: true

    property alias zoom: map.zoomLevel
    property alias lat: map.center.latitude
    property alias lon: map.center.longitude
    property double oldZoom

    signal spotDoubleClicked(string callsign, double frequencyMHz)
    signal editQSORequested(int qsoId)

    // Pixel step for pan buttons
    property int panStepPx: 100

    // For context menu lookup (hook points for C++ to set)
    property var locatorToCallsign: ({}) // e.g. { "IN80": "EA4K" }
    property var qsoLookup: null          // optional object with getCallsignByLocator(locator) function
    property string lastLocator: ""
    property string lastCallsign: ""

    // DX spot markers: tracked list and expiry (default 15 min)
    property var spotMarkers: []       // [ { item, addedAt }, ... ]
    property int spotExpiryMs: 900000  // 15 minutes; set from C++ via setSpotExpiryMinutes()

    // Periodic sweep to remove expired spot markers
    Timer {
        id: spotExpiryTimer
        interval: 60000  // check every 60 s
        repeat: true
        running: true
        onTriggered: {
            var now = Date.now()
            var remaining = []
            for (var i = 0; i < root.spotMarkers.length; i++) {
                var entry = root.spotMarkers[i]
                if (now - entry.addedAt >= root.spotExpiryMs) {
                    map.removeMapItem(entry.item)
                    entry.item.destroy()
                } else {
                    remaining.push(entry)
                }
            }
            root.spotMarkers = remaining
        }
    }

    // Zoom thresholds for label granularity
    property int labelZoom4: 7
    property int labelZoom6: 11

    // Debounce label rebuild during map drags to keep panning smooth
    Timer {
        id: labelRebuildTimer
        interval: 150
        repeat: false
        onTriggered: rebuildGridLabels()
    }

    // Dynamic model for grid labels
    ListModel { id: gridLabelModel }

    // Tooltip state for locator hover
    property bool   tooltipVisible: false
    property real   tooltipX: 0
    property real   tooltipY: 0
    property string tooltipLocator: ""

    // Model filled from C++ via locatorInfo.getQSOsForLocator()
    ListModel { id: tooltipModel }

    function updateTooltip(mouseX, mouseY) {
        if (typeof locatorInfo === "undefined" || locatorInfo === null) return

        var coord = map.toCoordinate(Qt.point(mouseX, mouseY))
        if (!coord.isValid) { tooltipVisible = false; return }

        // Compute Maidenhead locator at 6-char precision from mouse coordinate
        var loc6 = maidenhead(coord.latitude, coord.longitude, 6)

        // Check visibility at 6/4/2 char precision
        if (!locatorInfo.isLocatorVisible(loc6)) {
            tooltipVisible = false
            return
        }

        tooltipX = mouseX
        tooltipY = mouseY

        // Only refetch if locator changed (avoids re-query on every tiny mouse move)
        var qsos = locatorInfo.getQSOsForLocator(loc6)
        tooltipModel.clear()
        for (var i = 0; i < qsos.length; i++) {
            tooltipModel.append({
                qsoId:    qsos[i].id,
                callsign: qsos[i].callsign,
                band:     qsos[i].band,
                mode:     qsos[i].mode
            })
        }
        if (tooltipModel.count > 0) {
            tooltipLocator = loc6
            tooltipVisible = true
        } else {
            tooltipVisible = false
        }
    }

    // Maidenhead encoder for 2/4/6 length
    function maidenhead(lat, lon, length) {
        // Normalize
        var adjLon = lon + 180.0;
        var adjLat = lat + 90.0;

        // Fields (20° x 10°) -> letters
        var lonField = Math.floor(adjLon / 20.0);
        var latField = Math.floor(adjLat / 10.0);
        var c1 = String.fromCharCode('A'.charCodeAt(0) + lonField);
        var c2 = String.fromCharCode('A'.charCodeAt(0) + latField);
        if (length <= 2) return c1 + c2;

        // Squares (2° x 1°) -> digits
        var lonRemain = adjLon - lonField * 20.0;
        var latRemain = adjLat - latField * 10.0;
        var lonSquare = Math.floor(lonRemain / 2.0);
        var latSquare = Math.floor(latRemain / 1.0);
        var c3 = lonSquare.toString();
        var c4 = latSquare.toString();
        if (length <= 4) return c1 + c2 + c3 + c4;

        // Subsquares (2°/24 x 1°/24) -> letters (uppercase)
        lonRemain = lonRemain - lonSquare * 2.0;
        latRemain = latRemain - latSquare * 1.0;
        var lonSub = Math.floor(lonRemain / (2.0 / 24.0));  // 0..23
        var latSub = Math.floor(latRemain / (1.0 / 24.0));  // 0..23
        lonSub = Math.max(0, Math.min(23, lonSub));
        latSub = Math.max(0, Math.min(23, latSub));
        var c5 = String.fromCharCode('a'.charCodeAt(0) + lonSub);
        var c6 = String.fromCharCode('a'.charCodeAt(0) + latSub);
        return c1 + c2 + c3 + c4 + c5 + c6
    }

    function getCallsignForLocator(locator) {
        // Try a provided lookup object first (from C++)
        if (qsoLookup && typeof qsoLookup.getCallsignByLocator === "function") {
            var cs = qsoLookup.getCallsignByLocator(locator)
            if (cs && cs.length) return cs
        }
        // Fallback to dictionary map if provided
        if (locatorToCallsign && locatorToCallsign[locator]) {
            return locatorToCallsign[locator]
        }
        return "Unknown"
    }

    function rebuildGridLabels() {
        // Decide label length
        var length = 2
        if (map.zoomLevel >= labelZoom6) {
            length = 6
        } else if (map.zoomLevel >= labelZoom4) {
            length = 4
        } else {
            length = 2
        }

        // Compute visible bounds
        var tl = map.toCoordinate(Qt.point(0, 0))
        var br = map.toCoordinate(Qt.point(map.width, map.height))
        var west = Math.min(tl.longitude, br.longitude)
        var east = Math.max(tl.longitude, br.longitude)
        var north = Math.max(tl.latitude, br.latitude)
        var south = Math.min(tl.latitude, br.latitude)

        // Determine step sizes
        var lonStep, latStep
        if (length === 2) { lonStep = 20.0; latStep = 10.0; }
        else if (length === 4) { lonStep = 2.0; latStep = 1.0; }
        else { lonStep = 2.0/24.0; latStep = 1.0/24.0; }

        // Align starts to grid
        function alignDown(value, origin, step) {
            var v = value - origin
            var n = Math.floor(v / step)
            return origin + n * step
        }
        var startLon = alignDown(west, -180.0, lonStep)
        var startLat = alignDown(south, -90.0, latStep)

        gridLabelModel.clear()

        // Limit to avoid pathological counts
        var maxItems = 2000
        var count = 0

        for (var lon = startLon; lon <= east; lon += lonStep) {
            for (var lat = startLat; lat <= north; lat += latStep) {
                var cLon = lon + lonStep / 2.0
                var cLat = lat + latStep / 2.0

                // Skip if outside visible bounds due to rounding
                if (cLon < west - lonStep || cLon > east + lonStep) continue
                if (cLat < south - latStep || cLat > north + latStep) continue

                var text = maidenhead(cLat, cLon, length)
                gridLabelModel.append({
                    latitude: cLat,
                    longitude: cLon,
                    text: text
                })

                count++
                if (count > maxItems) break
            }
            if (count > maxItems) break
        }
    }

    //Location { id: mapCenter }

    function addMarker(latitude, longitude, callsign, color, frequencyMHz) {
        var component = Qt.createComponent("qrc:///qml/marker.qml")
        if (component.status !== Component.Ready) {
            console.warn("addMarker: failed to load marker.qml:", component.errorString())
            return
        }
        var item = component.createObject(map, {
            coordinate:  QtPositioning.coordinate(latitude, longitude),
            text:        callsign      || "",
            markerColor: color         || "#FF0000",
            frequency:   frequencyMHz  || 0.0,
            z:           5
        })
        if (item === null) {
            console.warn("addMarker: createObject returned null")
            return
        }
        item.markerDoubleClicked.connect(function(cs, freq) {
            root.spotDoubleClicked(cs, freq)
        })
        map.addMapItem(item)
        root.spotMarkers = root.spotMarkers.concat([{ item: item, addedAt: Date.now() }])
    }

    function clearMarkers() {
        for (var i = 0; i < root.spotMarkers.length; i++) {
            map.removeMapItem(root.spotMarkers[i].item)
            root.spotMarkers[i].item.destroy()
        }
        root.spotMarkers = []
    }

    FocusScope { anchors.fill: parent }

    // Shown when the Qt6 Location geo-services plugin (OSM) is not available.
    // This prevents the "Unable to assign [undefined] to QGeoMapType" error
    // and gives the user an actionable message instead of a blank/broken map.
    Rectangle {
        id: mapUnavailableOverlay
        anchors.fill: parent
        // Visible once the map component has initialised and reports no map types
        visible: false
        color: "#1e1e2e"
        z: 200

        Column {
            anchors.centerIn: parent
            spacing: 10

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Map not available")
                color: "white"
                font.bold: true
                font.pixelSize: 18
            }
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("The Qt6 Location geo-services plugin is missing.\n" +
                            "Install the OSM plugin for your platform, e.g.:\n" +
                            "  Debian/Ubuntu/Raspberry Pi OS: apt install qml6-module-qtlocation\n" +
                            "  or: apt install qt6-declarative-dev")
                color: "#aaaaaa"
                font.pixelSize: 12
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }

    Plugin {
        id: mapPlugin
          name: "osm"
          PluginParameter {
              name: "osm.mapping.custom.host"
              value: "https://tile.openstreetmap.org/"
          }
          PluginParameter {
              name: "osm.mapping.providersrepository.disabled"
              value: "true"
          }
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(0, 0)
        zoomLevel: 4
        // Note: activeMapType is set in Component.onCompleted to avoid
        // "Unable to assign [undefined] to QGeoMapType" when the OSM plugin
        // is not installed (e.g. Raspberry Pi without qt6-qtlocation plugins).

        // Helper: pan by screen pixels (dx, dy). Positive dx -> right, positive dy -> down.
        function panByPixels(dx, dy) {
            var centerPx = Qt.point(width / 2, height / 2)
            var targetPx = Qt.point(centerPx.x + dx, centerPx.y + dy)
            var targetCoord = map.toCoordinate(targetPx)
            map.center = targetCoord
        }

        // Data overlays (worked/confirmed) - semi-transparent fills (from C++)
        MapItemView {
            z: 0
            model: rectangle_model
            delegate: MapRectangle {
                topLeft     : model.north
                bottomRight : model.south
                color       : model.color
                border.width: 2
                border.color: "#000000"
            }
        }

        // Base GRID (2-letter fields) - persistent, thin borders, transparent fill (from C++)
        MapItemView {
            z: 1
            model: grid_model
            delegate: MapRectangle {
                topLeft     : model.north
                bottomRight : model.south
                color       : "transparent"
                border.width: 1
                border.color: "#808080"
            }
        }

        // Optional circle markers
        MapItemView {
            z: 2
            model: circle_model
            delegate: MapCircle {
                center: model.coordinate
                radius: 5000.0
                color: "green"
                border.width: 10
            }
        }

        // Dynamic GRID labels; 2/4/6 chars depending on zoom level
        MapItemView {
            z: 3
            model: gridLabelModel
            delegate: MapQuickItem {
                coordinate: QtPositioning.coordinate(model.latitude, model.longitude)
                sourceItem: Rectangle {
                    color: "transparent"
                    border.width: 0
                    Text {
                        text: model.text
                        color: "white"
                        font.bold: true
                        style: Text.Outline
                        styleColor: "black"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                anchorPoint.x: width / 2
                anchorPoint.y: height / 2
            }
        }

        // Rebuild labels when view changes (debounced) + print zoom level
        onZoomLevelChanged: function() {
            //console.log("Map zoom level:", map.zoomLevel)
            labelRebuildTimer.restart()
        }
        onCenterChanged: function() {
            labelRebuildTimer.restart()
        }
        Component.onCompleted: {
            // Guard against empty supportedMapTypes (missing OSM plugin).
            // Shows a user-friendly overlay instead of a QML binding error.
            if (supportedMapTypes.length > 0) {
                activeMapType = supportedMapTypes[supportedMapTypes.length - 1]
                mapUnavailableOverlay.visible = false
            } else {
                mapUnavailableOverlay.visible = true
            }
            rebuildGridLabels()
        }

        // =========================
        // Zoom controls (top-right)
        // =========================
        Item {
            id: zoomControls
            z: 100
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.topMargin: 8
            anchors.rightMargin: 8
            width: 30
            height: (30 * 2) + 6

            Column {
                anchors.fill: parent
                spacing: 6

                // Zoom in (+)
                Rectangle {
                    width: parent.width; height: 30
                    radius: 5
                    border.color: "red"
                    Text { text: "+"; color: "black"; anchors.centerIn: parent }
                    MouseArea { anchors.fill: parent; onClicked: { oldZoom = zoom; zoom = oldZoom + 1 } }
                }
                // Zoom out (-)
                Rectangle {
                    width: parent.width; height: 30
                    radius: 5
                    border.color: "red"
                    Text { text: "-"; color: "black"; anchors.centerIn: parent }
                    MouseArea { anchors.fill: parent; onClicked: { oldZoom = zoom; zoom = oldZoom - 1 } }
                }
            }
        }

        // ====================================================
        // Invisible hover area for locator tooltip detection
        // ====================================================
        MouseArea {
            id: locatorHoverArea
            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.NoButton  // pass all clicks through to the map
            propagateComposedEvents: true
            z: 50

            onPositionChanged: function(mouse) {
                root.updateTooltip(mouse.x, mouse.y)
            }
            onExited: {
                root.tooltipVisible = false
            }
        }

        // ================================
        // Clear spots button (top-left)
        // ================================
        Rectangle {
            id: clearSpotsButton
            z: 100
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 8
            anchors.leftMargin: 8
            width: 60
            height: 26
            radius: 5
            color: clearArea.containsMouse ? "#cc2222" : "#882222"
            border.color: "#ff4444"
            visible: root.spotMarkers.length > 0

            Text {
                anchors.centerIn: parent
                text: qsTr("Clear")
                color: "white"
                font.bold: true
                font.pixelSize: 12
            }

            MouseArea {
                id: clearArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: root.clearMarkers()
            }
        }

        // ==================================
        // Pan controls (bottom-right, cross)
        // Arrows; N/S close together; W at left edge, E at right edge
        // ==================================
        Item {
            id: panControls
            z: 100
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 8
            anchors.bottomMargin: 8

            // Cross layout parameters
            property int btn: 30
            property int gap: 4
            // Container sized to fit W + NS + E with gaps, and N+S stacked with a gap
            width: (btn * 3) + (gap * 2)
            height: (btn * 2) + gap

            // North/South stacked group in the middle (close together)
            Column {
                id: nsGroup
                spacing: panControls.gap
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                // North (top)
                Rectangle {
                    id: panNorth
                    width: panControls.btn; height: panControls.btn
                    radius: 5; border.color: "red"
                    Text { text: "↑"; color: "black"; anchors.centerIn: parent }
                    MouseArea { anchors.fill: parent; onClicked: { map.panByPixels(0, -root.panStepPx) } }
                }
                // South (bottom)
                Rectangle {
                    id: panSouth
                    width: panControls.btn; height: panControls.btn
                    radius: 5; border.color: "red"
                    Text { text: "↓"; color: "black"; anchors.centerIn: parent }
                    MouseArea { anchors.fill: parent; onClicked: { map.panByPixels(0, root.panStepPx) } }
                }
            }

            // West: at the left limit of the NS group
            Rectangle {
                id: panWest
                width: panControls.btn; height: panControls.btn
                radius: 5; border.color: "red"
                anchors.verticalCenter: nsGroup.verticalCenter
                anchors.right: nsGroup.left
                anchors.rightMargin: panControls.gap
                Text { text: "←"; color: "black"; anchors.centerIn: parent }
                MouseArea { anchors.fill: parent; onClicked: { map.panByPixels(-root.panStepPx, 0) } }
            }

            // East: at the right limit of the NS group
            Rectangle {
                id: panEast
                width: panControls.btn; height: panControls.btn
                radius: 5; border.color: "red"
                anchors.verticalCenter: nsGroup.verticalCenter
                anchors.left: nsGroup.right
                anchors.leftMargin: panControls.gap
                Text { text: "→"; color: "black"; anchors.centerIn: parent }
                MouseArea { anchors.fill: parent; onClicked: { map.panByPixels(root.panStepPx, 0) } }
            }
        }
    }

    // ============================================================
    // Locator hover tooltip: shown when mouse is over a worked/
    // confirmed grid square. Double-click a row to edit the QSO.
    // ============================================================
    Rectangle {
        id: locatorTooltip
        visible: root.tooltipVisible && tooltipModel.count > 0
        z: 300

        // Position near the cursor, clamped to stay within the window
        x: Math.min(root.tooltipX + 14, root.width  - width  - 4)
        y: Math.min(root.tooltipY + 14, root.height - height - 4)

        width: 280
        // locator header(22) + col-headers(20) + QSO rows(22 each, max 10) + hint(16) + top+bottom margins(10)
        height: 22 + 20 + Math.min(tooltipModel.count, 10) * 22 + 16 + 10

        color: "#e8f0f8"
        border.color: "#334466"
        border.width: 1
        radius: 5

        // Swallow mouse events so the tooltip doesn't accidentally hide itself
        MouseArea { anchors.fill: parent; acceptedButtons: Qt.NoButton; hoverEnabled: true }

        Column {
            anchors {
                top: parent.top; left: parent.left; right: parent.right
                margins: 5
            }
            spacing: 0

            // Header showing the locator
            Rectangle {
                id: headerRow
                width: parent.width
                height: 22
                color: "#334466"
                radius: 3

                Text {
                    anchors.centerIn: parent
                    text: root.tooltipLocator
                    color: "white"
                    font.bold: true
                    font.pixelSize: 12
                }
            }

            // Column header row
            Row {
                width: parent.width
                height: 20
                spacing: 0

                Rectangle { width: parent.width * 0.5; height: parent.height; color: "#c0cce0"
                    Text { anchors.centerIn: parent; text: qsTr("Callsign"); font.bold: true; font.pixelSize: 11; color: "#222" }
                }
                Rectangle { width: parent.width * 0.25; height: parent.height; color: "#c0cce0"
                    Text { anchors.centerIn: parent; text: qsTr("Band"); font.bold: true; font.pixelSize: 11; color: "#222" }
                }
                Rectangle { width: parent.width * 0.25; height: parent.height; color: "#c0cce0"
                    Text { anchors.centerIn: parent; text: qsTr("Mode"); font.bold: true; font.pixelSize: 11; color: "#222" }
                }
            }

            // QSO rows
            ListView {
                id: tooltipListView
                width: parent.width
                height: Math.min(tooltipModel.count, 10) * 22
                model: tooltipModel
                clip: true
                interactive: tooltipModel.count > 10

                delegate: Rectangle {
                    width: tooltipListView.width
                    height: 22
                    color: index % 2 === 0 ? "#f0f4fa" : "#dce8f0"

                    Row {
                        anchors.fill: parent
                        spacing: 0

                        Text {
                            width: parent.width * 0.5
                            height: parent.height
                            text: model.callsign
                            font.pixelSize: 11
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: 4
                            elide: Text.ElideRight
                        }
                        Text {
                            width: parent.width * 0.25
                            height: parent.height
                            text: model.band
                            font.pixelSize: 11
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            elide: Text.ElideRight
                        }
                        Text {
                            width: parent.width * 0.25
                            height: parent.height
                            text: model.mode
                            font.pixelSize: 11
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            elide: Text.ElideRight
                        }
                    }

                    // Highlight on hover
                    Rectangle {
                        anchors.fill: parent
                        color: rowHover.containsMouse ? "#aabbdd" : "transparent"
                        MouseArea {
                            id: rowHover
                            anchors.fill: parent
                            hoverEnabled: true
                            onDoubleClicked: {
                                root.editQSORequested(model.qsoId)
                            }
                        }
                    }
                }
            }

            // Hint text for double-click
            Text {
                width: parent.width
                text: qsTr("Double-click to edit")
                font.pixelSize: 9
                color: "#667"
                horizontalAlignment: Text.AlignRight
                rightPadding: 4
                topPadding: 2
            }
        }
    }
}

