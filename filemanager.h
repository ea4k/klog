#ifndef FILEMANAGER_H
#define FILEMANAGER_H

/***************************************************************************
                          filemanager.h  -  description
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

#include <QObject>
//#include <QtWidgets>

#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QSqlQuery>
#include <QStringList>
#include <QSqlRecord>
#include <QProgressDialog>
#include <QMessageBox>
#include <QDateTime>
#include "world.h"
#include "awards.h"
#include "database.h"
#include "dataproxy.h"
#include "dataproxy_sqlite.h"
#include "utilities.h"

enum
{
    CQZones = 40,
    ITUZones = 90,
    DXCCEntities = 521 // http://www.adif.org/adif302.htm#Country%20Codes
};

class FileManager : public QWidget
{
    Q_OBJECT
public:
    FileManager();
    FileManager(const QString _kontestDir);
    FileManager(const QString _kontestDir, const QString _softVersion, DataBase _db);
    ~FileManager();
    //bool readAdif(const QString& tfileName, const int logN);
    bool adifReadLog(const QString& tfileName, const int logN);
    bool adifLogExport(const QString& _fileName, const int _logN);
    bool adifLogExportMarked(const QString& _fileName);
    bool adifReqQSLExport(const QString& _fileName);
    bool cabrilloLogExport(const QString& _fileName, const int _contestType, const int logNconst);
    bool modifySetupFile(const QString& _filename, QString _field, const QString _value);


private:
    bool adifLogExportToFile(const QString& _fileName, const int _logN, bool justMarked = false, bool _qslRequested = false);
    bool cabrilloLogExportToFile(const QString& _fileName, const int logNconst);
    bool cabrilloLogExportCQWWToFile(const QString& _fileName, const int logNconst);
    //bool adifCheckMoreThanOneLog(QFile &_f);
    int howManyLogsInFile(QFile & _f);
    bool fillHashLog(QFile & _f);
    QStringList getListOfLogsInFile(QFile & _f);


    //QString checkAndFixASCIIinADIF(const QString _data);

    bool processQsoReadingADIF(const QStringList _line, const int logNumber, const bool _keepLogsInFile, QHash <int, int> &_logs);
    void queryPreparation(const int _logN);

    bool checkADIFValidFormat(const QStringList _qs);

    DataBase *db;
    //float softwareVersion;
    DataProxy *dataProxy;
    Utilities *util;

    bool rstTXDefault, rstRXDefault; // If true and a log is not including RST, 59 is automatically added



    bool printQs(const QString _q, const QStringList _line);
    //int confirmed;
    QString kontestDir;
    QString kontestVersion;
    //QProgressBar *progressBar;
    bool ignoreUnknownAlways;   // When importing ADIF, ignore all unknown fields.
    bool noMoreQso;

    World *world;
    Awards *awards;

    QSqlQuery preparedQuery;
    QHash<int, int> hashLogs;  // to create different logs when importing a ADIF file

};
#endif // FILEMANAGER_H
