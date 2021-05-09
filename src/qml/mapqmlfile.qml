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
    width: 512
    height: 512
    visible: true
    property alias zoom: map.zoomLevel
    property alias lat: map.center.latitude
    property alias lon: map.center.longitude
    //property alias locLat1: locR.topLeft.latitude
    //property alias locLon1: locR.topLeft.longitude
    //property alias locLat2: locR.bottomRight.latitude
    //property alias locLon2: locR.bottomRight.longitude

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
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center {
                // One great place (Naranjo de Bulnes)
                latitude: 43.2
                longitude: -4.816669
            }
        zoomLevel: 14
    }
    MapRectangle
    function addLoc(lat,longi) {

            var locator = Qt.createQmlObject('import QtLocation 5.3; MapRectangle {   }', map, "dynamic");
            if(grid=== null) {
               console.log("error creating object" +  grid.errorString());
               return false;
            }
            grid.center = QtPositioning.coordinate(lat, longi);
            grid.radius = 50000.0;

            grid.border.width = 1;
            map.addMapItem(grid);
            map.center = QtPositioning.coordinate(lat, longi);
            console.log("success creating object");
            return true;
        }

    function addPoi(lat,longi) {

            var circle = Qt.createQmlObject('import QtLocation 5.3; MapCircle {   }', map, "dynamic");
            if(circle === null) {
               console.log("error creating object" +  circle.errorString());
               return false;
            }
            circle.center = QtPositioning.coordinate(lat, longi);
            circle.radius = 50000.0;

            circle.border.width = 1;
            map.addMapItem(circle);
            map.center = QtPositioning.coordinate(lat, longi);
            console.log("success creating object");
            return true;
        }
}
