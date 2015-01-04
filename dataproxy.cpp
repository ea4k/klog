#include "dataproxy.h"


DataProxy::DataProxy()
{
   // qDebug() << "DataProxy::DataProxy"  << endl;
}


DataProxy::~DataProxy()
{
}

void DataProxy::createLogModel(){}
void DataProxy::createLogPanel(){}

int DataProxy::getIdFromModeName(const QString& _modeName)
{
    return -1;
}

int DataProxy::getIdFromBandName(const QString& _bandName)
{
    return -1;
}

QString DataProxy::getNameFromBandId (const int _id)
{
    return "";
}

QString DataProxy::getNameFromModeId (const int _id)
{
    return "";
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





