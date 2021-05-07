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
import QtQuick.Window 2.12
import QtLocation 5.6
import QtPositioning 5.6

Rectangle {
    width: Qt.platform.os == "android" ? Screen.width : 512
    height: Qt.platform.os == "android" ? Screen.height : 512
    visible: true
    property alias zoom: mapRect.zoomLevel
    property alias lat: mapRect.center.latitude
    property alias lon: mapRect.center.longitude


    Plugin {
        id: mapPlugin
        name: "osm" // "mapboxgl", "esri", ...
        // specify plugin parameters if necessary
        // PluginParameter {
        //     name:
        //     value:
        // }
    }

    Map {
        id: mapRect
        anchors.fill: parent
        plugin: mapPlugin
        center {
                // One great place (Naranjo de Bulnes)
                latitude: 43.2
                longitude: -4.816669
            }
        zoomLevel: 14
    }
}
