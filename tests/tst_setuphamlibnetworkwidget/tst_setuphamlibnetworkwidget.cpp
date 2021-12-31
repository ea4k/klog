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
#include "../../src/setuppages/hamlibnetworkconfigwidget.h"

class tst_SetupHamLibNetworkWidget: public QObject
{
    Q_OBJECT

public:
    tst_SetupHamLibNetworkWidget();
    ~tst_SetupHamLibNetworkWidget();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_Constructor();
    void test_Address();
    void test_Port();

private:
    HamLibNetworkConfigWidget *netHamLibWidget;

};

tst_SetupHamLibNetworkWidget::tst_SetupHamLibNetworkWidget()
{
    netHamLibWidget = new HamLibNetworkConfigWidget();
    netHamLibWidget->createUI();
}

tst_SetupHamLibNetworkWidget::~tst_SetupHamLibNetworkWidget()
{

}

void tst_SetupHamLibNetworkWidget::initTestCase()
{

}

void tst_SetupHamLibNetworkWidget::cleanupTestCase()
{

}

void tst_SetupHamLibNetworkWidget::test_Constructor()
{

}

void tst_SetupHamLibNetworkWidget::test_Address()
{
    netHamLibWidget->setAddress("10.0.0.1");
    QVERIFY2(netHamLibWidget->getAddress() == QString("10.0.0.1"), "Network Address not properly saved");
}

void tst_SetupHamLibNetworkWidget::test_Port()
{
    netHamLibWidget->setPort(1);
    QVERIFY2(netHamLibWidget->getPort() == 1, "Port 1 not OK");

    netHamLibWidget->setPort(0);
    QVERIFY2(netHamLibWidget->getPort() == 0, "Port out of range (0) not OK");

    netHamLibWidget->setPort(10);
    QVERIFY2(netHamLibWidget->getPort() == 10, "Port 10 not OK");

    netHamLibWidget->setPort(1000);
    QVERIFY2(netHamLibWidget->getPort() == 1000, "Port 1000 not OK");

    netHamLibWidget->setPort(70000);
    QVERIFY2(netHamLibWidget->getPort() == 1000, "Port out of range (70000) not OK");
}

QTEST_MAIN(tst_SetupHamLibNetworkWidget)

#include "tst_setuphamlibnetworkwidget.moc"
