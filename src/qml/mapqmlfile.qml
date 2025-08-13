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
 *    KLog is free software: you can redistribute it and/or modify           *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,                *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/
import QtQuick
import QtQuick.Window
import QtLocation
import QtPositioning
import QtQuick.Controls

Rectangle {
    width: 640
    height: 480
    visible: true
    property alias zoom: map.zoomLevel
    property alias lat: map.center.latitude
    property alias lon: map.center.longitude
    property double oldZoom

    // Threshold for switching between short (4-char) and long (6-char) locator labels
    property int labelZoomThreshold: 9

    Location { id: mapCenter }

    function addMarker(latitude, longitude) {
        var Component = Qt.createComponent("qrc:qml/marker.qml")
        var item = Component.createObject(Rectangle, {
            coordinate: QtPositioning.coordinate(latitude, longitude)
        })
        map.addMapItem(item)
    }

    FocusScope { anchors.fill: parent }

    Plugin {
        id: mapPlugin
        name: "osm" // default provider
        PluginParameter {
            name: "osm.mapping.custom.host"
            value: "https://tile.openstreetmap.org/"
        }
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: mapCenter.coordinate
        zoomLevel: 14
        activeMapType: supportedMapTypes[supportedMapTypes.length - 1]

        MouseArea { hoverEnabled: true; anchors.fill: parent }
        MouseArea { hoverEnabled: true; anchors.fill: parent }

        // Zoom out button
        Rectangle {
            id: buttonout
            width: 30; height: 30
            border.color: "red"; radius: 5
            anchors.right: parent.right; anchors.bottom: parent.bottom
            Text { text: "-"; color: "black"; anchors.centerIn: parent }
            MouseArea { anchors.fill: parent; onClicked: { oldZoom = zoom; zoom = oldZoom - 1 } }
        }
        // Zoom in button
        Rectangle {
            id: buttonin
            width: 30; height: 30
            border.color: "red"; radius: 5
            anchors.bottom: buttonout.top; anchors.right: buttonout.right
            Text { text: "+"; color: "black"; anchors.centerIn: parent }
            MouseArea { anchors.fill: parent; onClicked: { oldZoom = zoom; zoom = oldZoom + 1 } }
        }

        // Painted Maidenhead rectangles
        MapItemView {
            model: rectangle_model
            delegate: MapRectangle {
                border.width: 2
                topLeft     : model.north
                bottomRight : model.south
                color       : model.color
            }
        }

        // Optional circle markers (existing)
        MapItemView {
            model: circle_model
            delegate: MapCircle {
                center: model.coordinate
                radius: 5000.0
                color: "green"
                border.width: 10
            }
        }

        // Labels centered on each rectangle; text switches with zoom
        MapItemView {
            model: label_model
            delegate: MapQuickItem {
                id: labelItem
                coordinate: model.center
                // Center the text at the coordinate
                sourceItem: Rectangle {
                    id: labelRect
                    color: "transparent"
                    border.width: 0
                    Text {
                        id: labelText
                        text: map.zoomLevel >= labelZoomThreshold ? model.longtext : model.shorttext
                        color: model.textcolor
                        font.bold: true
                        // Add an outline for readability
                        style: Text.Outline
                        styleColor: "black"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                // Center the text over the geo coordinate
                anchorPoint.x: labelRect.width  / 2
                anchorPoint.y: labelRect.height / 2
            }
        }
    }
}

