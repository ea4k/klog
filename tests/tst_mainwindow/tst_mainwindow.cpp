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

// http://blog.davidecoppola.com/2018/01/gui-unit-testing-with-qt-test-introduction/
// http://blog.davidecoppola.com/2018/01/gui-unit-testing-with-qt-test-advanced/
class tst_MainWindow : public QObject
{
    Q_OBJECT

public:
    tst_MainWindow();
    ~tst_MainWindow();

private slots:
    //void initTestCase();
    //void cleanupTestCase();
    void init();
    void test_Constructor();
    void test_focusOrder();
    void test_QSOEntry();
    void test_Settings();

private:
    MainWindow *mainWindow;
    //QWidget *testCandidate;

public slots:
    void slotTimeOut();
};

tst_MainWindow::tst_MainWindow()
{
    //MainWindow mw(klogDir, version);
    Utilities util(Q_FUNC_INFO);
    mainWindow = new MainWindow(util.getHomeDir(), "2.2");
}

tst_MainWindow::~tst_MainWindow(){}

void tst_MainWindow::init()
{
    mainWindow->init();
}

void tst_MainWindow::test_focusOrder()
{
    QApplication::setActiveWindow(mainWindow);
    mainWindow->mainQSOEntryWidget->setFocus();
    QVERIFY2(mainWindow->mainQSOEntryWidget->hasFocus(), "mainQSOEntriWidget didn't get focus");
}

void tst_MainWindow::test_Constructor()
{
    qDebug() << Q_FUNC_INFO;
    QVERIFY2(mainWindow->showKLogLogWidget, "showKLogLogWidget not created");
    QVERIFY2(mainWindow->showErrorDialog, "showErrorDialog not created");
    QVERIFY2(mainWindow->UDPLogServer, "UDPLogServer not created");
    QVERIFY2(mainWindow->util, "util not created");
    QVERIFY2(mainWindow->qso, "qso not created");

    QVERIFY2(mainWindow->showErrorDialog, "showErrorDialog not created");
    QVERIFY2(mainWindow->lotwUtilities, "lotwUtilities not created");
    QVERIFY2(mainWindow->eqslUtilities, "eqslUtilities not created");
    QVERIFY2(mainWindow->elogQRZcom, "elogQRZcom not created");
    QVERIFY2(mainWindow->elogClublog, "elogClublog not created");
    QVERIFY2(mainWindow->downloadcty, "downloadcty not created");
    QVERIFY2(mainWindow->world, "world not created");
    QVERIFY2(mainWindow->locator, "locator not created");
    QVERIFY2(mainWindow->qso, "qso not created");
    //QVERIFY2(mainWindow->debugFile, "debugFile not created");
    QVERIFY2(mainWindow->dateTime, "dateTime not created");
    QVERIFY2(mainWindow->dateTimeTemp, "dateTimeTemp not created");
    QVERIFY2(mainWindow->awards, "awards not created");
    QVERIFY2(mainWindow->softUpdate, "softUpdate not created");
    QVERIFY2(mainWindow->filemanager, "filemanager not created");
    QVERIFY2(mainWindow->fileAwardManager, "fileAwardManager not created");
    QVERIFY2(mainWindow->qso, "qso not created");
    //QTimer::singleShot(500, this, SLOT(slotTimeOut()));
    qDebug() << Q_FUNC_INFO << " - Init";

    mainWindow->init();
    qDebug() << Q_FUNC_INFO << " - Show";
    mainWindow->show();
    qDebug() << Q_FUNC_INFO << " - SlotSetup";
    mainWindow->slotSetup();
    qDebug() << Q_FUNC_INFO << " - After the SlotSetup";

    //QTest::keyClick(&mainWindow, Qt::Key_Tab);
    // QTest::keyClick(toplevelWidget, Qt::Key_Space); // To close the showWar button
    //qDebug() << Q_FUNC_INFO << "To be shown";

    //qDebug() << Q_FUNC_INFO << "To be showed";
    //QTest::mouseClick(toplevelWidget. , Qt::LeftButton);

    qDebug() << Q_FUNC_INFO << " - END";
}

void tst_MainWindow::test_Settings()
{
    //qDebug() << Q_FUNC_INFO;
/*
     QTestEventList events;
    events.addKeyClick('a');
    events.addKeyClick(Qt::Key_Backspace);
    events.addDelay(200);
    QLineEdit *lineEdit = new QLineEdit(myParent);
    // ...
    events.simulate(lineEdit);
    events.simulate(lineEdit);
*/
    mainWindow->show();
    mainWindow->slotSetup();
    //qDebug() << Q_FUNC_INFO << " - END";
}


