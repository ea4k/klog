/***************************************************************************
                          updatesatsdata.cpp  -  description
                             -------------------
    begin                : sep 2018
    copyright            : (C) 2018 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                                *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify           *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,                *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/

#include "updatesatsdata.h"

UpdateSatsData::UpdateSatsData(DataProxy_SQLite *dp, QObject *parent) : QObject(parent)
{
    util = new Utilities;
    dataProxy = dp;
}

UpdateSatsData::~UpdateSatsData()
{
    delete(util);
}

bool UpdateSatsData::satDataFileRead(const QString& tfileName)
{
       //qDebug() << "UpdateSatsData::satDataFileRead: " << tfileName << QT_ENDL;
    QString fileName = tfileName;
    bool errorFound = true;

    QFile file( fileName );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) /* Flawfinder: ignore */
    {
           //qDebug() << "UpdateSatsData::satDataFileRead File not found" << fileName << QT_ENDL;
        return false;
    }
    if (dataProxy->clearSatList())
    {
           //qDebug() << "UpdateSatsData::satDataFileRead Sats YES deleted"  << QT_ENDL;
    }
    else
    {
           //qDebug() << "UpdateSatsData::satDataFileRead Sats NOT deleted"  << QT_ENDL;
         return false;
    }

    int numberOfSats = 0;
    bool hasEOH = false;
    //bool inHeader = true;
    QString line = QString();
    bool noMoreRegisters = false;
    qint64 pos; //Position in the file
    bool haveId = false;
    bool haveName = false;
    //bool haveUpLink = false;
    //bool haveDownLink = false;
    //bool haveMode = false;

    pos = file.pos();
    while ( !file.atEnd() && !hasEOH)
    {
        line = (file.readLine()).toUpper();
        numberOfSats = numberOfSats + line.count("EOR>");
        if ((line.count("<EOH>")>0) && (!hasEOH))
        {
            errorFound = false;
            hasEOH = true;
            pos = file.pos();

        }
    }

    file.seek(pos);

    QProgressDialog progress(tr("Reading Satellites data file..."), tr("Abort reading"), 0, numberOfSats, nullptr);

    progress.setWindowModality(Qt::ApplicationModal);
    progress.setVisible(true);
    progress.setValue(0);
    progress.setMaximum(numberOfSats);


       //qDebug() << "UpdateSatsData::satDataFileRead: END OF HEADER"  << QT_ENDL;


    //file.seek(pos);
    //START reading SAT data...
       //qDebug() << "UpdateSatsData::satDataFileRead: Start reading data"  << QT_ENDL;

    QStringList fields, fieldToAnalyze;//, qsToPass;
    fields.clear();
    fieldToAnalyze.clear();
    QString aux = QString();
    QString field, data;
    //int tagLength = 0;
    //int dataLength = 0;
    QString satID = QString();
    QString satName = QString();
    QString satUpLink = QString();
    QString satDownLink = QString();
    QString satMode = QString();


    while (!noMoreRegisters)
    {
           //qDebug() << "UpdateSatsData::satDataFileRead: While Start"  << QT_ENDL;
        if (!file.atEnd())
        {
            line.clear();
            line.append(file.readLine().trimmed().toUpper());
            fields.clear();
               //qDebug() << "UpdateSatsData::satDataFileRead-line:" << line << QT_ENDL;
            fields << line.split("<", QT_SKIP);

            foreach (aux, fields)
            {
                aux = aux.simplified();
                  //qDebug() << "UpdateSatsData::satDataFileRead-aux:" << aux << QT_ENDL;
                fieldToAnalyze = util->getValidADIFFieldAndData("<" + aux);
                if (fieldToAnalyze.size() == 2)
                {
                    field = fieldToAnalyze.at(0);
                    data = fieldToAnalyze.at(1);
                      //qDebug() << "UpdateSatsData::satDataFileRead-Field:" << field << QT_ENDL;
                      //qDebug() << "UpdateSatsData::satDataFileRead-Data:" << data << QT_ENDL;
                    if (field == "EOR")
                    {
                          //qDebug() << "UpdateSatsData::satDataFileRead - EOR DETECTED!" << QT_ENDL;
                        if (haveId && haveName)
                        {
                              //qDebug() << "UpdateSatsData::satDataFileRead - EOR DETECTED and have it all!" << QT_ENDL;
                            haveId = false;
                            haveName = false;
                            //haveUpLink = false;
                            //haveDownLink = false;
                            //haveMode = false;
                            if (!dataProxy->addSatellite(satID, satName, satDownLink,satUpLink, satMode))
                            {
                                //errorFound = true;
                                return false;
                            }
                              //qDebug() << "UpdateSatsData::satDataFileRead - Satellite added: " << satID << QT_ENDL;
                            satID = QString();
                            satName = QString();
                            satUpLink = QString();
                            satDownLink = QString();
                            satMode = QString();
                        }
                        else
                        {
                            haveId = false;
                            haveName = false;
                            //haveUpLink = false;
                            //haveDownLink = false;
                            //haveMode = false;
                            satID = QString();
                            satName = QString();
                            satUpLink = QString();
                            satDownLink = QString();
                            satMode = QString();
                        }
                    }
                    else
                    {
                        if (field == "APP_KLOG_SATS_ARRLID")
                        {
                            satID = data;
                            haveId = true;
                                   //qDebug() << "UpdateSatsData::satDataFileRead - Detected: " << "APP_KLOG_SATS_ARRLID" << QT_ENDL;
                        }
                        else if (field == "APP_KLOG_SATS_NAME")
                        {
                            satName = data;
                            haveName = true;
                                   //qDebug() << "UpdateSatsData::satDataFileRead - Detected: " << "APP_KLOG_SATS_NAME" << QT_ENDL;
                        }
                        else if (field == "APP_KLOG_SATS_UPLINK")
                        {
                            satUpLink = data;
                            //haveUpLink = true;
                               //qDebug() << "UpdateSatsData::satDataFileRead - Detected: " << "APP_KLOG_SATS_UPLINK" << QT_ENDL;
                        }
                        else if (field == "APP_KLOG_SATS_DOWNLINK")
                        {
                            satDownLink = data;
                            //haveDownLink = true;
                               //qDebug() << "UpdateSatsData::satDataFileRead - Detected: " << "APP_KLOG_SATS_DOWNLINK" << QT_ENDL;
                        }
                        else if (field == "APP_KLOG_SATS_MODE")
                        {
                            satMode = data;
                            //haveMode = true;
                                   //qDebug() << "UpdateSatsData::satDataFileRead - Detected: " << "APP_KLOG_SATS_MODE" << QT_ENDL;
                        }
                        else if (field == "APP_KLOG_DATA")
                        {
                               //qDebug() << "UpdateSatsData::satDataFileRead - Detected: " << "APP_KLOG_DATA" << QT_ENDL;
                            if (data != "SATS")
                            {
                                return false;
                            }
                        }
                    }
                }
                   //qDebug() << "UpdateSatsData::satDataFileRead: foreach end"  << QT_ENDL;
            }
               //qDebug() << "UpdateSatsData::satDataFileRead: out of foreach"  << QT_ENDL;
        }
        else
        {
            noMoreRegisters = true;
        }

           //qDebug() << "UpdateSatsData::satDataFileRead: While END"  << QT_ENDL;
    }


    if (errorFound)
    {
          //qDebug() << "UpdateSatsData::satDataFileRead: errorFound = true"  << QT_ENDL;
        return false;
    }
    else
    {
         //qDebug() << "UpdateSatsData::satDataFileRead: END"  << QT_ENDL;
       emit satsUpdatedSignal(true);
       QMessageBox msgBox;
       msgBox.setIcon(QMessageBox::Information);
       msgBox.setText(tr("The Satellites information has been updated."));
       msgBox.exec();
    }


      //qDebug() << "UpdateSatsData::satDataFileRead: END "  << QT_ENDL;
    return true;
}


bool UpdateSatsData::readSatDataFile()
{
       //qDebug() << "UpdateSatsData::readSatDataFile: " << QT_ENDL;
    //QString fileName = QFileDialog::getOpenFileName(0, tr("Open File"), "/home", "Sat data (*.dat)");


    QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open File"),
                                                     util->getHomeDir(),
                                                     tr("Sat Data") + "(*.dat)");
    if (fileName.isNull())
    {
        return false;
    }
    else
    {
           //qDebug() << "MainWindow::slotADIFImport -1" << QT_ENDL;
        return satDataFileRead(fileName);
        //filemanager->adifReadLog(fileName, currentLog);
    }
       //qDebug() << "UpdateSatsData::readSatDataFile: END" << QT_ENDL;
}
