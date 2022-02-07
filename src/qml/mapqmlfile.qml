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
    //property alias locator: map.loca
    /*topLeft {
    latitude: -27
    longitude: 153
}*/
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

        //onCenterChanged:
        //{
        //    console.log("Map Center X: ", lat, " - Map Center Y: ", lon);
        //}
        zoomLevel: 14
        MouseArea
        {
            hoverEnabled: true
            anchors.fill: parent

            //onPositionChanged:
            //{
            //    Qt.point(mouseX, mouseY)
            //    var coordinate = map.toCoordinate(Qt.point(mouse.x,mouse.y))
            //    console.log("Mouse Position (", mouseX, ", ", mouseY, ")");
            //    console.log("Mouse GeoPosition (", coordinate.latitude, ", ", coordinate.longitude, ")");
            //}
        }
/*
        MapItemView
        {
            model: rectangle_model
            delegate: MapRectangle
            {
                topLeft     : model.coordinate
                bottomRight : model.coordinate
                color: 'green'
                border.width: 10
            }
        }

*/
        /*
    //North
    cornerManual.lat = 41.0;
    cornerManual.lon = -4.0;
    //South
    cornerManual.lat = 40.0;
    cornerManual.lon = -2.0;
*/
    MapItemView{
    model: rectangle_model
    delegate: MapRectangle
    {
      //color: 'green'
      border.width: 2
      topLeft       : model.north
      bottomRight   : model.south
      color         : model.color
    }
}
/*
    MapItemView{
                  model: rectangle_model
                  delegate: MapRectangle
                  {
                    color: 'green'
                    border.width: 2
                    topLeft {
                        latitude: 41.0
                        longitude: -4.0
                    }
                    bottomRight
                    {
                        latitude: 40.0
                        longitude: -2.0
                    }
                  }
    }
    */


        MapItemView
        {
              model: circle_model
              delegate: MapCircle{
                    center: model.coordinate
                    radius: 5000.0
                    color: 'green'
                    border.width: 10
              }
          }
    }
}
