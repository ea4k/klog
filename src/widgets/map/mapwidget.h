#ifndef KLOG_WIDGETS_MAP_MAPWIDGET_H
#define KLOG_WIDGETS_MAP_MAPWIDGET_H
/***************************************************************************
                          mapwidget.h  -  description
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
#include <QObject>
#include <QtWidgets>
#include <QQuickWidget>
#include <QQuickItem>
#include "../../locator.h"

class MapWidget : public QWidget
{
    Q_OBJECT

public:
    MapWidget(QWidget *parent = nullptr);
    ~MapWidget();
    void init();

    void setCenter(const Coordinate &_c);
    void addLocator(const double lat1, const double lon1, const double lat2, const double lon2);
    void addQSO(const QString &_loc);
    void addMarker(const Coordinate _coord, const QString &_callsign, const QColor &_color);
    void addLocator(const QString &_loc, const QColor &_color);

    // Clears only data overlays (worked/confirmed rectangles and circles), NOT the base grid layer
    void clearDataLayers();

signals:
    void doAddMarker(double latitude, double longitude);

private:
    void createUI();

    // Build the global 2-letter Maidenhead field grid (drawn once, persistent)
    void paintFieldGrid();

    QQuickWidget *qmlView;

    // Models
    QStandardItemModel modelCircle;      // optional circle markers
    QStandardItemModel modelRectangle;   // data overlays (worked/confirmed)
    QStandardItemModel modelGrid;        // persistent field grid (2-letter)

    // Role name maps
    QHash<int, QByteArray> circleRoles;
    QHash<int, QByteArray> rectangleRoles;
    QHash<int, QByteArray> gridRoles;



    // Roles (keep distinct spaces)
    static constexpr int CoordinateRole = Qt::UserRole + 1000;

    // For overlays (rectangles)
    static constexpr int NorthRole      = Qt::UserRole + 1100;
    static constexpr int SouthRole      = Qt::UserRole + 1101;
    static constexpr int ColorRole      = Qt::UserRole + 1102;

    // For grid rectangles
    static constexpr int GridNorthRole  = Qt::UserRole + 1200;
    static constexpr int GridSouthRole  = Qt::UserRole + 1201;

    Locator locator;
};

#endif // MAPWIDGET_H
