/***************************************************************************
                          tst_setuphamlibserialwidget.h  -  description
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
#include "../../src/setuppages/hamlibserialconfigwidget.h"

class tst_SetupHamLibSerialWidget: public QObject
{
    Q_OBJECT

public:
    tst_SetupHamLibSerialWidget();
    ~tst_SetupHamLibSerialWidget();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_Constructor();
    void test_DataBits();
    void test_FlowControl();
    void test_Parity();
    void test_StopBits();
    void test_SerialSpeed();
    void test_SerialPort();


    /*

        int getDataBits();
        void setDataBits (const int _b);
        QString getFlowControl();
        void setFlowControl(const QString &_st);
        void setParity(const QString &_st);
        QString getParity();
        void setStopBits(const QString &_st);
        int getStopBits();
        bool setSerialSpeed(const int _speed );
        bool setSerialPort(const QString &_port);

        QString getSerialPort();
        int getSerialBauds();
    */



private:
    HamLibSerialConfigWidget *serialHamLibWidget;

};

tst_SetupHamLibSerialWidget::tst_SetupHamLibSerialWidget()
{
    serialHamLibWidget = new HamLibSerialConfigWidget();
    serialHamLibWidget->createUI();
}

tst_SetupHamLibSerialWidget::~tst_SetupHamLibSerialWidget(){}

void tst_SetupHamLibSerialWidget::initTestCase(){}

void tst_SetupHamLibSerialWidget::cleanupTestCase()
{

}


void tst_SetupHamLibSerialWidget::test_Constructor()
{

}

void tst_SetupHamLibSerialWidget::test_DataBits()
{
    QString aux;
    for (int i = 5; i<9; i++)
    {
        serialHamLibWidget->setDataBits(i);
        aux = QString("Databits %1 not OK").arg(i);
        QVERIFY2(serialHamLibWidget->getDataBits() == i, qPrintable(aux));
    }
    serialHamLibWidget->setDataBits(9);
    QVERIFY2(serialHamLibWidget->getDataBits() == 8, "Databits default not OK");
}

void tst_SetupHamLibSerialWidget::test_FlowControl()
{
    serialHamLibWidget->setFlowControl("Hardware");
    //QVERIFY2(serialHamLibWidget->getFlowControl() == "HamLibSerialFlowControl=Hardware", "FlowControl Hardware not OK");
    QVERIFY2(serialHamLibWidget->getFlowControl() == "Hardware", "FlowControl Hardware not OK");
    serialHamLibWidget->setFlowControl("Software");
    QVERIFY2(serialHamLibWidget->getFlowControl() == "Software", "FlowControl Software not OK");
    serialHamLibWidget->setFlowControl("None");
    QVERIFY2(serialHamLibWidget->getFlowControl() == "None", "FlowControl None not OK");
    serialHamLibWidget->setFlowControl("DEFAULT");
    QVERIFY2(serialHamLibWidget->getFlowControl() == "None", "FlowControl Default not OK");
}

void tst_SetupHamLibSerialWidget::test_Parity()
{
    serialHamLibWidget->setParity("None");
    QVERIFY2(serialHamLibWidget->getParity() == "None", "Parity None not OK");
    serialHamLibWidget->setParity("Even");
    QVERIFY2(serialHamLibWidget->getParity() == "Even", "Parity Even not OK");
    serialHamLibWidget->setParity("Odd");
    QVERIFY2(serialHamLibWidget->getParity() == "Odd", "Parity Odd not OK");
    serialHamLibWidget->setParity("Space");
    QVERIFY2(serialHamLibWidget->getParity() == "Space", "Parity Space not OK");
    serialHamLibWidget->setParity("Mark");
    QVERIFY2(serialHamLibWidget->getParity() == "Mark", "Parity Mark not OK");
    serialHamLibWidget->setParity("Default");
    QVERIFY2(serialHamLibWidget->getParity() == "None", "Parity Default not OK");
}

void tst_SetupHamLibSerialWidget::test_StopBits()
{
    serialHamLibWidget->setStopBits("OneStop");
    QVERIFY2(serialHamLibWidget->getStopBits() == "OneStop", "StopBits 1 not OK");
    serialHamLibWidget->setStopBits("OneAndHalfStop");
    QVERIFY2(serialHamLibWidget->getStopBits() == "OneAndHalfStop", "StopBits 1.5 not OK");
    serialHamLibWidget->setStopBits("TwoStop");
    QVERIFY2(serialHamLibWidget->getStopBits() == "TwoStop", "StopBits 2 not OK");
    serialHamLibWidget->setStopBits("AnyOtherThing");
    QVERIFY2(serialHamLibWidget->getStopBits() == "OneStop", "StopBits Default not OK");
}

void tst_SetupHamLibSerialWidget::test_SerialSpeed()
{
    QStringList strings;
    strings.clear();
    strings << "1200" << "2400" << "4800" << "9600" << "19200" << "38400" << "57600" << "115200";
    foreach(QString aux, strings)
    {
        serialHamLibWidget->setSerialBauds(aux.toInt());
        QVERIFY2(serialHamLibWidget->getSerialBauds() == aux.toInt(), qPrintable(QString("Serial Speed %1 not OK").arg(aux.toInt())));
    }
    serialHamLibWidget->setSerialBauds(1111);
    QVERIFY2(serialHamLibWidget->getSerialBauds() == 1200, qPrintable(QString("Serial Speed default not OK")));
}

void tst_SetupHamLibSerialWidget::test_SerialPort()
{}

QTEST_MAIN(tst_SetupHamLibSerialWidget)

#include "tst_setuphamlibserialwidget.moc"
