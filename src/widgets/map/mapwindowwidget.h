#ifndef KLOG_WIDGETS_MAP_MAPWINDOWWIDGET_H
#define KLOG_WIDGETS_MAP_MAPWINDOWWIDGET_H
/***************************************************************************
                          mapwindowwidget.h  -  description
                             -------------------
    begin                : Feb 2022
    copyright            : (C) 2022 by Jaime Robles
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
//#include <QWidget>
#include "mapwidget.h"
#include "../../klogdefinitions.h"
#include "../../dataproxy_sqlite.h"
#include "../../locator.h"

class MapWindowWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWindowWidget(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    ~MapWindowWidget();
    void init();
    void setBands(QStringList _bands);
    void setModes(QStringList _modes);
    void setCenter(const Coordinate &_c);
    void addQSO(const QString &_loc);
    void addLocator(const QString &_loc, const QColor &_color);
    void addLocators(const QStringList &_locators, const QColor &_color);
    void appendLocators(const QStringList &_locators, const QColor &_color);
    void setColors (const QColor &_worked, const QColor &_confirmed, const QColor &_default);
    void addMarker(const Coordinate _coord, const QString _loc);

private slots:
    void slotBandsComboBoxChanged();
    void slotModesComboBoxChanged();
    void slotPropComboBoxChanged();
    void slotSatsComboBoxChanged();
    void slotConfirmedCheckBoxChanged();
    //void slotLocatorsCheckBoxChanged();

private:
    void createUI();
    void paintGlobalGrid();
    void setPropModes();
    void setSatNames();
    void showFiltered();
    QString getShortLocators (const int _length);
    QString getPropModeFromComboBox();

    DataProxy_SQLite *dataProxy;
    MapWidget *mapWidget;
    QComboBox *propComboBox, *bandComboBox, *modeComboBox, *satNameComboBox;
    QCheckBox *confirmedCheckBox;//, *locatorsCheckBox;

    QColor workedColor;
    QColor confirmedColor;
    QColor defaultColor;

};

#endif // MAPWINDOWWIDGET_H
