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
 *    KLog is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *                                                                           *
 *    You should have received a copy of the GNU General Public License
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.
 *                                                                           *
 *****************************************************************************/

#include <QQuickWidget>
#include <QGeoCoordinate>
#include <QGeoRectangle>
#include <QQmlContext>
#include <QQmlEngine>
#include <QStandardItemModel>
#include "mapwidget.h"

MapWidget::MapWidget(QWidget *parent)
    : QWidget(parent)
    , qmlView(nullptr)
{
    //Q_UNUSED(parent);
}

MapWidget::~MapWidget()
{
    if (qmlView) {
        qmlView->setSource(QUrl());
        qmlView->engine()->collectGarbage();
        QCoreApplication::processEvents();
    }
}

void MapWidget::init()
{
    createUI();
}

void MapWidget::createUI()
{
    qmlView = new QQuickWidget(this);
    qmlView->setResizeMode(QQuickWidget::SizeRootObjectToView);

    // Circle roles
    circleRoles[CoordinateRole] = QByteArray("coordinate");
    modelCircle.setItemRoleNames(circleRoles);

    // Data overlay rectangle roles
    rectangleRoles[NorthRole] = QByteArray("north");
    rectangleRoles[SouthRole] = QByteArray("south");
    rectangleRoles[ColorRole] = QByteArray("color");
    modelRectangle.setItemRoleNames(rectangleRoles);

    // Grid rectangle roles (no color role; grid has transparent fill, only borders)
    gridRoles[GridNorthRole] = QByteArray("north");
    gridRoles[GridSouthRole] = QByteArray("south");
    modelGrid.setItemRoleNames(gridRoles);

    // Expose models to QML
    qmlView->rootContext()->setContextProperty("rectangle_model", &modelRectangle);
    qmlView->rootContext()->setContextProperty("circle_model",    &modelCircle);
    qmlView->rootContext()->setContextProperty("grid_model",      &modelGrid);

    qmlView->setSource(QUrl(QStringLiteral("qrc:///qml/mapqmlfile.qml")));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(qmlView);
    setLayout(layout);

    paintFieldGrid();
}

void MapWidget::clearDataLayers()
{
    // Only clear overlays; keep grid_model intact so the grid stays visible
    modelRectangle.clear();
    // modelCircle.clear(); // Uncomment if you want to clear markers as part of "data layers"
}

void MapWidget::setCenter(const Coordinate &_c)
{
    QObject *object = qmlView->rootObject();
    if (!object) return;
    //QObject *object = qmlView.rootObject();
    object->setProperty("zoom", 8.0);
    object->setProperty("lat", _c.lat);
    object->setProperty("lon", _c.lon);
}

void MapWidget::addLocator(const double lat1, const double lon1, const double lat2, const double lon2)
{
    QObject *object = qmlView->rootObject();
    if (!object) return;
    //QObject *object = qmlView.rootObject();
    object->setProperty("locLat1", lat1);
    object->setProperty("locLon1", lon1);
    object->setProperty("locLat2", lat2);
    object->setProperty("locLon2", lon2);
}

void MapWidget::addMarker(const Coordinate _coord, const QString &_callsign, const QColor &_color)
{
    QObject *object = qmlView->rootObject();
    if (!object) return;
    QMetaObject::invokeMethod(object, "addMarker",
                              Q_ARG(QVariant, _coord.lat),
                              Q_ARG(QVariant, _coord.lon),
                              Q_ARG(QVariant, _callsign),
                              Q_ARG(QVariant, _color.name()));
}

void MapWidget::addQSO(const QString &_loc)
{
    if (!locator.isValidLocator(_loc))
        return;

    qmlView->rootContext()->setContextProperty("circle_model", &modelCircle);
    QStandardItem *item = new QStandardItem;
    item->setData(QVariant::fromValue(QGeoCoordinate(locator.getLat(_loc), locator.getLon(_loc))), CoordinateRole);
    modelCircle.appendRow(item);
}

void MapWidget::addLocator(const QString &_loc, const QColor &_color)
{
    if (!locator.isValidLocator(_loc))
        return;

    qmlView->rootContext()->setContextProperty("rectangle_model", &modelRectangle);

    Coordinate _north = locator.getLocatorCorner(_loc, true);
    Coordinate _south = locator.getLocatorCorner(_loc, false);

    QGeoRectangle rect;
    rect.setTopLeft(QGeoCoordinate(_north.lat, _north.lon));
    rect.setBottomRight(QGeoCoordinate(_south.lat, _south.lon));

    if (rect.isValid())
    {
        QStandardItem *item = new QStandardItem;
        item->setData(QVariant::fromValue(QGeoCoordinate(_north.lat, _north.lon)), NorthRole);
        item->setData(QVariant::fromValue(QGeoCoordinate(_south.lat, _south.lon)), SouthRole);
        item->setData(QVariant::fromValue(_color), ColorRole);
        modelRectangle.appendRow(item);
    }
}

void MapWidget::paintFieldGrid()
{
    // Draw all 2-letter fields (A..R x A..R)
    modelGrid.clear();
    for (int lonIdx = 0; lonIdx < 18; ++lonIdx) {
        for (int latIdx = 0; latIdx < 18; ++latIdx) {
            const QChar lonChar = QChar('A' + lonIdx);
            const QChar latChar = QChar('A' + latIdx);
            const QString field = QString("%1%2").arg(lonChar).arg(latChar);

            if (!locator.isValidLocator(field))
                continue;

            Coordinate north = locator.getLocatorCorner(field, true);
            Coordinate south = locator.getLocatorCorner(field, false);

            QGeoRectangle rect;
            rect.setTopLeft(QGeoCoordinate(north.lat, north.lon));
            rect.setBottomRight(QGeoCoordinate(south.lat, south.lon));

            if (rect.isValid()) {
                QStandardItem *item = new QStandardItem;
                item->setData(QVariant::fromValue(QGeoCoordinate(north.lat, north.lon)), GridNorthRole);
                item->setData(QVariant::fromValue(QGeoCoordinate(south.lat, south.lon)), GridSouthRole);
                modelGrid.appendRow(item);
            }
        }
    }
}
