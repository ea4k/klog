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
    qDebug() << Q_FUNC_INFO;
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
    QString result;
    result.clear ();
    if ( (0 < _data.toInt()) && (_data.toInt() < CQZones+1) )
    {
        return getADIFPair("CQZ", _data);
    }
    return result;
}

QString ADIFForField::getADIFForITUz(const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
    QString result;
    result.clear ();
    if ( (0 < _data.toInt()) && (_data.toInt() < ITUZones+1) )
    {
        return getADIFPair("ITUZ", _data);
    }
    return result;
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
    float num = _data.toFloat(&ok);
    if (!ok)
        return QString();
    if (!( (num>=0.0) && (num<=120.0)  ))
        return QString();

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
        return QString();
    if (!( (num>=0.0) && (num<=400.0)  ))
        return QString();
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


QString ADIFForField::getADIFPair(const QString &_field, const QString &_data)
{
    qDebug() << Q_FUNC_INFO;
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
