/***************************************************************************
                          fileawardmanager.cpp  -  description
                             -------------------
    begin                : oct 2020
    copyright            : (C) 2020 by Jaime Robles
    user                : jaime@robles.es
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

#include "fileawardmanager.h"

FileAwardManager::FileAwardManager(DataProxy_SQLite *dp, const QString &_parentFunction)
{
    dataProxy = dp;
    util = new Utilities;
    world = new World(dataProxy, Q_FUNC_INFO);
}

bool FileAwardManager::importNewAwardFile()
{
    //qDebug() << "FileAwardManager::importNewAwardFile"  << Qt::endl;
    QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open Award file"), util->getHomeDir(), tr("Award files (*.awa)"));
    //qDebug() << "FileAwardManager::importNewAwardFile - file: " << fileName  << Qt::endl;
    QFile file( fileName );
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) /* Flawfinder: ignore */
    {
        //qDebug() << "FileAwardManager::importNewAwardFile File not found" << fileName << Qt::endl;
        showError(tr("Award file not opened"), tr("KLog was not able to read the award file"), QString(tr("It was not possible to open the file %1 for reading.") ).arg(fileName));
        return false;
    }
    else
    {
        //qDebug() << "FileAwardManager::importNewAwardFile File opened" << Qt::endl;
    }

    QString line = QString();
    int number = 0;
    bool hasEOH = false;
    // id / name / shortname / prefix / regionalgroup / regionalid / dxcc / cqz / ituz / start_date / end_date / deleted
    QString sub_name = QString();
    QString sub_shortname = QString();
    QString sub_prefix = QString();
    QString sub_regionalGroup = QString();
    int sub_regionalID = -1;
    int sub_dxcc = -1;
    int sub_cqz = -1;
    int sub_ituz = -1;
    QDate sub_startDate = QDate();
    QDate sub_endDate = QDate();
    bool sub_deleted = false;

    qint64 pos; //Position in the file
    pos = file.pos();

    while ( !file.atEnd() )
    { // KLog first read the full file, to detect potential format errors and counting the number of references
        //qDebug() << "FileAwardManager::importNewAwardFile: First read, counting and looking for errors: " << line << Qt::endl;

        line = file.readLine().trimmed().toUpper();
        //qDebug() << "FileAwardManager::importNewAwardFile: " << line << Qt::endl;
        number = number + line.count("EOR>");
        if ((line.count("<EOH>")>0) )
        {
            hasEOH = true;
        }
        //qDebug() << " //FileAwardManager::importNewAwardFile in the while, end of loop"  << Qt::endl;
    }
    //qDebug() << "FileAwardManager::importNewAwardFile: While finished "  << Qt::endl;
    if (!hasEOH)
    {
        showError(tr("AWA wrong format"), tr("The AWA file does not have the right format"), QString(tr("AWA file does not have an <EOH> field") ));
        return false;
    }

    file.seek(pos);
    bool haveAll = false;
    QStringList fields;
    QString aux; aux.clear();
    hasEOH = false;

    while ( !file.atEnd()   )
    {
        line = (file.readLine()).toUpper();
        fields.clear();
        fields << line.split("<", Qt::SkipEmptyParts);
        if (!hasEOH)
        { // Lets read the HEADER
            // IF EOH
            if (line.contains("<EOH>"))
            {
                hasEOH = true;
            }

            foreach (aux, fields)
            {
                //qDebug() << "FileAwardManager::importNewAwardFile: Reading header: " << aux << Qt::endl;
                aux = aux.trimmed();
                aux = "<" + aux;
                QStringList adifField;
                adifField.clear();
                adifField << util->getValidADIFFieldAndData(aux);
                if (adifField.length()==2)
                {
                    if (adifField.at(0) == "SUBDIV_ENTITY" )
                    {
                        sub_dxcc = world->getQRZARRLId(adifField.at(1));
                    }
                    else if (adifField.at(0) == "SUBDIV_REFERENCES")
                    {
                        //regionalAward->addRegionalAwardNumberOfReferences(adifField.at(1).toInt());
                    }
                    else if (adifField.at(0) == "SUBDIV_VERSION")
                    {
                        //regionalAward->addRegionalAwardVersion(adifField.at(1).toInt());
                    }
                    else if (adifField.at(0) == "SUBDIV_SPONSOR")
                    {
                        //regionalAward->addRegionalAwardSponsor(adifField.at(1));
                    }
                    else if (adifField.at(0) == "SUBDIV_NAME")
                    {
                        //regionalAward->addRegionalAwardName(adifField.at(1));
                    }

                    else
                    {
                        //qDebug() << "FileAwardManager::importNewAwardFile: Invalid AWA field found HEADER" << adifField.at(0) << Qt::endl;
                    }

                }
                aux.clear();
            }
        }
        else
        { // NOT in EOH
            // Read line, MODIFY AWARD
            //qDebug() << "FileAwardManager::importNewAwardFile: Parsing : " << line << Qt::endl;
            foreach (aux, fields)
            {
                aux = aux.trimmed();
                aux = "<" + aux;
                QStringList adifField;
                adifField.clear();
                adifField << util->getValidADIFFieldAndData(aux);
                //qDebug() << "FileAwardManager::importNewAwardFile: Length: " << QString::number(adifField.count()) << Qt::endl;
                if (adifField.count()==2)
                {
                    //qDebug() << "FileAwardManager::importNewAwardFile: Parsing : (" << adifField.at(0) << "/" << adifField.at(1) << ")" << Qt::endl;
                    if (adifField.at(0) == "SUBDIV_REFNUMBER" )
                    {
                        sub_regionalID = adifField.at(1).toInt();
                    }
                    if (adifField.at(0) == "SUBDIV_DXCC" )
                    {
                        sub_dxcc = adifField.at(1).toInt();
                    }
                    if (adifField.at(0) == "SUBDIV_CQ" )
                    {
                        sub_cqz = adifField.at(1).toInt();
                    }
                    if (adifField.at(0) == "SUBDIV_ITU" )
                    {
                        sub_ituz = adifField.at(1).toInt();
                    }
                    else if (adifField.at(0) == "SUBDIV_PREF")
                    {
                        sub_prefix = adifField.at(1);
                    }
                    else if (adifField.at(0) == "SUBDIV_SHORT")
                    {
                            sub_shortname = adifField.at(1);
                    }
                    else if (adifField.at(0) == "SUBDIV_NAME")
                    {
                        sub_name = adifField.at(1);
                    }
                    else if (adifField.at(0) == "SUBDIV_GROUP")
                    {
                        sub_regionalGroup = adifField.at(1);
                    }
                    else if (adifField.at(0) == "EOR")
                    {// END OF REGISTRY
                        //qDebug() << "FileAwardManager::importNewAwardFile: EOR FOUND!\nLet's add the reference." << Qt::endl;
                        QStringList _subDiv;
                        dataProxy->addDXCCEntitySubdivision(sub_name, sub_shortname, sub_prefix, sub_regionalGroup,
                                                            sub_regionalID, sub_dxcc, sub_cqz, sub_ituz, sub_startDate, sub_endDate, sub_deleted);
                        sub_name = QString();
                        sub_shortname = QString();
                        sub_prefix = QString();
                        sub_regionalGroup = QString();
                        sub_regionalID = -1;
                        sub_dxcc = -1;
                        sub_cqz = -1;
                        sub_ituz = -1;
                        sub_startDate = QDate();
                        sub_endDate = QDate();
                        sub_deleted = false;
                    }
                 else
                {
                    //qDebug() << "FileAwardManager::importNewAwardFile: Invalid AWA field found in BODY" << adifField.at(0) << Qt::endl;
                }
            }}
        }
    }

    return false;
}

void FileAwardManager::showError(const QString _header, const QString _msg, const QString _detailedMsg)
{
    //qDebug() << "FileAwardManager::showError: " << _msg << Qt::endl;
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr("KLog - %1").arg(_header));
    msgBox.setText(_msg);
    msgBox.setInformativeText(_detailedMsg);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}
