/***************************************************************************
                          tst_mainqsoentrywidget.h  -  description
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
#include <QStringList>
#include "../../src/inputwidgets/mainwindowinputothers.h"
#include "../../src/dataproxy_sqlite.h"
#include "../../src/utilities.h"

class tst_MainWindowInputOthers : public QObject
{
    Q_OBJECT

public:
    tst_MainWindowInputOthers();
    ~tst_MainWindowInputOthers();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_Constructor();
    void test_Setters();
    void test_PropModes();
    void test_Subdivisions();


/*

    void setPropModeFromSat(const QString &_p);

    void setModifying(const bool _m);

*/

private:
    DataProxy_SQLite *dataProxy;
    Utilities *util;

    MainWindowInputOthers *mainWindowInputOthers;
};

tst_MainWindowInputOthers::tst_MainWindowInputOthers()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO, "0.0");
    util = new Utilities(Q_FUNC_INFO);
    util->setLongPrefixes(dataProxy->getLongPrefixes());
    util->setSpecialCalls(dataProxy->getSpecialCallsigns());
    mainWindowInputOthers = new MainWindowInputOthers(dataProxy);
    mainWindowInputOthers->setEntitiesList(dataProxy->getEntitiesNames());
   //qDebug() << Q_FUNC_INFO << " - CurrentProp" << mainWindowInputOthers->getPropModeFromComboBox() ;

}

tst_MainWindowInputOthers::~tst_MainWindowInputOthers(){}

void tst_MainWindowInputOthers::initTestCase()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
}

void tst_MainWindowInputOthers::cleanupTestCase()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
}



void tst_MainWindowInputOthers::test_Constructor()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    //distance = 0;
    //age = 0;
    //currentInt = 0;
    QVERIFY2(mainWindowInputOthers->getDistance() == 0.0, "Wrong Distance");
    QVERIFY2(mainWindowInputOthers->getAge() == 0, "Wrong Age");
    QVERIFY2(mainWindowInputOthers->getSIG() == QString(), "Wrong Sig");
    QVERIFY2(mainWindowInputOthers->getSIG_INFO() == QString(), "Wrong Sig_info");
    QVERIFY2(mainWindowInputOthers->getWWFF_Ref() == QString(), "Wrong WWFF_Ref");
    QVERIFY2(mainWindowInputOthers->getPOTA_REF() == QString(), "Wrong POTA_Ref");

}

void tst_MainWindowInputOthers::test_Setters()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    mainWindowInputOthers->clear();
    mainWindowInputOthers->setAge(10.1);
    QVERIFY2(mainWindowInputOthers->getAge() == 10.1, "Wrong Age (10.1)");
    mainWindowInputOthers->setDistance(10.2);
    QVERIFY2(mainWindowInputOthers->getDistance() == 10.2, "Wrong Distance (10.2)");
    mainWindowInputOthers->setIOTA("EU-001");
    QVERIFY2(mainWindowInputOthers->getIOTA() == "EU-001", "Wrong IOTA (EO-001)");
    mainWindowInputOthers->setVUCCGrids("IN80,IN70");
    QVERIFY2(mainWindowInputOthers->getVUCCGrids() == "IN80,IN70", "Wrong VUCC Grids");
    mainWindowInputOthers->setSOTA("EA4/MD-001");
    QVERIFY2(mainWindowInputOthers->getSOTA() == "EA4/MD-001", "Wrong SOTA (EA4/MD-001)");
    mainWindowInputOthers->setSIG("S-001");
    QVERIFY2(mainWindowInputOthers->getSIG() == "S-001", "Wrong Sig (S-001)");
    mainWindowInputOthers->setSIG_INFO("S-001");
    QVERIFY2(mainWindowInputOthers->getSIG_INFO() == "S-001", "Wrong Sig Info (S-001)");
    mainWindowInputOthers->setPOTA_REF("EA-0151");
    QVERIFY2(mainWindowInputOthers->getPOTA_REF() == "EA-0151", "Wrong POTA (EA-151)");
    mainWindowInputOthers->setWWFF_Ref("EAFF-0163");
    QVERIFY2(mainWindowInputOthers->getWWFF_Ref() == "EAFF-0163", "Wrong WWFF (EAFF-0163)");
   //qDebug() << Q_FUNC_INFO << " - Testing PROP_MODE";

    mainWindowInputOthers->setPropMode("SAT", false);
    QVERIFY2(mainWindowInputOthers->getPropModeFromComboBox() == "SAT", "Wrong PROP_MODE (SAT)");
   //qDebug() << Q_FUNC_INFO << " - PROP_MODE-1: " << mainWindowInputOthers->getPropModeFromComboBox();
   // mainWindowInputOthers->setPropMode("ES", false);              // Wrong PROP_MODE
   //qDebug() << Q_FUNC_INFO << " - PROP_MODE-2: " << mainWindowInputOthers->getPropModeFromComboBox();
    //QVERIFY2(mainWindowInputOthers->getPropModeFromComboBox() == "SAT", "Wrong wrong PROP_MODE (SAT)");
/*

    othersTabWidget->updatePrimarySubDivisions(qsoE.getDXCC(), qsoE.getCall());
    othersTabWidget->setState(qsoE.getState());

*/

}

void tst_MainWindowInputOthers::test_PropModes()
{
    QStringList propModes;
    propModes.clear();
    propModes.append(dataProxy->getPropModeList());

    QString aux;
    foreach (aux, propModes)
    {
       //qDebug() << Q_FUNC_INFO << " - PropMode: " << aux;
        QStringList fields;
        fields.clear();
        fields.append(aux.split('-'));
        QString field = fields.at(1);
        field = field.simplified();
       //qDebug() << Q_FUNC_INFO << " - ShortName: " << field;
        mainWindowInputOthers->setPropMode(field, false);
        QVERIFY2(mainWindowInputOthers->getPropModeFromComboBox() == field, "Wrong propMode in list");
    }
}

void tst_MainWindowInputOthers::test_Subdivisions()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    mainWindowInputOthers->clear(true);
    int currentEntity = 281; // 281 = EA


    mainWindowInputOthers->setIOTAContinentFromEntity(currentEntity);

   //qDebug() << Q_FUNC_INFO << ": IOTACont: " << mainWindowInputOthers->iotaContinentComboBox->currentText();
   //qDebug() << Q_FUNC_INFO << ": " << mainWindowInputOthers->getEntityPrefix();


    //QVERIFY2( util->isSameFreq (mainWindowInputQSO->getRXPwr(), 1.1), "Wrong RX Pwr");

}



QTEST_MAIN(tst_MainWindowInputOthers)

#include "tst_mainwindowinputothers.moc"
