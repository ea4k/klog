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
import QtQuick 2.12
import QtQuick.Window 2.12
import QtLocation 5.6
import QtPositioning 5.6
import QtQuick.Controls 2.15

Rectangle {
    width: 640
    height: 480
    visible: true
    property alias zoom: map.zoomLevel
    property alias lat: map.center.latitude
    property alias lon: map.center.longitude
    property double oldZoom
    property double locatorTemplateW
    property double locatorTemplateH
    property int wgrid
/*
    if (aux.length() == 2)
    {
        return (((aux.at(1)).toLatin1() - 'A') * 10) - 85;
    }
*/
    //function getLat(b) {
    //        a = qth.charCodeAt(0) - 65;
    //        return a*20
    //}

    //property alias mapLocale: map.plugin.locales

    Location
    {
            // Define location that will be "center" of map
            id: mapCenter
    }
    //function addMarker(latitude: double, longitude: double, locatorText: String)
    function addMarker(latitude: double, longitude: double)
        {
            var Component = Qt.createComponent("qrc:qml/marker.qml")
            //var item = Component.createObject(Rectangle, {
            //                                      coordinate: QtPositioning.coordinate(latitude, longitude), text: locatorText
            //                                  })
        var item = Component.createObject(Rectangle, {
                                              coordinate: QtPositioning.coordinate(latitude, longitude)
                                          })
        //if (zoom>5)
        //{
        //    console.log("Zoom>5: ", zoom);
        //}
            map.addMapItem(item)
        }

    FocusScope
    {
         anchors.fill: parent
    }

    Plugin {
        id: mapPlugin
        //name: "osm" // "osm", "mapboxgl", "esri", "googleMap...
        name: "esri"
        //name: "googleMap"
        //name: "mapboxgl"
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: mapCenter.coordinate
        Plugin {
           //locales: "en_US"
        }
        //Component.onCompleted:addMarker(40.18, -3.649, "OOO")
        //onCenterChanged:
        //{
        //    console.log("Map Center X: ", lat, " - Map Center Y: ", lon);
        //}
        zoomLevel: 5
        MouseArea
        {
            hoverEnabled: true
            anchors.fill: parent
            //acceptedButtons: Qt.LeftButton
            //onClicked:
            //{
            //             console.log("left button clicked!")
            //}
            //onPositionChanged:
            //{
            //    Qt.point(mouseX, mouseY)
            //    var coordinate = map.toCoordinate(Qt.point(mouse.x,mouse.y))
            //    console.log("Mouse Position (", mouseX, ", ", mouseY, ")");
            //    console.log("Mouse GeoPosition (", coordinate.latitude, ", ", coordinate.longitude, ")");
            //}
        }
        MouseArea
        {
            hoverEnabled: true
            anchors.fill: parent
            //acceptedButtons: Qt.RightButton
            //onClicked:
            //{
            //     console.log("right button clicked!")
            //    contextMenu.popup()
            //}
            //Menu {
            //    id: contextMenu
            //    MenuItem {text: "Show QSOs"}
            //}
        }

        Rectangle {
            id: buttonout
            width: 30
            height: 30
            border.color: "red"
            radius: 5     // Let's round the rectangle's corner a bit, so it resembles more a button
            //anchors.centerIn: parent
            anchors.right: parent.right; anchors.bottom: parent.bottom

            Text {
                id: buttonText
                text: "-"
                color: "black"
                anchors.centerIn: parent
            }

            MouseArea {
                // We make the MouseArea as big as its parent, i.e. the rectangle. So pressing anywhere on the button will trigger the event
                anchors.fill: parent

                // Exploit the built-in "clicked" signal of the MouseArea component to do something when the MouseArea is clicked.
                // Note that the code associated to the signal is plain JavaScript. We can reference any QML objects by using their IDs
                onClicked: {
                    oldZoom = zoom
                    zoom = oldZoom - 1
                    canvasGrid.requestPaint()
                }
            }
        }
        Rectangle {
            id: buttonin
            width: 30
            height: 30
            border.color: "red"
            radius: 5     // Let's round the rectangle's corner a bit, so it resembles more a button
            anchors.bottom: buttonout.top; anchors.right: buttonout.right
            //anchors.right: parent.right; anchors.bottom: parent.bottom

            Text {
                id: buttonTextout
                text: "+"
                color: "black"
                anchors.centerIn: parent
            }

            MouseArea {
                // We make the MouseArea as big as its parent, i.e. the rectangle. So pressing anywhere on the button will trigger the event
                anchors.fill: parent

                // Exploit the built-in "clicked" signal of the MouseArea component to do something when the MouseArea is clicked.
                // Note that the code associated to the signal is plain JavaScript. We can reference any QML objects by using their IDs
                onClicked: {
                    oldZoom = zoom
                    zoom = oldZoom + 1
                    canvasGrid.requestPaint()
                    //buttonText.text = qsTr("Clicked");
                    //buttonText.color = "black";
                }
            }
        }

        MapItemView
        {
            model: rectangle_model
            delegate: MapRectangle
            {
                border.width: 2
                topLeft       : model.north
                bottomRight   : model.south
                color         : model.color
                //opacity       : 0.5
            }

            MouseArea {
                hoverEnabled: true
                anchors.fill: parent
                //acceptedButtons: Qt.LeftButton
                //onClicked:
                //{
                //             console.log("left button clicked!")
                //}
                /*
                onPositionChanged:
                {
                    Qt.point(mouseX, mouseY)
                    var coordinate = map.toCoordinate(Qt.point(mouse.x,mouse.y))
                    console.log("Mouse Position (", mouseX, ", ", mouseY, ")");
                    console.log("Mouse GeoPosition (", coordinate.latitude, ", ", coordinate.longitude, ")");
                    var posicion = map.fromCoordinate(coordinate, false);
                    console.log("Pos (", posicion.x, ", ", posicion.y, ")");
                }
                */
            }
        }
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
       Canvas {
            id: canvasGrid
            anchors.fill : parent
            function clear_canvas() {
                        var ctx = getContext("2d");
                        ctx.reset();
                        requestPaint();
                    }
            onPaint: {

                clear_canvas()
                if (map.zoomLevel > 7)
                {
                    //wgrid = 80
                    console.log("Zoom>7: ", zoom);
                    console.log("wgrid: ", wgrid);
                }
                else
                {
                    //wgrid = 10
                    console.log("Zoom<7: ", zoom);
                    console.log("wgrid: ", wgrid);
                }

                var ctx = getContext("2d")
                ctx.lineWidth = 1
                ctx.strokeStyle = "black"
                ctx.beginPath()
                /*
                        var posicion = map.fromCoordinate(coordinate, false);
                        console.log("Pos (", posicion.x, ", ", posicion.y, ")");
                */
                //https://dxcluster.ha8tks.hu/hamgeocoding/L.Maidenhead.js
                //wgrid = 18
                //var nrows = height/wgrid;
                console.log("Height: ", height);
                console.log("Width: ", width);
                //console.log("Rows: ", nrows);
                var w = map.toCoordinate(Qt.point(0,0));
                var e = map.toCoordinate(Qt.point(width,height));
                console.log("West: ", w.latitude, ",", w.longitude);
                console.log("East: ", e.latitude, ",", e.longitude);

                //coordinate: QtPositioning.coordinate(-27.5, 153.1)
                var nrows = 18
                var lati = w.longitude
                var posici
                //center: QtPositioning.coordinate(19.997454, 73.789803)
                var controlPoint = w
                //var mouseGeoPos = map.toCoordinate(Qt.point(mouse.x, mouse.y));
                //var preZoomPoint = map.fromCoordinate(mouseGeoPos, false);
                for(var i=0; i < 10; i++){
                    //QtPositioning.coordinate((360/filas)*i, 0)
                    // -180 -> 180
                    controlPoint = controlPoint + QtPositioning.coordinate(w.latitude+(10.0*i), w.longitude)
                    //posici = map.fromCoordinate(controlPoint, true);

                    lati = posici.y
                    console.log("Y: ", lati);
                    ctx.moveTo(0, lati);
                    ctx.lineTo(width, lati);
                }
                //for(var i=0; i < nrows+1; i++){
                //    ctx.moveTo(0, wgrid*i);
                //    ctx.lineTo(width, wgrid*i);
                //}

                //var ncols = width/wgrid
                //console.log("Painting ncols: ", ncols);
                //for(var j=0; j < ncols+1; j++){
                //    ctx.moveTo(wgrid*j, 0);
                //    ctx.lineTo(wgrid*j, height);
                //}
                ctx.closePath()
                ctx.stroke()
            }
        }

        /*
        Text { // text changes when button was clicked
                id: status
                //x: 12; y: 76
                //width: 116; height: 26
                text: "waiting ..."
                //horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                //verticalAlignment: Text.AlignVCenter
                //anchors.centerIn: parent
            }
        */
    }




}
