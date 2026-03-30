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

#include <QQuickView>
#include <QQuickWidget>
#include <QGuiApplication>
#include <QGeoCoordinate>
#include <QGeoRectangle>
#include <QQmlContext>
#include <QQmlEngine>
#include <QStandardItemModel>
#include "mapwidget.h"

MapWidget::MapWidget(QWidget *parent)
    : QWidget(parent)
{
    //Q_UNUSED(parent);
}

MapWidget::~MapWidget()
{
    if (m_quickView || m_quickWidget) {
        if (m_quickView)   m_quickView->setSource(QUrl());
        else               m_quickWidget->setSource(QUrl());
        qmlEngine()->collectGarbage();
        QCoreApplication::processEvents();
    }
}

QQmlContext *MapWidget::qmlContext() const
{
    return m_quickView ? m_quickView->rootContext()
                       : m_quickWidget->rootContext();
}

QObject *MapWidget::qmlRoot() const
{
    return m_quickView ? m_quickView->rootObject()
                       : m_quickWidget->rootObject();
}

QQmlEngine *MapWidget::qmlEngine() const
{
    return m_quickView ? m_quickView->engine()
                       : m_quickWidget->engine();
}

void MapWidget::init()
{
    createUI();
}

void MapWidget::createUI()
{
    // QQuickWidget renders into an offscreen FBO: on Wayland the compositor
    // never registers that surface as an input target, so pointer handlers
    // (DragHandler, TapHandler, HoverHandler) in QML never receive events.
    // QQuickView + createWindowContainer creates a real native window that
    // the Wayland compositor routes input into correctly.
    // On X11 / Windows / macOS QQuickWidget is used because it composes
    // cleanly with the widget stack (no z-ordering or clipping issues).
    const bool isWayland = QGuiApplication::platformName() == QLatin1String("wayland");
    if (isWayland) {
        m_quickView = new QQuickView();
        m_quickView->setResizeMode(QQuickView::SizeRootObjectToView);
    } else {
        m_quickWidget = new QQuickWidget(this);
        m_quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    }

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
    qmlContext()->setContextProperty("rectangle_model", &modelRectangle);
    qmlContext()->setContextProperty("circle_model",    &modelCircle);
    qmlContext()->setContextProperty("grid_model",      &modelGrid);

    // Expose locatorInfoProvider (may be null until setLocatorInfoProvider is called)
    qmlContext()->setContextProperty("locatorInfo", locatorInfoProvider);

    if (m_quickView)   m_quickView->setSource(QUrl(QStringLiteral("qrc:///qml/mapqmlfile.qml")));
    else               m_quickWidget->setSource(QUrl(QStringLiteral("qrc:///qml/mapqmlfile.qml")));

    QWidget *viewWidget;
    if (m_quickView) {
        QWidget *container = QWidget::createWindowContainer(m_quickView, this);
        container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        container->setFocusPolicy(Qt::StrongFocus);
        viewWidget = container;
    } else {
        viewWidget = m_quickWidget;
    }

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(viewWidget);
    setLayout(layout);

    paintFieldGrid();

    // Forward the QML signals as our own C++ signals
    QObject *root = qmlRoot();
    if (root) {
        connect(root, SIGNAL(spotDoubleClicked(QString,double)),
                this, SIGNAL(spotDoubleClicked(QString,double)));
        connect(root, SIGNAL(editQSORequested(int)),
                this, SIGNAL(editQSORequested(int)));
    }
}

void MapWidget::setLocatorInfoProvider(LocatorInfoProvider *provider)
{
    locatorInfoProvider = provider;
    if (m_quickView || m_quickWidget)
        qmlContext()->setContextProperty("locatorInfo", locatorInfoProvider);
}

void MapWidget::clearDataLayers()
{
    // Only clear overlays; keep grid_model intact so the grid stays visible
    modelRectangle.clear();
    if (locatorInfoProvider)
        locatorInfoProvider->clearVisibleLocators();
    // modelCircle.clear(); // Uncomment if you want to clear markers as part of "data layers"
}

void MapWidget::setCenter(const Coordinate &_c)
{
    QObject *object = qmlRoot();
    if (!object) return;
    //QObject *object = qmlView.rootObject();
    object->setProperty("zoom", 8.0);
    object->setProperty("lat", _c.lat);
    object->setProperty("lon", _c.lon);
}

void MapWidget::addLocator(const double lat1, const double lon1, const double lat2, const double lon2)
{
    QObject *object = qmlRoot();
    if (!object) return;
    //QObject *object = qmlView.rootObject();
    object->setProperty("locLat1", lat1);
    object->setProperty("locLon1", lon1);
    object->setProperty("locLat2", lat2);
    object->setProperty("locLon2", lon2);
}

void MapWidget::clearMarkers()
{
    QObject *object = qmlRoot();
    if (!object) return;
    QMetaObject::invokeMethod(object, "clearMarkers");
}

void MapWidget::setSpotExpiryMinutes(int minutes)
{
    QObject *object = qmlRoot();
    if (!object) return;
    object->setProperty("spotExpiryMs", minutes * 60 * 1000);
}

void MapWidget::addMarker(const Coordinate _coord, const QString &_callsign, const QColor &_color, double frequencyMHz)
{
    QObject *object = qmlRoot();
    if (!object) return;
    QMetaObject::invokeMethod(object, "addMarker",
                              Q_ARG(QVariant, _coord.lat),
                              Q_ARG(QVariant, _coord.lon),
                              Q_ARG(QVariant, _callsign),
                              Q_ARG(QVariant, _color.name()),
                              Q_ARG(QVariant, frequencyMHz));
}

void MapWidget::addQSO(const QString &_loc)
{
    if (!locator.isValidLocator(_loc))
        return;

    qmlContext()->setContextProperty("circle_model", &modelCircle);
    QStandardItem *item = new QStandardItem;
    item->setData(QVariant::fromValue(QGeoCoordinate(locator.getLat(_loc), locator.getLon(_loc))), CoordinateRole);
    modelCircle.appendRow(item);
}

void MapWidget::addLocator(const QString &_loc, const QColor &_color)
{
    if (!locator.isValidLocator(_loc))
        return;

    qmlContext()->setContextProperty("rectangle_model", &modelRectangle);

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

        if (locatorInfoProvider)
            locatorInfoProvider->addVisibleLocator(_loc);
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
