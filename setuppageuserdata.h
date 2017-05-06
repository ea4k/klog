#ifndef SETUPPAGEUSERDATA_H
#define SETUPPAGEUSERDATA_H

/***************************************************************************
                          setuppageuserdata.h  -  description
                             -------------------
    begin                : sept 2011
    copyright            : (C) 2011 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                             *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify        *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,             *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/

#include <QtWidgets>
//#include <QtWidgets>
#include "world.h"
#include "locator.h"


class SetupPageUserDataPage : public QWidget {
    Q_OBJECT

public:
    SetupPageUserDataPage(QWidget *parent=0);
    ~SetupPageUserDataPage();

    QString getStationQrz();
    QString getOperators();

    QString getStationLocator();
    int getCQz();
    int getITUz();

    QString getName();
    QStringList getAddress();
    QString getAddress1();
    QString getAddress2();
    QString getAddress3();
    QString getAddress4();
    QString getCity();
    QString getZipCode();
    QString getProvince();
    QString getCountry();

    bool setName (const QString _aux);
    bool setAddress1 (const QString _aux);
    bool setAddress2 (const QString _aux);
    bool setAddress3 (const QString _aux);
    bool setAddress4 (const QString _aux);
    bool setAddress (const QStringList _aux);

    bool setCity (const QString _aux);
    bool setZipCode (const QString _aux);
    bool setProvince (const QString _aux);
    bool setCountry (const QString _aux);

    bool setStationQrz(const QString _qrz);
    bool setOperators(const QString _aux);

    bool setStationLocator(const QString _loc);
    bool setCQz(const int _cqz);
    bool setITUz(const int _ituz);

    QString getRig1();
    QString getRig2();
    QString getRig3();
    QString getAntenna1();
    QString getAntenna2();
    QString getAntenna3();
    QStringList getRigs();
    QStringList getAntennas();
    QString getPower();

    bool setRigs(const QStringList _aux);
    bool setAntennas(const QStringList _aux);
    bool setPower(const QString _aux);
    bool setRig1 (const QString _aux);
    bool setRig2 (const QString _aux);
    bool setRig3 (const QString _aux);

    bool setAntenna1 (const QString _aux);
    bool setAntenna2 (const QString _aux);
    bool setAntenna3 (const QString _aux);

signals:
    void stationCallSignal (const QString _p);
    void operatorsSignal (const QString _p);

private slots:

    void slotMyLocatorTextChanged();
    void slotOperatorsChanged();


    void slotQRZTextChanged();
//    void slotContestOverLayChanged(int i);

private:
    bool checkOperatorsLineQString(const QString _auxLine);

    QTabWidget *tabWidget;

    QLineEdit *qrzLineEdit; // Station Callsign
    QLineEdit *operatorsLineEdit; //Operators
    QLineEdit *cqzLineEdit;
    QLineEdit *ituzLineEdit;
    QLineEdit *myLocatorLineEdit;
    QLabel *myLocatorLabel;

    //Personal Tab
    QLineEdit *nameLineEdit;
    QTextEdit *addressTextEdit;
    QLineEdit *address1LineEdit;
    QLineEdit *address2LineEdit;
    QLineEdit *address3LineEdit;
    QLineEdit *address4LineEdit;
    QLineEdit *cityLineEdit;
    QLineEdit *zipLineEdit;
    QLineEdit *provinceLineEdit;
    QLineEdit *countryLineEdit;

    // Station Tab
    QLineEdit *rig1LineEdit;
    QLineEdit *rig2LineEdit;
    QLineEdit *rig3LineEdit;
    QLineEdit *ant1LineEdit;
    QLineEdit *ant2LineEdit;
    QLineEdit *ant3LineEdit;
    //QLineEdit *powerLineEdit;
    QDoubleSpinBox *myPowerSpinBox;

    QPalette *defaultPalette, *wrongPalette;
    QColor redColor;

    Locator *locator;
    World world;

    bool operatorsOK;
    bool operatorOK;


};


#endif // SETUPPAGEUSERDATA_H
