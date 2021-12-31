#ifndef MAINWINDOWINPUTOTHERS_H
#define MAINWINDOWINPUTOTHERS_H
/***************************************************************************
                          mainwindowminputothers.h  -  description
                             -------------------
    begin                : ago 2016
    copyright            : (C) 2016 by Jaime Robles
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
// This class implement the tab of the mainwindow that supports the Others tab
//
#include <QWidget>
#include <QtWidgets>
#include "../dataproxy_sqlite.h"
#include "../utilities.h"
#include "../klogdefinitions.h"

class MainWindowInputOthers : public QWidget
{
    Q_OBJECT

public:
    MainWindowInputOthers(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    ~MainWindowInputOthers();

    void setEntitiesList(const QStringList _qs);
    void setEntity(const int _n);
    int getEntity();
    QString getEntityPrefix();

    void setPropMode(const QString &_qs, bool _keep);
    QString getPropModeFromComboBox();
    bool isSATPropagation();

    bool setUserADIFTypeComboBox(const QString &_value);
    QString getUserADIFTypeComboBox();
    bool setUserADIFValue(const QString &_adifValue);
    QString getUserADIFValue();

    bool setSOTA(const QString &_op);
    QString getSOTA();
    bool setAge(const double _op);
    double getAge();
    bool setVUCCGrids(const QString &_op);
    QString getVUCCGrids();

    void setIOTA(const QString &_qs);
    QString getIOTA();
    void setIOTAContinentFromEntity(const int _n);
    void setIOTAContinent(const QString &_qs);
    bool isIOTAModified();
    void clearIOTA();
    void setKeep(const bool _b);
    bool getKeep();

    void createUI();
    void clear(bool _full = false); //full= false leaves the "keep this data"; full = true clears everything

signals:
    void setPropMode(const QString _p);

private slots:
    //void slotSetPropMode(const QString &_p); // To receive the signal from the SAT widget and set "SAT" propagation mode, of needed.
    void slotPropModeComboBoxChanged();
    void slotUSerDefinedADIFComboBoxChanged();
    void slotSetCurrentUSerData();

private:
    QString checkIfValidIOTA(const QString &_tiota); //TODO: There is an equivalent function in the Awards class. I should use only one!
    void setColorsForUserDefinedADIFValueLineEdit();
    bool checkVUCC_GRIDS(const QString &_string);
    bool setInitialADIFValues();
    bool getDarkMode();

    Utilities *util;
    DataProxy_SQLite *dataProxy;

    QStringList entitiesList, propModeList;
    //QLabel *entityPrimLabel, *entitySecLabel, *iotaAwardLabel, *entityNameLabel, *propModeLabel;
    QComboBox *iotaContinentComboBox, *entityPrimDivComboBox, *entitySecDivComboBox, *entityNameComboBox, *propModeComboBox;
    QLineEdit *iotaNumberLineEdit;
    QCheckBox *keepPropCheckBox;

    QPalette palRed, palBlack, palWhite;
    bool autoUpdating;

    QComboBox *userDefinedADIFComboBox;
    QLineEdit *userDefinedADIFValueLineEdit;
    QStringList adifValidTypes;

    QString sota_ref, vucc_grids;
    double age;
};

#endif // MAINWINDOWINPUTOTHERS_H
