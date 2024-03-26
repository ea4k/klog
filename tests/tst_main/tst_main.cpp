/***************************************************************************
                          tst_main.h  -  description
                             -------------------
    begin                : Mar 2024
    copyright            : (C) 2024 by Jaime Robles
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
#include <QtWidgets>

#include "../../src/main.cpp"
#include "../../src/inputwidgets/mainwindowsattab.h"
//#include "../../src/startwizard.h"

//#include "../../src/utilities.h"

class tst_Main : public QObject
{
    Q_OBJECT

private:
    bool myCondition()
    {
        return true;
    }

private slots:
    void initTestCase()
    {
        qDebug("Called before everything else.");
    }

    void myFirstTest()
    {
        QVERIFY(true); // check that a condition is satisfied
        QCOMPARE(1, 1); // compare two values
    }

    void mySecondTest()
    {
        QVERIFY(myCondition());
        QVERIFY(1 != 2);
    }

    void cleanupTestCase()
    {
        qDebug("Called after myFirstTest and mySecondTest.");
    }
};

