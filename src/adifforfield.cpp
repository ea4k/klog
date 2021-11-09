/***************************************************************************
                          adifforfield.cpp  -  description
                             -------------------
    begin                : oct 2021
    copyright            : (C) 2021 by Jaime Robles
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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/

#include "adifforfield.h"

ADIFForField::ADIFForField()
{
    util = new Utilities;
}

ADIFForField::~ADIFForField() {}

QString ADIFForField::getADIFForQSODate(const QString &_data, ExportMode _em)
{
    qDebug() << Q_FUNC_INFO;
    if (_data.length ()<1)
        return QString();
    QString aux, result;
    result.clear ();
    QDateTime tDateTime;
    tDateTime = util->getDateTimeFromSQLiteString(_data);

    if (tDateTime.isValid())
    {
        aux = util->getADIFDateFromQDateTime(tDateTime);
        result = getADIFPair("QSO_DATE", aux);
        aux = util->getADIFTimeFromQDateTime(tDateTime);
        if (_em == ModeEQSL)
        {
            aux.chop(2);
        }
        //qDebug() << Q_FUNC_INFO << ": " << result;
        result = result + getADIFPair("TIME_ON", aux);
    }
    //qDebug() << Q_FUNC_INFO << ": " << result;
    return result;
}

QString ADIFForField::getADIFForCall(const QString &_data)
{
    qDebug() << Q_FUNC_INFO << ": " << _data;
    QString result;
    result.clear ();
    if (util->isValidCall(_data))
    {
        result = getADIFPair("CALL", _data);
    }
    else
    {
        if (showInvalidCallMessage(_data))
        {
             result = getADIFPair("CALL", _data);
        }
    }
    return result;
}

QString ADIFForField::getADIFForRSTSent(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    return getADIFPair("RST_SENT", _data);
}

QString ADIFForField::getADIFForRSTRcvd(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    return getADIFPair("RST_RCVD", _data);
}

QString ADIFForField::getADIFForBand(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    return getADIFPair("BAND", _data);
}

QString ADIFForField::getADIFForMode(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    //if ((aux.length()>1) && (dataProxy->getIdFromModeName(aux)>=0))
    return getADIFPair("MODE", _data);
}

QString ADIFForField::getADIFForSubMode(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    return getADIFPair("SUBMODE", _data);
}

QString ADIFForField::getADIFForCQz(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;

    bool ok;
    int i = _data.toInt(&ok);
    if (!ok)
    {
        return QString();
    }
    if (!util->isValidCQ (i))
    {
        return QString();
    }
    return getADIFPair("CQZ", _data);
}

QString ADIFForField::getADIFForITUz(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    bool ok;
    int i = _data.toInt(&ok);
    if (!ok)
    {
        return QString();
    }
    if (!util->isValidITU (i))
    {
        return QString();
    }
    return getADIFPair("ITUZ", _data);
}

QString ADIFForField::getADIFForDXCC(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    QString result;
    result.clear ();
    if ( (0 < _data.toInt()) && (_data.toInt() < DXCCEntities+1) )
    {
        return getADIFPair("DXCC", _data);
    }
    return result;
}

QString ADIFForField::getADIFForAddress(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    return getADIFPair("ADDRESS", _data);
}

QString ADIFForField::getADIFForAge(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    bool ok;
    int i = _data.toInt(&ok);
    if (!ok)
    {
        return QString();
    }
    if (!util->isValidAge (i))
    {
        return QString();
    }
    return getADIFPair("AGE", _data);
}

QString ADIFForField::getADIFForCNTY(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    return getADIFPair("CNTY", _data);
}

QString ADIFForField::getADIFForComment(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    return getADIFPair("COMMENT", _data);
}

QString ADIFForField::getADIFForA_Index(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    bool ok;
    float num = _data.toFloat(&ok);
    if (!ok)
    {
        return QString();
    }
    if (!( (num>=0.0) && (num<=400.0)  ))
    {
        return QString();
    }
    return getADIFPair("A_INDEX", _data);
}

QString ADIFForField::getADIFForAnt_az(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    //TODO: Normalize to 0-360
    bool ok;
    float num = _data.toFloat(&ok);
    if (!ok)
        return QString();
    return getADIFPair("ANT_AZ", QString::number(num));
}

QString ADIFForField::getADIFForAnt_el(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    //TODO: Normalize to -90 - 90
    bool ok;
    float num = _data.toFloat(&ok);
    if (!ok)
        return QString();
    if (!((-90.0 <= num) && (90.0>=num)))
        return QString();
    return getADIFPair("ANT_EL", _data);
}

QString ADIFForField::getADIFForAnt_path(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    QStringList validData;
    validData.clear ();
    validData << "G" << "O" << "S" << "L";
    if (!validData.contains (_data))
         return QString();
    return getADIFPair("ANT_PATH", _data);
}

QString ADIFForField::getADIFForARRL_sect(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    //TODO: Ensure the exported data is a valid ARRL SECT
    return getADIFPair("ARRL_SECT", _data);
}

QString ADIFForField::getADIFForGridSquare(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    //TODO: Ensure the exported data is a valid ARRL SECT
    if (!util->isValidGrid (_data))
        return QString();
    return getADIFPair("GRIDSQUARE", _data);
}
QString ADIFForField::getADIFForMyGridSquare(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    //TODO: Ensure the exported data is a valid ARRL SECT
    if (!util->isValidGrid (_data))
        return QString();
    return getADIFPair("MY_GRIDSQUARE", _data);
}

QString ADIFForField::getADIFForQSODateOff(const QString &_data, ExportMode _em)
{
    qDebug() << Q_FUNC_INFO;
    if (_data.length ()<1)
        return QString();
    QString aux, result;
    result.clear ();
    QDateTime tDateTime;
    tDateTime = util->getDateTimeFromSQLiteString(_data);
    if (tDateTime.isValid())
    {
        aux = util->getADIFDateFromQDateTime(tDateTime);
        result = getADIFPair("QSO_DATE_OFF", aux);
        aux = util->getADIFTimeFromQDateTime(tDateTime);
        if (_em == ModeEQSL)
        {
            aux.chop(2);
        }
        //qDebug() << Q_FUNC_INFO << ": " << result;
        result = result + getADIFPair("TIME_OFF", aux);
    }
    //qDebug() << Q_FUNC_INFO << ": " << result;
    return result;
}

QString ADIFForField::getADIFForFreq(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    //TODO: Normalize to 0-360
    bool ok;
    float num = _data.toFloat(&ok);
    if (!ok)
        return QString();
    if (num<0)
    {
        return QString();
    }
    return getADIFPair("FREQ", QString::number(num));
}

QString ADIFForField::getADIFForStationCallsign(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    QString result;
    result.clear ();
    if (util->isValidCall(_data))
    {
        result = getADIFPair("STATION_CALLSIGN", _data);
    }
    else
    {
        if (showInvalidCallMessage(_data))
        {
             result = getADIFPair("STATION_CALLSIGN", _data);
        }
    }
    return result;
}

QString ADIFForField::getADIFPair(const QString &_field, const QString &_data)
{
    qDebug() << Q_FUNC_INFO << ": " << _field << "/" << _data;
    int length = _data.length ();
    if (length<1)
    {
        return QString();
    }
    if (_field.length ()<1)
    {
        return QString();
    }
    return QString("<%1:%2>%3 ").arg(_field).arg(length).arg(_data);
}

bool ADIFForField::showInvalidCallMessage(const QString &_call){
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr("KLog - Invalid call detected"));

    QString aux = _call;

    if (aux.length()<1)
    {
        aux = QString(tr("An empty callsign has been detected. Do you want to export this QSO anyway (click on Yes) or remove the field from the exported log file?"));
    }
    else
    {
        aux = QString(tr("An invalid callsign has been detected %1. Do you want to export this callsign anyway (click on Yes) or remove the call from the exported log file?").arg(aux));
    }
    msgBox.setText(aux);
    msgBox.setInformativeText(tr("Exporting wrong calls may create problems in the applications you are potentially importing this logfile to. It may, however, be a good callsign that is wrongly identified by KLog as not valid. You can, however, edit the ADIF file once the export process is finished."));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Yes:
          // Yes was clicked
           return true;
    case QMessageBox::No:
            // No Save was clicked
        return false;
    default:
            // should never be reached
        return false;
    }
}

QString ADIFForField::getADIFForBandRX(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    return getADIFPair("BAND_RX", _data);
}

QString ADIFForField::getADIFForFreq_rx(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    //TODO: Normalize to 0-360
    bool ok;
    float num = _data.toFloat(&ok);
    if (!ok)
        return QString();
    if (num<0)
    {
        return QString();
    }
    return getADIFPair("FREQ_RX", QString::number(num));
}

QString ADIFForField::getADIFForQSLRDate(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    if (_data.length ()<1)
        return QString();

    QString aux, result;
    result.clear ();
    QDate tDate;
    tDate = util->getDateFromSQliteString(_data);

    if (tDate.isValid())
    {
        aux = util->getADIFDateFromQDate (tDate);
        result = getADIFPair("QSLRDATE", aux);
      }
    //qDebug() << Q_FUNC_INFO << ": " << result;
    return result;
}

QString ADIFForField::getADIFForQSLSDate(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    if (_data.length ()<1)
        return QString();

    QString aux, result;
    result.clear ();
    QDate tDate;
    tDate = util->getDateFromSQliteString(_data);

    if (tDate.isValid())
    {
        aux = util->getADIFDateFromQDate (tDate);
        result = getADIFPair("QSLSDATE", aux);
      }
    //qDebug() << Q_FUNC_INFO << ": " << result;
    return result;
}

QString ADIFForField::getADIFForQSLRcvd(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    if (!util->isValidQSL_Rcvd (_data, false))
         return QString();
    return getADIFPair("QSL_RCVD", _data);
}

QString ADIFForField::getADIFForQSLSent(const QString &_data)
{
    if (!util->isValidQSL_Sent (_data))
         return QString();
    return getADIFPair("QSL_SENT", _data);
}

QString ADIFForField::getADIFForLoTWRDate(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    if (_data.length ()<1)
        return QString();

    QString aux, result;
    result.clear ();
    QDate tDate;
    tDate = util->getDateFromSQliteString(_data);

    if (tDate.isValid())
    {
        aux = util->getADIFDateFromQDate (tDate);
        result = getADIFPair("LOTW_QSLRDATE", aux);
      }
    return result;
}

QString ADIFForField::getADIFForLoTWSDate(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    if (_data.length ()<1)
        return QString();

    QString aux, result;
    result.clear ();
    QDate tDate;
    tDate = util->getDateFromSQliteString(_data);

    if (tDate.isValid())
    {
        aux = util->getADIFDateFromQDate (tDate);
        result = getADIFPair("LOTW_QSLSDATE", aux);
      }
    return result;
}

QString ADIFForField::getADIFForLoTWQSLRcvd(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    if (!util->isValidQSL_Rcvd (_data, false))
         return QString();
    return getADIFPair("LOTW_QSL_RCVD", _data);
}

QString ADIFForField::getADIFForLoTWQSLSent(const QString &_data)
{
    if (!util->isValidQSL_Sent (_data))
         return QString();
    return getADIFPair("LOTW_QSL_SENT", _data);
}

QString ADIFForField::getADIFForClubLogQSOUploadDate(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    if (_data.length ()<1)
        return QString();

    QString aux, result;
    result.clear ();
    QDate tDate;
    tDate = util->getDateFromSQliteString(_data);

    if (tDate.isValid())
    {
        aux = util->getADIFDateFromQDate (tDate);
        result = getADIFPair("CLUBLOG_QSO_UPLOAD_DATE", aux);
      }
    return result;
}

QString ADIFForField::getADIFForClubLogQSOUploadStatus(const QString &_data)
{
    if (!util->isValidUpload_Status (_data))
         return QString();
    return getADIFPair("CLUBLOG_QSO_UPLOAD_STATUS", _data);
}

QString ADIFForField::getADIFForHRDLogQSOUploadDate(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    if (_data.length ()<1)
        return QString();

    QString aux, result;
    result.clear ();
    QDate tDate;
    tDate = util->getDateFromSQliteString(_data);

    if (tDate.isValid())
    {
        aux = util->getADIFDateFromQDate (tDate);
        result = getADIFPair("HRDLOG_QSO_UPLOAD_DATE", aux);
      }
    return result;
}

QString ADIFForField::getADIFForHRDLogQSOUploadStatus(const QString &_data)
{
    if (!util->isValidUpload_Status (_data))
         return QString();
    return getADIFPair("HRDLOG_QSO_UPLOAD_STATUS", _data);
}

QString ADIFForField::getADIFForQRZCOMQSOUploadDate(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    if (_data.length ()<1)
        return QString();

    QString aux, result;
    result.clear ();
    QDate tDate;
    tDate = util->getDateFromSQliteString(_data);

    if (tDate.isValid())
    {
        aux = util->getADIFDateFromQDate (tDate);
        result = getADIFPair("QRZCOM_QSO_UPLOAD_DATE", aux);
      }
    return result;
}

QString ADIFForField::getADIFForQRZCOMQSOUploadStatus(const QString &_data)
{
    if (!util->isValidUpload_Status (_data))
         return QString();
    return getADIFPair("QRZCOM_QSO_UPLOAD_STATUS", _data);
}

QString ADIFForField::getADIFForAward_Submitted(const QString &_data)
{
    if (_data.length ()<1)
         return QString();
    return getADIFPair("AWARD_SUBMITTED", _data);
}

QString ADIFForField::getADIFForAward_Granted(const QString &_data)
{
    if (_data.length ()<1)
         return QString();
    return getADIFPair("AWARD_GRANTED", _data);
}

QString ADIFForField::getADIFForEQSL_QSLRDate(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    if (_data.length ()<1)
        return QString();

    QString aux, result;
    result.clear ();
    QDate tDate;
    tDate = util->getDateFromSQliteString(_data);

    if (tDate.isValid())
    {
        aux = util->getADIFDateFromQDate (tDate);
        result = getADIFPair("EQSL_QSLRDATE", aux);
      }
    return result;
}

QString ADIFForField::getADIFForEQSL_QSLSDate(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    if (_data.length ()<1)
        return QString();

    QString aux, result;
    result.clear ();
    QDate tDate;
    tDate = util->getDateFromSQliteString(_data);

    if (tDate.isValid())
    {
        aux = util->getADIFDateFromQDate (tDate);
        result = getADIFPair("EQSL_QSLSDATE", aux);
      }
    return result;
}

QString ADIFForField::getADIFForEQSL_QSLRCVD(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    if (!util->isValidQSL_Rcvd (_data, false))
         return QString();
    return getADIFPair("EQSL_QSL_RCVD", _data);
}

QString ADIFForField::getADIFForEQSL_QSLSent(const QString &_data)
{
    if (!util->isValidQSL_Sent (_data))
         return QString();
    return getADIFPair("EQSL_QSL_SENT", _data);
}

QString ADIFForField::getADIFForContestId(const QString &_data)
{
    if (_data.length ()<1)
         return QString();
    return getADIFPair("CONTEST_ID", _data);
}

QString ADIFForField::getADIFForName(const QString &_data)
{
    if (_data.length ()<1)
         return QString();
    return getADIFPair("NAME", _data);
}

QString ADIFForField::getADIFForOperator(const QString &_data)
{
    if (_data.length ()<1)
         return QString();
    return getADIFPair("OPERATOR", _data);
}

QString ADIFForField::getADIFForPfx(const QString &_data)
{
    if (_data.length ()<1)
         return QString();
    return getADIFPair("PFX", _data);
}

QString ADIFForField::getADIFForCont(const QString &_data)
{
    if (_data.length ()<1)
         return QString();
    return getADIFPair("CONT", _data);
}

QString ADIFForField::getADIFForSRx_String(const QString &_data)
{
    if (_data.length ()<1)
         return QString();
    return getADIFPair("SRX_STRING", _data);
}

QString ADIFForField::getADIFForSTx_String(const QString &_data)
{
    if (_data.length ()<1)
         return QString();
    return getADIFPair("STX_STRING", _data);
}

QString ADIFForField::getADIFForSRx(const QString &_data)
{
    bool ok;
    _data.toInt(&ok);
    if (!ok)
         return QString();
    return getADIFPair("SRX", _data);
}

QString ADIFForField::getADIFForSTx(const QString &_data)
{
    bool ok;
    int i = _data.toInt(&ok);
    if (!ok)
         return QString();
    return getADIFPair("STX", QString::number(i));
}

QString ADIFForField::getADIFForQTH(const QString &_data)
{
    if (_data.length ()<1)
         return QString();
    return getADIFPair("QTH", _data);
}

QString ADIFForField::getADIFForRX_Pwr(const QString &_data)
{
    bool ok;
    int i = _data.toInt(&ok);
    if (!ok)
         return QString();
    return getADIFPair("RX_PWR", QString::number(i));
}

QString ADIFForField::getADIFForTX_Pwr(const QString &_data)
{
    bool ok;
    int i = _data.toInt(&ok);
    if (!ok)
         return QString();
    return getADIFPair("TX_PWR", QString::number(i));;
}

QString ADIFForField::getADIFForMy_CQz(const QString &_data)
{
    bool ok;
    int i = _data.toInt(&ok);
    if (!ok)
         return QString();
    if (!util->isValidCQ (i))
        return QString();

    return getADIFPair("MY_CQ_ZONE", QString::number(i));
}

QString ADIFForField::getADIFForMy_Ituz(const QString &_data)
{
    bool ok;
    int i = _data.toInt(&ok);
    if (!ok)
         return QString();
    if (!util->isValidITU (i))
        return QString();
    return getADIFPair("MY_ITU_ZONE", _data);
}

QString ADIFForField::getADIFForPrecedence(const QString &_data)
{
    if (_data.length ()<1)
         return QString();
    return getADIFPair("PRECEDENCE", _data);
}

QString ADIFForField::getADIFForPoints(const QString &_data)
{
    bool ok;
    int i = _data.toInt(&ok);
    if (!ok)
         return QString();
    if (i>0)
        return QString();
    return getADIFPair("APP_KLOG_POINTS", _data);
}
