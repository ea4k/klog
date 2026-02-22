/***************************************************************************
                          tst_mainwindowsattab.h  -  description
                             -------------------
    begin                : Feb 2021
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
#include "../../src/awards.h"
#include "../../src/callsign.h"
//#include "../../src/database/db_adif_primary_subdvisions_data.h"
#include "../../src/dataproxy_sqlite.h"
#include "../../src/awarddxmarathon.h"
#include "../../src/logwindow.h"
#include "../../src/logmodel.h"
#include "../../src/database/database.h"
#include "../../src/utilities.h"
#include "../../src/qso.h"
#include "../../src/qsodatacache.h"


// Test class for LogWindow
class tst_LogWindow : public QObject
{
    Q_OBJECT

private slots:
    // Basic test cases
    void initTestCase();        // Called before the first test function
    void cleanupTestCase();     // Called after the last test function
    void testSetColumns();      // Test for setColumns
    void testRestoreColumnOrder(); // Test for restoreColumnOrder

private:
    LogWindow *logWindow;       // Instance of LogWindow to test
    Awards *awards;         // Mock Awards object
    DataProxy_SQLite *dataProxy;
    World *world;
};

// Initializes resources before tests
void tst_LogWindow::initTestCase()
{
    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO);
    world = new World(dataProxy, Q_FUNC_INFO);
    awards = new Awards(dataProxy, world, nullptr); // Mock Awards object
    logWindow = new LogWindow(awards);     // Initialize LogWindow
}

// Cleans up resources after tests
void tst_LogWindow::cleanupTestCase()
{
    delete logWindow;
    //delete awards;
}

// Test for setColumns
void tst_LogWindow::testSetColumns()
{
    QStringList columns;
    columns << "qso_date" << "call" << "rst_sent" << "rst_rcvd" << "bandid" << "modeid" << "comment";
    //columns << "Date" << "Time" << "Call" << "Band";

    logWindow->setColumns(columns);

    // Verify the columns are set correctly
    QCOMPARE(logWindow->columns, columns);
}

// Test for restoreColumnOrder
void tst_LogWindow::testRestoreColumnOrder()
{
    // Mock some column headers in QSettings

}

// Register the test class
QTEST_MAIN(tst_LogWindow)
#include "tst_logwindow.moc"
