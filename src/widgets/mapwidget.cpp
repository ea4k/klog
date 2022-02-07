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

MapWidget::MapWidget()
{

    qDebug() << Q_FUNC_INFO;
    testButton = new QPushButton;
    testButton->setText ("Push");
    lat = 0.0;
    lon = 0.0;

    QWidget *container = QWidget::createWindowContainer(&qmlView, this);

    roles[CoordinateRole] = QByteArray("coordinate");
    roles[NorthRole] = QByteArray("north");
    roles[SouthRole] = QByteArray("south");
    roles[ColorRole] = QByteArray("color");

    modelCircle.setItemRoleNames(roles);
    modelRectangle.setItemRoleNames(roles);


    qmlView.rootContext()->setContextProperty("rectangle_model", &modelRectangle);
    qmlView.rootContext()->setContextProperty("circle_model", &modelCircle);
    qmlView.setSource(QUrl(QStringLiteral("qrc:qml/mapqmlfile.qml")));
    qmlView.setResizeMode(QQuickView::SizeRootObjectToView);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget (testButton);
    layout->addWidget(container);
    setLayout (layout);

    setMinimumSize (200, 200); //This minimum size may be relative to another widget... (maybe the mainwindow?)
    connect(testButton, SIGNAL(clicked()), this, SLOT(slotButtonClicked() ) );
    qDebug() << Q_FUNC_INFO << " - END";
}

void MapWidget::setCenter(const double lat, const double lon)
{
    QObject *object = qmlView.rootObject ();
    //object->setProperty ("zoom", 1);
    object->setProperty ("lat", lat);
    object->setProperty ("lon", lon);
    qDebug() << Q_FUNC_INFO << " - END";
}

void MapWidget::addLocator(const double lat1, const double lon1, const double lat2, const double lon2)
{
    QObject *object = qmlView.rootObject ();
    //MapRectangle
    object->setProperty ("locLat1", lat1);
    object->setProperty ("locLon1", lon1);
    object->setProperty ("locLat2", lat2);
    object->setProperty ("locLon2", lon2);
    qDebug() << Q_FUNC_INFO << " - END";
}

void MapWidget::slotButtonClicked ()
{
    qDebug() << "MapWidget::slotButtonClicked ";
    Coordinate _north, _south;
    _north = locator.getLocatorCorner("IN80", true);
    _south = locator.getLocatorCorner("IN80", false);

    QGeoRectangle rect;
    rect.setTopLeft (QGeoCoordinate(_north.lat, _north.lon));
    rect.setBottomRight (QGeoCoordinate(_south.lat, _south.lon) );


    if (rect.isValid ())
    {
        qDebug() << Q_FUNC_INFO << " Rectangle OK";
    }
    else
    {
        qDebug() << Q_FUNC_INFO << " Rectangle NOK";
    }
    QStandardItem *item = new QStandardItem;
    item->setData(QVariant::fromValue(QGeoCoordinate(_north.lat, _north.lon)), NorthRole);
    item->setData(QVariant::fromValue(QGeoCoordinate(_south.lat, _south.lon)), SouthRole);
    //item->setData(QVariant::fromValue(Qt::blue), ColorRole);
    modelRectangle.appendRow(item);

    qDebug() << Q_FUNC_INFO << " - END";
}

void MapWidget::addQSO(const QString &_loc)
{
    qDebug() << Q_FUNC_INFO << ": " << _loc;
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

void MapWidget::addWorkedLocator(const QString &_loc)
{
    qDebug() << Q_FUNC_INFO << ": " << _loc;
    if (!locator.isValidLocator(_loc))
    {
        return;
    }
    qmlView.rootContext()->setContextProperty("rectangle_model", &modelRectangle);
    //qmlView.setSource(QUrl(QStringLiteral("qrc:qml/mapqmlfile.qml")));

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
        modelRectangle.appendRow(item);
    }
    else
    {
        qDebug() << Q_FUNC_INFO << " Rectangle NOK";
    }
}
