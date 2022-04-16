#ifndef MAPWIDGET_H
#define MAPWIDGET_H
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
#include <QQuickView>
#include <QQuickItem>
#include "locator.h"

class MapWidget : public QWidget
{
    Q_OBJECT

public:
    MapWidget(QWidget *parent = nullptr);
    void init();

    void setCenter(const Coordinate &_c);
    void addLocator(const double lat1, const double lon1, const double lat2, const double lon2);
    void addQSO(const QString &_loc);
    void addMarker(const Coordinate _coord);
    void addLocator(const QString &_loc, const QColor &_color);
    void clearMap();
    //void setLocale (const QString _locale);

signals:
      void doAddMarker(double latitude, double longitude);

private slots:

private:
    void createUI();
    QQuickView qmlView;

    QStandardItemModel modelCircle, modelRectangle;
    QHash<int, QByteArray> circleRoles;
    QHash<int, QByteArray> rectangleRoles;

    int CoordinateRole = Qt::UserRole + 1000;
    int NorthRole = Qt::UserRole + 1000;
    int SouthRole = Qt::UserRole + 1001;
    int ColorRole = Qt::UserRole + 1002;
    int TextRole = Qt::UserRole + 1003;
    Locator locator;
    //double lat, lon;

};

#endif // MAPWIDGET_H
