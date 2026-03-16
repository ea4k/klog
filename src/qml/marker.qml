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

MapQuickItem {
    id: marker

    property alias text: locatorText.text
    property color markerColor: "#FF0000"

    anchorPoint.x: pinHead.width / 2
    anchorPoint.y: pinHead.height / 2

    sourceItem: Column {
        id: pinColumn
        spacing: 2

        // Colored circle — the pin head, anchored to the coordinate
        Rectangle {
            id: pinHead
            width: 16
            height: 16
            radius: 8
            color: marker.markerColor
            border.color: Qt.darker(marker.markerColor, 1.6)
            border.width: 2
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // Callsign label below the pin
        Text {
            id: locatorText
            font.pixelSize: 10
            font.bold: true
            color: "white"
            style: Text.Outline
            styleColor: "black"
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
