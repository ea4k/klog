/***************************************************************************
                          tst_mainwindow.h  -  description
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
#include <QtWidgets>
#include <QPrinter>
#include <QPrintDialog>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtAlgorithms>
#include <QDesktopServices>
#include <QUrl>

#include "../../src/aboutdialog.h"
#include "../../src/tipsdialog.h"
#include "../../src/filemanager.h"
#include "../../src/fileawardmanager.h"
#include "../../src/locator.h"
#include "../../src/dxcluster.h"
#include "../../src/awards.h"
#include "../../src/awarddxmarathon.h"
#include "../../src/inputwidgets/mainwindowsattab.h"
#include "../../src/inputwidgets/mainwindowmydatatab.h"
#include "../../src/inputwidgets/mainwindowinputcomment.h"
#include "../../src/inputwidgets/mainwindowinputothers.h"
#include "../../src/inputwidgets/mainwindowinputeqsl.h"
#include "../../src/inputwidgets/mainwindowinputqsl.h"
#include "../../src/inputwidgets/mainwindowinputqso.h"
#include "../../src/mainqsoentrywidget.h"
#include "../../src/elogclublog.h"
#include "../../src/downloadcty.h"
#include "../../src/dxccstatuswidget.h"
#include "../../src/awardswidget.h"
#include "../../src/softwareupdate.h"
#include "../../src/logmodel.h"
#include "../../src/logwindow.h"
#include "../../src/searchwidget.h"
#include "../../src/infowidget.h"
#include "../../src/showerrordialog.h"
#include "../../src/udpserver.h"
#include "../../src/statisticswidget.h"
#include "../../src/updatesatsdata.h"
#include "../../src/hamlibclass.h"
#include "../../src/elogqrzlog.h"
#include "../../src/lotwutilities.h"
#include "../../src/eqslutilities.h"
#include "../../src/widgets/adiflotwexportwidget.h"
#include "../../src/widgets/showadifimportwidget.h"
#include "../../src/widgets/map/mapwindowwidget.h"
#include "../../src/widgets/showkloglogwidget.h"
#include "../../src/mainwindow.h"
#include "../../src/dataproxy_sqlite.h"
#include "../../src/database.h"
#include "../../src/klogdefinitions.h"
#include "../../src/setupdialog.h"
#include "../../src/setuppages/setuppageuserdata.h"
#include "../../src/setuppages/setuppagemisc.h"
#include "../../src/utilities.h"
#include "../../src/qso.h"
#include "../../src/world.h"



class tst_MainWindow : public QObject
{
    Q_OBJECT

public:
    tst_MainWindow();
    ~tst_MainWindow();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_Constructor();

private:
    MainWindow *mainWindow;

};

tst_MainWindow::tst_MainWindow()
{
    //MainWindow mw(klogDir, version);
    Utilities util;
    mainWindow = new MainWindow(util.getHomeDir(), "2.2");
}

tst_MainWindow::~tst_MainWindow()
{

}

void tst_MainWindow::initTestCase()
{

}

void tst_MainWindow::cleanupTestCase()
{

}

void tst_MainWindow::test_Constructor()
{

}

QTEST_MAIN(tst_MainWindow)

#include "tst_mainwindow.moc"
