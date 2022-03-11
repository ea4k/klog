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
import QtQuick 2.0
import QtLocation 5.12

MapQuickItem{
    id: marker
    anchorPoint.x: marker.width / 4
    anchorPoint.y: marker.height
    property alias text: locatorText.text
    //sourceItem: Image{
    //    id: icon
    //    source: "../img/marker.png"
    //    sourceSize.width: 40
    //    sourceSize.height: 40
    //}
    sourceItem: Rectangle{
    id: rectaMap
    Image{
            id: icon
            //source: "../img/marker.png"
            source: "qrc:/img/marker.png"
            sourceSize.width: 40
            sourceSize.height: 40
        }
    Text{
        id: locatorText
        //text: 'IN80'
        //width: rectaMap.width
        color: focus?"red":"black"
        anchors.top: icon.bottom
        anchors.horizontalCenter: icon.horizontalCenter
        horizontalAlignment: icon.AlignHCenter
    }
    }
}
