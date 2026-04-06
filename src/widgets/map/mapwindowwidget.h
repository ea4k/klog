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
#include <QSettings>
#include <QShowEvent>
#include "mapwidget.h"
#include "locatorinfoprovider.h"
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
    KLOG_DEPRECATED void addQSO(const QString &_loc);
    void addLocator(const QString &_loc, const QColor &_color);
    void addLocators(const QStringList &_locators, const QColor &_color);
    void appendLocators(const QStringList &_locators, const QColor &_color);
    void setColors (const QColor &_newOne, const QColor &_needed, const QColor &_worked, const QColor &_confirmed, const QColor &_default);
    //void setColors (const QColor &_worked, const QColor &_confirmed, const QColor &_default);
    void addMarker(const Coordinate _coord, const QString &_callsign, const QColor &_color, double frequencyMHz = 0.0);
    void clearMarkers();
    void setSpotExpiryMinutes(int minutes);

signals:
    void spotDoubleClicked(const QString &callsign, double frequencyMHz);
    void editQSORequested(int qsoId);

private slots:
    void slotBandsComboBoxChanged();
    void slotModesComboBoxChanged();
    void slotPropComboBoxChanged();
    void slotSatsComboBoxChanged();
    void slotConfirmedCheckBoxChanged();
    //void slotLocatorsCheckBoxChanged();

protected:
    void showEvent(QShowEvent *event) override;

private:
    void createUI();
    KLOG_DEPRECATED void paintGlobalGrid();
    void setPropModes();
    void setSatNames();
    void showFiltered();
    KLOG_DEPRECATED QString getShortLocators (const int _length);
    QString getPropModeFromComboBox();

    DataProxy_SQLite *dataProxy;
    MapWidget *mapWidget;
    LocatorInfoProvider *locatorInfo;
    QComboBox *propComboBox, *bandComboBox, *modeComboBox, *satNameComboBox;
    QCheckBox *confirmedCheckBox;//, *locatorsCheckBox;

    QColor workedColor;
    QColor confirmedColor;
    QColor defaultColor;
    QColor newOneColor, neededColor;

    bool m_initialized = false;
    Coordinate m_pendingCenter;
    bool m_hasPendingCenter = false;
};

#endif // MAPWINDOWWIDGET_H
