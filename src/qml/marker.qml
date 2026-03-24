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
            color: marker.markerColor
            border.color: Qt.darker(marker.markerColor, 1.6)
            border.width: 2
        }

        // Callsign label: shown for 5s on arrival, or while hovering
        Rectangle {
            id: callsignBubble
            visible: marker.text.length > 0 && (arrivalTimer.running || hoverArea.containsMouse)
            color: Qt.rgba(0, 0, 0, 0.72)
            radius: 3
            x: pinHead.width + 5
            y: (pinHead.height - height) / 2
            width: callsignText.width + 8
            height: callsignText.height + 6

            Text {
                id: callsignText
                anchors.centerIn: parent
                text: marker.text
                color: "white"
                font.pixelSize: 11
            }
        }

        // Auto-hide timer: starts when the marker is created, runs 5 seconds
        Timer {
            id: arrivalTimer
            interval: 5000
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
