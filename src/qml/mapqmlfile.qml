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
    width: 640
    height: 480
    visible: true
    property alias zoom: map.zoomLevel
    property alias lat: map.center.latitude
    property alias lon: map.center.longitude
    //property alias locLat1: locR.topLeft.latitude
    //property alias locLon1: locR.topLeft.longitude
    //property alias locLat2: locR.bottomRight.latitude
    //property alias locLon2: locR.bottomRight.longitude



    Location {
            // Define location that will be "center" of map
            id: mapCenter
            coordinate {
                latitude: 43.2
                longitude: -4.816669
            }
    }

    FocusScope
    {
         anchors.fill: parent
    }
    Plugin {
        id: mapPlugin
        name: "osm" // "osm", "mapboxgl", "esri", "googleMap...
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: mapCenter.coordinate

        onCenterChanged:
        {
            console.log("Map Center X: ", lat, " - Map Center Y: ", lon);
        }
        zoomLevel: 14
        MouseArea
        {
            hoverEnabled: true
            anchors.fill: parent

            onPositionChanged:
            {
                Qt.point(mouseX, mouseY)
                var coordinate = map.toCoordinate(Qt.point(mouse.x,mouse.y))
                console.log("Mouse Position (", mouseX, ", ", mouseY, ")");
                console.log("Mouse GeoPosition (", coordinate.latitude, ", ", coordinate.longitude, ")");
            }
        }
    }



    Canvas {
             id: root
             anchors.fill : parent

             property double wgrid: 20.0
             property double hgrid: 10.0

             onPaint: {

                 console.log("Latitude: ", lat, " - Longitude: ", lon);
                 var ctx = getContext("2d")
                 ctx.lineWidth = 1
                 ctx.strokeStyle = "black"
                 ctx.beginPath()

                 ctx.moveTo(0, 0);
                 ctx.lineTo(width, hgrid);
             }
    }

    /*
   Canvas {
            id: root
            anchors.fill : parent

            property double wgrid: 20.0
            property double hgrid: 10.0
            onPaint: {
                var ctx = getContext("2d")
                ctx.lineWidth = 1
                ctx.strokeStyle = "black"
                ctx.beginPath()
                var nrows = 360/10; // 36
                var ncols = 360/20; // 18

                //console.log("Zoom is: ", zoom)

                hgrid = height/nrows
                wgrid = width/ncols

                for(var i=0; i < nrows+1; i++){

                    ctx.moveTo(0, hgrid*i);
                    ctx.lineTo(width, hgrid*i);
                }


                for(var j=0; j < ncols+1; j++){
                    ctx.moveTo(wgrid*j, 0);
                    ctx.lineTo(wgrid*j, height);
                }
                ctx.closePath()
                ctx.stroke()
            }
        }


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
    */
}
