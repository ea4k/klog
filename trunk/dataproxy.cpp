/***************************************************************************
                          dataproxy.cpp  -  description
                             -------------------
    begin                : sept 2014
    copyright            : (C) 2014 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                             *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify         *
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


#include "dataproxy.h"


DataProxy::DataProxy()
{
   //qDebug() << "DataProxy::DataProxy"  << endl;
}


DataProxy::~DataProxy()
{
}

void DataProxy::createLogModel(){}
void DataProxy::createLogPanel(){}
bool DataProxy::haveAtLeastOneLog(){return true;}

int DataProxy::getIdFromModeName(const QString& _modeName)
{
    return -1;
}

int DataProxy::getIdFromBandName(const QString& _bandName)
{
    return -1;
}

int DataProxy::getSubModeIdFromSubMode(const QString _subModeName)
{
    return -1;
}

int DataProxy::getModeIdFromSubModeId(const int _sm)
{
    return -1;
}

QString DataProxy::getModeFromSubMode (const QString _sm)
{
    return "";
}

bool DataProxy::isModeDeprecated (const QString _sm)
{
    return false;
}

QString DataProxy::getNameFromBandId (const int _id)
{
    return "";
}

QString DataProxy::getNameFromModeId (const int _id)
{
    return "";
}

QString DataProxy::getNameFromSubModeId (const int _id)
{
    return "";
}

QString DataProxy::getSubModeFromId (const int _id)
{
    return QString();
}

QString DataProxy::getNameFromSubMode (const QString _sm)
{
    return "";
}


double DataProxy::getFreqFromBandId(const int _id)
{
    return -1.0;
}


int DataProxy::getBandIdFromFreq(const double _n)
{
    return -1;
}

QStringList DataProxy::getBands()
{
    return QStringList();
}

QStringList DataProxy::getModes()
{
    return QStringList();
}

QStringList DataProxy::getBandsInLog(const int _log)
{
    return QStringList();
}

QStringList DataProxy::getModesInLog(const int _log)
{
    return QStringList();
}

int DataProxy::getLastQSOid()
{
    return -1;

}

bool DataProxy::clearLog()
{
   return false;

}

bool DataProxy::qslSentViaDirect(const int _qsoId, const QString _updateDate)
{
    return false;
}


bool DataProxy::qslSentViaBureau(const int _qsoId, const QString _updateDate)
{
    return false;
}

bool DataProxy::qslRecViaBureau(const int _qsoId, const QString _updateDate)
{
    return false;
}

bool DataProxy::qslRecViaBureau(const int _qsoId, const QString _updateDate, const bool _queueSentQSL)
{
    return false;
}

bool DataProxy::qslRecViaDirect(const int _qsoId, const QString _updateDate, const bool _queueSentQSL)
{
    return false;
}

bool DataProxy::qslRecViaDirect(const int _qsoId, const QString _updateDate)
{
    return false;
}

bool DataProxy::qslSentAsRequested(const int _qsoId, const QString _updateDate)
{
    return false;
}

bool DataProxy::qslRecAsRequested(const int _qsoId, const QString _updateDate)
{
    return false;
}

bool DataProxy::setClubLogSent(const int _qsoId, const QString _st, const QString _updateDate)
{
    return false;
}

bool DataProxy::isQSLReceived(const int _qsoId)
{
    return false;
}

bool DataProxy::isQSLSent(const int _qsoId)
{
    return false;
}

QString DataProxy::getCallFromId(const int _qsoId)
{
    return "";
}

QStringList DataProxy::getClubLogRealTimeFromId(const int _qsoId)
{
    return QStringList();
}


bool DataProxy::deleteQSO(const int _qsoId)
{
    return false;
}

int DataProxy::isWorkedB4(const QString _qrz, const int _currentLog)
{
    return -1;
}

bool DataProxy::isDXCCConfirmed(const int _dxcc, const int _currentLog)
{
    return false;
}

bool DataProxy::isHF(const int _band)
{
    return false;
}

bool DataProxy::isWARC(const int _band)
{
    return false;
}

bool DataProxy::isVHF(const int _band)
{
    return false;
}

QStringList DataProxy::getOperatingYears(const int _currentLog)
{
    return QStringList();
}

void DataProxy::compressDB()
{

}

int DataProxy::getDXCConYear(const int _year, const int _logNumber)
{
    return -1;
}
int DataProxy::getCQzonYear(const int _year, const int _logNumber)
{
    return -1;
}

bool DataProxy::newDXMarathon(const int _dxcc, const int _cq, const int _year, const int _logNumber)
{
    return false;
}

QStringList DataProxy::getContestNames()
{
    return QStringList();
}

QStringList DataProxy::getContestCat(const int _catn){
    return QStringList();
}

QStringList DataProxy::getContestOverlays()
{
    return QStringList();
}

QStringList DataProxy::getBandNames(){
    return QStringList();
}

QStringList DataProxy::getPropModeList()
{
    return QStringList();
}

QStringList DataProxy::getValidCatOptions(const int _currentCat, const int _lowerCa)
{
    return QStringList();
}

int DataProxy::getNumberOfManagedLogs()
{
    return -1;
}

QString DataProxy::getStationCallSignFromLog(const int _log)
{
    return QString();
}

int DataProxy::getContestTypeN(const int _co, const int _catop, const int _catas, const int _catpo, const int _catba, const int _catov, const int _catmo)
{
    return -1;
}
QStringList DataProxy::getDataFromContestType(const int _n)
{
    return QStringList();
}

int DataProxy::getHowManyQSOInLog(const int _log)
{
    return 0;
}
