/***************************************************************************
                          tst_hamlib.h  -  description
                             -------------------
    begin                : Jan 2021
    copyright            : (C) 2021 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of Testing suite of KLog.                               *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify           *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    The testing suite of KLog is distributed in the hope that it will      *
 *    be useful, but WITHOUT ANY WARRANTY; without even the implied          *
 *    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.       *
 *    See the GNU General Public License for more details.                   *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with Testing suite of KLog.                                      *
 *    If not, see <https://www.gnu.org/licenses/>.                           *
 *                                                                           *
 *****************************************************************************/

#include <QtTest>
#include <QDebug>
#include <QMap>
#include <QObject>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <hamlib/rig.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <QSettings>
#include "../../src/klogdefinitions.h"
#include "../../src/hamlibclass.h"
#include "../../src/utilities.h"

class tst_Hamlib : public QObject
{
    Q_OBJECT

public:
    tst_Hamlib();
    ~tst_Hamlib();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_Constructor();
    void test_start();

private:
    HamLibClass *hamlib;
    int backupModelId;
};

tst_Hamlib::tst_Hamlib()
{
    hamlib = new HamLibClass;
}

tst_Hamlib::~tst_Hamlib(){}

void tst_Hamlib::initTestCase(){

    Utilities util(Q_FUNC_INFO);
    QSettings settings(util.getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("HamLib");
    backupModelId = settings.value("HamLibRigType", 0).toInt();
    QVERIFY2(backupModelId > 0, "ModelID could not be read");
    settings.setValue("HamLibRigType", 1);
    int backupModel = settings.value("HamLibRigType", 0).toInt();
    QVERIFY2(backupModel == 1, "backupModelId could not be saved for later");


    //setModelId(settings.value("HamLibRigType", 0).toInt());
    //setPort(settings.value ("HamlibSerialPort").toString());
    //setSpeed(settings.value ("HamlibSerialBauds", 9600).toInt ());
    //setDataBits(settings.value ("HamLibSerialDataBits", 8).toInt ());
    //setStop(settings.value ("HamLibSerialStopBit", "OneStop").toString());
    //setFlow(settings.value ("HamLibSerialFlowControl", "None").toString());
    //setParity(settings.value ("HamLibSerialParity", "Even").toString());
    //setPoll(settings.value ("HamlibRigPollRate", 2000).toInt ());
    //setReadOnly(settings.value ("HamlibReadOnly", false).toBool ());
    //setNetworkAddress (settings.value ("HamlibNetAddress").toString());
    //setNetworkPort (settings.value ("HamlibNetPort", 4532).toInt ());
    settings.endGroup ();

}

void tst_Hamlib::cleanupTestCase(){}

void tst_Hamlib::test_Constructor()
{

}

void tst_Hamlib::test_start(){
    hamlib->initClass();
    hamlib->init(true);
    QVERIFY2(hamlib->init(true), "Hamlib failed in init(true)");
    QVERIFY2(hamlib->init(false), "Hamlib failed in init(false)");
}

QTEST_APPLESS_MAIN(tst_Hamlib)

#include "tst_hamlib.moc"
