#include "updatesatsdata.h"

UpdateSatsData::UpdateSatsData(DataProxy *dp, QObject *parent) : QObject(parent)
{
    util = new Utilities;
     dataProxy = dp;
}

bool UpdateSatsData::satDataFileRead(const QString& tfileName)
{
    //qDebug() << "UpdateSatsData::satDataFileRead: " << tfileName << endl;
    QString fileName = tfileName;

    QFile file( fileName );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //qDebug() << "UpdateSatsData::satDataFileRead File not found" << fileName << endl;
        return false;
    }
    if (dataProxy->clearSatList())
    {
        //qDebug() << "UpdateSatsData::satDataFileRead Sats YES deleted"  << endl;
    }
    else
    {
        //qDebug() << "UpdateSatsData::satDataFileRead Sats NOT deleted"  << endl;
    }

    int numberOfSats = 0;
    bool hasEOH = false;
    bool inHeader = true;
    QString line = QString();
    bool noMoreRegisters = false;
    qint64 pos; //Position in the file
    bool haveId = false;
    bool haveName = false;
    bool haveUpLink = false;
    bool haveDownLink = false;
    bool haveMode = false;

    pos = file.pos();
    while ( !file.atEnd() && !hasEOH)
    {
        line = (file.readLine()).toUpper();
        numberOfSats = numberOfSats + line.count("EOR>");
        if ((line.count("<EOH>")>0) && (!hasEOH))
        {
            hasEOH = true;
            pos = file.pos();

        }
    }

    file.seek(pos);



    QProgressDialog progress(tr("Reading Satellites data file..."), tr("Abort reading"), 0, numberOfSats, 0);

    progress.setWindowModality(Qt::ApplicationModal);
    progress.setVisible(true);
    progress.setValue(0);
    progress.setMaximum(numberOfSats);


    //qDebug() << "UpdateSatsData::satDataFileRead: END OF HEADER"  << endl;


    //file.seek(pos);
    //START reading SAT data...
    //qDebug() << "UpdateSatsData::satDataFileRead: Start reading data"  << endl;

    QStringList fields, fieldToAnalyze;//, qsToPass;
    fields.clear();
    fieldToAnalyze.clear();
    QString aux = QString();
    QString field, data;
    int tagLength = 0;
    int dataLength = 0;
    QString satID = QString();
    QString satName = QString();
    QString satUpLink = QString();
    QString satDownLink = QString();
    QString satMode = QString();

/*
    while (!noMoreRegisters)
    {
        if (!file.atEnd())
        {
            line.clear();
            line.append(file.readLine().trimmed().toUpper());
            //qDebug() << "UpdateSatsData::satDataFileRead-line:" << line << endl;
            fields.clear();
            fields << line.split("<", QString::SkipEmptyParts);


        }
        else
        {
            noMoreRegisters = true;
            //qDebug() << "UpdateSatsData::satDataFileRead: While - End of File"  << endl;
        }
    }

    */

    while (!noMoreRegisters)
    {
        //qDebug() << "UpdateSatsData::satDataFileRead: While Start"  << endl;
        if (!file.atEnd())
        {
            line.clear();
            line.append(file.readLine().trimmed().toUpper());
            fields.clear();
            //qDebug() << "UpdateSatsData::satDataFileRead-line:" << line << endl;
            fields << line.split("<", QString::SkipEmptyParts);

            foreach (aux, fields)
            {
                aux = aux.simplified();
                qDebug() << "UpdateSatsData::satDataFileRead-aux:" << aux << endl;
                fieldToAnalyze = util->getValidADIFFieldAndData("<" + aux);
                if (fieldToAnalyze.size() == 2)
                {
                    field = fieldToAnalyze.at(0);
                    data = fieldToAnalyze.at(1);
                    qDebug() << "UpdateSatsData::satDataFileRead-Field:" << field << endl;
                    qDebug() << "UpdateSatsData::satDataFileRead-Data:" << data << endl;
                    if (field == "EOR")
                    {
                        qDebug() << "UpdateSatsData::satDataFileRead - EOR DETECTED!" << endl;
                        if (haveId && haveName)
                        {
                            qDebug() << "UpdateSatsData::satDataFileRead - EOR DETECTED and have it all!" << endl;
                            haveId = false;
                            haveName = false;
                            haveUpLink = false;
                            haveDownLink = false;
                            haveMode = false;
                            dataProxy->addSatellite(satID, satName, satDownLink,satUpLink, satMode);
                            qDebug() << "UpdateSatsData::satDataFileRead - Satellite added: " << satID << endl;
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
                            haveUpLink = false;
                            haveDownLink = false;
                            haveMode = false;
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
                                //qDebug() << "UpdateSatsData::satDataFileRead - Detected: " << "APP_KLOG_SATS_ARRLID" << endl;
                        }
                        else if (field == "APP_KLOG_SATS_NAME")
                        {
                            satName = data;
                            haveName = true;
                                //qDebug() << "UpdateSatsData::satDataFileRead - Detected: " << "APP_KLOG_SATS_NAME" << endl;
                        }
                        else if (field == "APP_KLOG_SATS_UPLINK")
                        {
                            satUpLink = data;
                            haveUpLink = true;
                            //qDebug() << "UpdateSatsData::satDataFileRead - Detected: " << "APP_KLOG_SATS_UPLINK" << endl;
                        }
                        else if (field == "APP_KLOG_SATS_DOWNLINK")
                        {
                            satDownLink = data;
                            haveDownLink = true;
                            //qDebug() << "UpdateSatsData::satDataFileRead - Detected: " << "APP_KLOG_SATS_DOWNLINK" << endl;
                        }
                        else if (field == "APP_KLOG_SATS_MODE")
                        {
                            satMode = data;
                            haveMode = true;
                                //qDebug() << "UpdateSatsData::satDataFileRead - Detected: " << "APP_KLOG_SATS_MODE" << endl;
                        }
                        else if (field == "APP_KLOG_DATA")
                        {
                            //qDebug() << "UpdateSatsData::satDataFileRead - Detected: " << "APP_KLOG_DATA" << endl;
                            if (data != "SATS")
                            {
                                return false;
                            }
                        }
                    }
                }
                //qDebug() << "UpdateSatsData::satDataFileRead: foreach end"  << endl;
            }
            //qDebug() << "UpdateSatsData::satDataFileRead: out of foreach"  << endl;
        }
        else
        {
            noMoreRegisters = true;
        }

        //qDebug() << "UpdateSatsData::satDataFileRead: While END"  << endl;
    }




    //qDebug() << "UpdateSatsData::satDataFileRead: END"  << endl;
    emit satsUpdatedSignal(true);
    return true;
}




bool UpdateSatsData::readSatDataFile()
{
    //qDebug() << "UpdateSatsData::readSatDataFile: " << endl;
    //QString fileName = QFileDialog::getOpenFileName(0, tr("Open File"), "/home", "Sat data (*.dat)");


    QString fileName = QFileDialog::getOpenFileName(0, tr("Open File"),
                                                     util->getHomeDir(),
                                                     tr("Sat Data") + "(*.dat)");
    if (fileName.isNull())
    {
        return false;
    }
    else
    {
        //qDebug() << "MainWindow::slotADIFImport -1" << endl;
        return satDataFileRead(fileName);
        //filemanager->adifReadLog(fileName, currentLog);
    }
    //qDebug() << "UpdateSatsData::readSatDataFile: END" << endl;
}
