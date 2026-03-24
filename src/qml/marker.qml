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
import QtLocation
import QtQuick.Controls

MapQuickItem {
    id: marker

    property string text: ""
    property color markerColor: "#FF0000"
    property double frequency: 0.0   // MHz

    signal markerDoubleClicked(string callsign, double frequencyMHz)

    anchorPoint.x: pinHead.width / 2
    anchorPoint.y: pinHead.height / 2

    sourceItem: Item {
        id: pinItem
        width: pinHead.width
        height: pinHead.height

        // Colored circle — the pin head, anchored to the coordinate
        Rectangle {
            id: pinHead
            width: 16
            height: 16
            radius: 8
            color: Qt.rgba(marker.markerColor.r, marker.markerColor.g, marker.markerColor.b,
                           Math.min(1.0, marker.markerColor.a + 0.15))
            border.color: Qt.darker(marker.markerColor, 1.6)
            border.width: 2
        }

        // Callsign label: shown for 10s on arrival, or while hovering
        Text {
            id: callsignText
            visible: marker.text.length > 0 && (arrivalTimer.running || hoverArea.containsMouse)
            text: marker.text
            color: "black"
            style: Text.Outline
            styleColor: "white"
            font.pixelSize: 12
            font.bold: true
            x: pinHead.width + 5
            y: (pinHead.height - height) / 2
        }

        // Auto-hide timer: starts when the marker is created, runs 10 seconds
        Timer {
            id: arrivalTimer
            interval: 10000
            running: true
            repeat: false
        }

        // Hover + double-click area
        MouseArea {
            id: hoverArea
            anchors.fill: parent
            hoverEnabled: true
            onDoubleClicked: marker.markerDoubleClicked(marker.text, marker.frequency)
        }
    }
}
