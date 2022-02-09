#ifndef MAPWINDOWWIDGET_H
#define MAPWINDOWWIDGET_H
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
#include "klogdefinitions.h"
#include "mapwidget.h"
#include "dataproxy_sqlite.h"

class MapWindowWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWindowWidget(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    //MapWindowWidget();
    void init();
    void setBands(QStringList _bands);
    void setModes(QStringList _modes);
    void setCenter(const QString _grid);
    void addQSO(const QString &_loc);
    void addLocator(const QString &_loc, const QColor &_color);
    void addLocators(const QStringList &_locators);


private slots:
    void slotBandsComboBoxChanged(const QString &_c);
    void slotModesComboBoxChanged(const QString &_c);
    void slotPropComboBoxChanged(const QString &_c);
    void slotSatsComboBoxChanged(const QString &_c);
    void slotConfirmedCheckBoxChanged();
    void slotLocatorsCheckBoxChanged();

private:
    void createUI();
    void setPropModes();
    void setSatNames();
    void showFiltered();
    DataProxy_SQLite *dataProxy;
    MapWidget *mapWidget;
    QComboBox *propComboBox, *bandComboBox, *modeComboBox, *satNameComboBox;
    QCheckBox *confirmedCheckBox, *locatorsCheckBox;
    //QPushButton *okButton;

};

#endif // MAPWINDOWWIDGET_H
