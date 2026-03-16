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

MapQuickItem{
    id: marker
    anchorPoint.x: marker.width / 2
    anchorPoint.y: marker.height
    property alias text: locatorText.text
    sourceItem: Rectangle{
        id: rectaMap
        width: icon.sourceSize.width
        height: icon.sourceSize.height + locatorText.height
        color: "transparent"
        Image{
            id: icon
            source: "qrc:/img/marker.png"
            sourceSize.width: 40
            sourceSize.height: 40
        }
        Text{
            id: locatorText
            color: "black"
            anchors.top: icon.bottom
            anchors.horizontalCenter: icon.horizontalCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
