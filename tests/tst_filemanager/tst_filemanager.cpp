/***************************************************************************
                          tst_filemanager.h  -  description
                             -------------------
    begin                : Aug 2025
    copyright            : (C) 2025 by Jaime Robles
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
#include "../../src/world.h"
#include "../../src/awards.h"
#include "../../src/callsign.h"
#include "../../src/database.h"
#include "../../src/dataproxy_sqlite.h"
#include "../../src/filemanager.h"
#include "../../src/utilities.h"
#include "../../src/qso.h"

class tst_FileManager : public QObject
{
    Q_OBJECT

public:
    tst_FileManager();
    ~tst_FileManager();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_Constructor();
    void test_ADIF_Import();


private:
    void createADIFFile();
    void deleteADIFFile();
    DataProxy_SQLite *dataProxy;
    FileManager *fileManager;
    DataBase *db;
    QString version;
};

tst_FileManager::tst_FileManager()
{
    //qDebug() << Q_FUNC_INFO << "Start";
    version = "1.5";
    //qDebug() << Q_FUNC_INFO << "001";
    Utilities util(Q_FUNC_INFO);
/*
    db = new DataBase(Q_FUNC_INFO, version, util.getKLogDBFile());
    //qDebug() << Q_FUNC_INFO << "- 003";
    QCOMPARE(db->createConnection(Q_FUNC_INFO), true);

    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO, version);
    QStringList newLogq;
    newLogq << "2025-02-01" << "EA4K" << "EA4K" << "Test-Log" << "1" << "0" ;

    if (dataProxy->addNewLog(newLogq))
    {
       //qDebug() << Q_FUNC_INFO << " - New log created";
    }
    else
    {
        QFAIL("Could not create a log");
    }

*/
    dataProxy = new DataProxy_SQLite(Q_FUNC_INFO, version);
    fileManager = new FileManager(dataProxy);
    fileManager->init();

    //qDebug() << Q_FUNC_INFO << "END";
}

tst_FileManager::~tst_FileManager(){}

void tst_FileManager::initTestCase()
{
    createADIFFile();
}

void tst_FileManager::cleanupTestCase(){}

void tst_FileManager::test_Constructor()
{
    //QVERIFY(util->getVersion() == "0.0");
    //qDebug() << "Testing the constructor" ;
}

void tst_FileManager::createADIFFile()
{ // Creates an ADIF file
   //qDebug() << Q_FUNC_INFO;
    QFile file("test-adif.adi");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) /* Flawfinder: ignore */
        QFAIL("Could not create the file");

    QTextStream out(&file);
    out << "ADIF v3.1.0 Export from KLog\n";
    out << "https://www.klog.xyz/klog\n";
    out << "<PROGRAMVERSION:5>2.3.2\n";
    out << "<PROGRAMID:4>KLOG <APP_KLOG_QSOS:5>40228<APP_KLOG_LOG_DATE_EXPORT:13>20231210-1534\n<EOH>\n";
    out << "<CALL:4>EA4KV <QSO_DATE:8>20240828 <TIME_ON:6>014744 <FREQ:6>7.1971 <BAND:3>40M <BAND_RX:3>40M <MODE:3>SSB <STATION_CALLSIGN:4>EA4K <SRX:3>203 <STX:1>1 <CQZ:2>15 <ITUZ:2>28 <DXCC:3>281 <COMMENT:10>CQ-WPX-SSB <CONT:2>EU <CONTEST_ID:10>CQ-WPX-SSB <APP_KLOG_POINTS:1>2 <CLUBLOG_QSO_UPLOAD_DATE:8>20201019 <CLUBLOG_QSO_UPLOAD_STATUS:1>M <OPERATOR:5>EA4TV <RST_SENT:2>59 <RST_RCVD:2>59 <TX_PWR:3>900 <APP_KLOG_LOGN:1>1 <EOR>";
    file.close();
}

void tst_FileManager::test_ADIF_Import()
{
    createADIFFile();
    //int errorCode = fileManager->adifReadLog("test-adif.adi", "EA4K", 1);
    //qDebug() << Q_FUNC_INFO << ": " << errorCode;
    //QVERIFY2( errorCode == 1, "Import did not returned the right number of QSO");
    //int FileManager::adifReadLog(const QString& tfileName, QString _stationCallsign, int logN)
}

void tst_FileManager::deleteADIFFile()
{
   //qDebug() << Q_FUNC_INFO;
    QFile file("test-adif.adi");
    //QVERIFY2(file.remove() == true, "test-adif-file could not be deleted");
}


QTEST_MAIN(tst_FileManager)

#include "tst_filemanager.moc"
