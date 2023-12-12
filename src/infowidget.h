#ifndef KLOG_INFOWIDGET_H
#define KLOG_INFOWIDGET_H
/***************************************************************************
                          infowidget.h  -  description
                             -------------------
    begin                : ago 2017
    copyright            : (C) 2017 by Jaime Robles
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
//
// This class implement the Info widget where the band, bearing, CQ & ITU zones.... are shown
//
#include <QWidget>
#include <QtWidgets>
#include "locator.h"
#include "awards.h"
#include "world.h"
#include "dataproxy_sqlite.h"
#include "dataproxy_sqlite.h"



class InfoWidget : public QWidget
{
    Q_OBJECT
    friend class tst_MainWindow;

public:
    InfoWidget(DataProxy_SQLite *dp, QWidget *parent = 0);
    ~InfoWidget();
    void createUI();
    void clear();
    void setCurrentLog(const int _log);
    void setColors (const QString &_newOne, const QString &_needed, const QString &_worked, const QString &_confirmed, const QString &_default);
    void setImperialSystem (const  bool _imp);
    void showInfo(const int _entity);
    void showDistanceAndBearing(const QString &_locLocal, const QString &_loc2);
    void showEntityInfo(const int _enti, int _cq=-1, int _itu=-1);
    void setLocalLocator(const QString &_loc);   //MAYBE NOT NEEDED
    void setDXLocator(const QString &_loc);      //MAYBE NOT NEEDED
    int getDistance(bool shortPath=true);                          // Returns the distance in KM

private:
    void clearBandLabels();
    void clearInfoFromLocators();
    QString getStyleColorToLabelFromBand(const QString &_b, const QString &_q);

    QLabel *bandLabel1, *bandLabel2, *bandLabel3, *bandLabel4;
    QLabel *bandLabel5, *bandLabel6, *bandLabel7, *bandLabel8;
    QLabel *bandLabel9, *bandLabel10, *bandLabel11, *bandLabel12;
    QLabel *continentLabel, *prefixLabel, *cqzLabel, *ituzLabel;
    QLabel *gradShortLabel, *distShortLabel;
    QLabel *gradLongLabel, *distLongLabel;
    QLabel *distShortLabelN;
    QLabel *distLongLabelN;

    Awards *awards;
    DataProxy_SQLite *dataProxy;
    Locator *locator;
    World *world;

    int currentLog;
    bool imperialSystem;
    QString dxLocator, localLocator;

};

#endif // INFOWIDGET_H
