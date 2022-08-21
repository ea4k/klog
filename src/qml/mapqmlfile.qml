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
    //property alias mapLocale: map.plugin.locales

    Location {
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
        name: "osm" // Names in local language
        //name: "esri" // Names not in local language but english
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
        zoomLevel: 14
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
    }

}