void tst_MainWindow::slotTimeOut()
{
    //qDebug() << Q_FUNC_INFO;
    QWidget *modalWidget = QApplication::activeModalWidget();
    if (modalWidget->inherits("QMessageBox"))
    {
        //qDebug() << Q_FUNC_INFO << " Inherits QmessageBox";
                QMessageBox *mb = qobject_cast<QMessageBox *>(modalWidget);
                //qDebug() << mb->text ();
                //QString msg = mb->informativeText ();
                QString msg = mb->text ();
                QVERIFY2(msg.contains ("Ukraine"), "Show War Message not created");
                QTest::keyClick(mb, Qt::Key_Enter);
    }
    //qDebug() << Q_FUNC_INFO << " - END";
}

void tst_MainWindow::test_QSOEntry()
{
    //qDebug() << Q_FUNC_INFO;
    mainWindow->show();
    //mainWindow->mainQSOEntryWidget->setQRZ ("EA4K");
    //QVERIFY2(mainWindow->mainQSOEntryWidget->getQrz () == "EA4K", "Callsign tested wrong");
    //qDebug() << "Prefix: " << mainWindow->othersTabWidget->getEntityPrefix ();
    //qDebug() << "Label: " << mainWindow->infoLabel2->text ();
    //QVERIFY2(mainWindow->othersTabWidget->getEntityPrefix () == "EA", "Prefix not properly idenfified");
    //QVERIFY2(mainWindow->infoLabel2->text () == "Spain", "Entity not properly idenfified");
    //QVERIFY2(mainWindow->infoLabel2->text () == "Spadin", "Entity2 not properly idenfified");

/*
 QSignalSpy spy(myCustomObject, SIGNAL(mySignal(int,QString,double)));

 myCustomObject->doSomething(); // trigger emission of the signal

 QList<QVariant> arguments = spy.takeFirst();
 QVERIFY(arguments.at(0).type() == QVariant::Int);
 QVERIFY(arguments.at(1).type() == QVariant::String);
 QVERIFY(arguments.at(2).type() == QVariant::double);

*/
    MainQSOEntryWidget *mainQSOEntryWidget = mainWindow->mainQSOEntryWidget;

    QWidget *testWidget;
    testWidget = mainQSOEntryWidget->manualModeCheckBox;
    QSignalSpy spy(testWidget, SIGNAL(clicked()));
    QSignalSpy spy1(mainQSOEntryWidget, SIGNAL(manualModeSignal(bool)));
    QTest::mouseClick(testWidget, Qt::LeftButton);
    // check the signal was indeed emitted, one time
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy1.count(), 1);

    QList<QVariant> args = spy1.takeFirst();
    QVERIFY(args.at(0).type() == QVariant::Bool);
    if (mainQSOEntryWidget->getManualMode() == true)
    {
        QVERIFY2(args.at(0).toBool () == true, "Manual mode  has been enabled");
    }
    else
    {
        QVERIFY2(args.at(0).toBool () == false, "Manual mode  has been disabled");
    }




    /*
    QTest::keyClick(testWidget, Qt::Key_E);
    QTest::keyClick(testWidget, Qt::Key_A);
    QTest::keyClick(testWidget, Qt::Key_4);
    QTest::keyClick(testWidget, Qt::Key_K);

    //qDebug() << "Signal count: " << QString::number(spy.count ());
    //qDebug() << Q_FUNC_INFO << " - reading signal";
    QList<QVariant> arguments = spy.takeFirst();
    //qDebug() << Q_FUNC_INFO << " - launching signal-1";
    //qDebug() << "Signal: " << arguments.at(0).toString ();
    //qDebug() << Q_FUNC_INFO << " - launching signal-2";
    QVERIFY2(arguments.at(0).type() == QVariant::String, "currentQRZSignal wrong type");
    //qDebug() << Q_FUNC_INFO << " - launching signal-3";
    QVERIFY2(arguments.at(0).toString ()== QString("EA4K"), "currentQRZSignal wrong type");
    //qDebug() << Q_FUNC_INFO << " - launching signal-4";
    */
    //QVERIFY(arguments.at(1).type() == QVariant::String);
    //QVERIFY(arguments.at(2).type() == QVariant::double);

    //QVERIFY2(mainWindow->mainQSOEntryWidget->getQrz () == "EA4K", "Callsign tested wrong");
    //qDebug() << Q_FUNC_INFO << " - END";
}

QTEST_MAIN(tst_MainWindow)
#include "tst_mainwindow.moc"
