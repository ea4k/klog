/***************************************************************************
                           mapwidget.cpp  -  description
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
#include "mapwidget.h"
#include <QQuickView>
#include <QGeoCoordinate>
#include <QGeoRectangle>
#include <QQmlContext>
#include <QStandardItemModel>

MapWidget::MapWidget(QWidget *parent)
{

    qDebug() << Q_FUNC_INFO;

    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWidget::init()
{
    createUI();

}

void MapWidget::createUI()
{
    //lat = 0.0;
    //lon = 0.0;

    QWidget *container = QWidget::createWindowContainer(&qmlView, this);

    circleRoles[CoordinateRole] = QByteArray("coordinate");
    rectangleRoles[NorthRole] = QByteArray("north");
    rectangleRoles[SouthRole] = QByteArray("south");
    rectangleRoles[ColorRole] = QByteArray("color");

    modelCircle.setItemRoleNames(circleRoles);
    modelRectangle.setItemRoleNames(rectangleRoles);

    qmlView.rootContext()->setContextProperty("rectangle_model", &modelRectangle);
    qmlView.rootContext()->setContextProperty("circle_model", &modelCircle);

    qmlView.setSource(QUrl(QStringLiteral("qrc:qml/mapqmlfile.qml")));
    qmlView.setResizeMode(QQuickView::SizeRootObjectToView);

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(container);
    setLayout (layout);
    //addMarker(latitude, longitude)

    //setMinimumSize (200, 200); //This minimum size may be relative to another widget... (maybe the mainwindow?)
    //connect(okButton, SIGNAL(clicked()), this, SLOT(slotButtonClicked() ) );
}

void MapWidget::clearMap()
{
    modelRectangle.clear();
}

void MapWidget::setCenter(const Coordinate &_c)
{
    QObject *object = qmlView.rootObject ();
    object->setProperty ("zoom", 8.0);
    object->setProperty ("lat", _c.lat);
    object->setProperty ("lon", _c.lon);
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWidget::addLocator(const double lat1, const double lon1, const double lat2, const double lon2)
{
    QObject *object = qmlView.rootObject ();
    //MapRectangle
    object->setProperty ("locLat1", lat1);
    object->setProperty ("locLon1", lon1);
    object->setProperty ("locLat2", lat2);
    object->setProperty ("locLon2", lon2);
    //qDebug() << Q_FUNC_INFO << " - END";
}

void MapWidget::addMarker(const Coordinate _coord)
{
    //qDebug() << Q_FUNC_INFO;
    QObject *object = qmlView.rootObject ();
    QMetaObject::invokeMethod(object, "addMarker",
            Q_ARG(double, _coord.lat), Q_ARG(double, _coord.lon));
//    QMetaObject::invokeMethod(object, "addMarker",
//            Q_RETURN_ARG(QString, returnedValue),
//            Q_ARG(double, 40.5), Q_ARG(double, -3.5));

}

void MapWidget::addQSO(const QString &_loc)
{
    //qDebug() << Q_FUNC_INFO << ": " << _loc;
    if (!locator.isValidLocator(_loc))
    {
        return;
    }
    qmlView.rootContext()->setContextProperty("circle_model", &modelCircle);
    //qmlView.setSource(QUrl(QStringLiteral("qrc:qml/mapqmlfile.qml")));
    QStandardItem *item = new QStandardItem;
    item->setData(QVariant::fromValue(QGeoCoordinate(locator.getLat(_loc), locator.getLon(_loc))), CoordinateRole);
    modelCircle.appendRow(item);
}

void MapWidget::addLocator(const QString &_loc, const QColor &_color)
{
    if (_loc.contains ("IN99"))
    {
        qDebug() << Q_FUNC_INFO << ": " << _loc;
    }
    //qDebug() << Q_FUNC_INFO << ": " << _loc;
    if (!locator.isValidLocator(_loc))
    {
        return;
    }

    qmlView.rootContext()->setContextProperty("rectangle_model", &modelRectangle);

    Coordinate _north, _south;
    _north = locator.getLocatorCorner(_loc, true);
    _south = locator.getLocatorCorner(_loc, false);

    QGeoRectangle rect;
    rect.setTopLeft (QGeoCoordinate(_north.lat, _north.lon));
    rect.setBottomRight (QGeoCoordinate(_south.lat, _south.lon) );

    if (rect.isValid ())
    {
        QStandardItem *item = new QStandardItem;
        item->setData(QVariant::fromValue(QGeoCoordinate(_north.lat, _north.lon)), NorthRole);
        item->setData(QVariant::fromValue(QGeoCoordinate(_south.lat, _south.lon)), SouthRole);
        item->setData(QVariant::fromValue(_color), ColorRole);
        modelRectangle.appendRow(item);
        //qDebug() << Q_FUNC_INFO << " Rectangle OK";
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " Rectangle NOK";
    }
}
