/***************************************************************************
                          adif.cpp  -  description
                             -------------------
    begin                : jun 2015
    copyright            : (C) 2015 by Jaime Robles
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
//#include "global.h"
#include "edi.h"
#include "callsign.h"

EDI::EDI(const QString &_parentName)
{
    parentName = _parentName;
    //qDebug() << Q_FUNC_INFO << " (" << _parentName << ")";
    init();
}

EDI::~EDI()
{
}

void EDI::init()
{
    InitializeBandHash();
    InitializeModeHash();
}

void EDI::InitializeBandHash() {
    bandHash = {
        {"6M", "50 MHz"},
        {"4M", "70 MHz"},
        {"2M", "145 MHz"},
        {"70CM", "435 MHz"},
        {"23CM", "1,3 GHz"},
        {"13CM", "2,3 GHz"},
        {"9CM", "3,4 GHz"},
        {"6CM", "5,7 GHz"},
        {"3CM", "10 GHz"},
        {"1.25CM", "24 GHz"},
        {"6mm", "47 GHz"},
        {"4mm", "76 GHz"},
        {"2.5mm", "120 GHz"},
        {"2mm", "144 GHz"},
        {"1mm", "248 GHz"}
    };
    return;
}

void EDI::InitializeModeHash() {
    modeTXHash = {
        {0, "none"},
        {1, "SSB"},
        {2, "CW"},
        {3, "SSB"},
        {4, "CW"},
        {5, "AM"},
        {6, "FM"},
        {7, "RTTY"},
        {8, "SSTV"},
        {9, "ATV"},
    };
    modeRXHash = {
        {0, "none"},
        {1, "SSB"},
        {2, "CW"},
        {3, "CW"},
        {4, "SSB"},
        {5, "AM"},
        {6, "FM"},
        {7, "RTTY"},
        {8, "SSTV"},
        {9, "ATV"},
    };
    return;
}

void EDI::setLogLevel(DebugLogLevel _l)
{
    logLevel = _l;
}


QString EDI::getEDIield(const QString &_fieldName, const QString &_data)
{// Receives the ADIF field and the data and returns the ADIF field with a blank space at the end.
    // Check if _fieldName is a valid ADIF
    //qDebug() << Q_FUNC_INFO << " - " << _fieldName << "/" << _data;
    if ((_data.length()<=0) || (_data.isNull()))
    {
        //qDebug() << Q_FUNC_INFO << " - Not Valid";
        return QString();
    }
    if (ADIFHash.empty()) {
        InitializeHash();
    }

    QString fieldN = _fieldName.toUpper();
    if (!ADIFHash.contains(fieldN)) {
        //qDebug() << Q_FUNC_INFO << " - No valid ADIF: " << _fieldName;
        return QString();
    }
    if (fieldN == "DISTANCE" )
        if (_data.toDouble() <= 0.0)
            return QString();
    return QString ("<%1:%2>%3 ").arg(fieldN).arg(_data.length ()).arg(_data);
}

bool EDI::isValidCall(const QString &_c)
{
    Callsign _callsign(_c);
    return _callsign.isValid();
}

void EDI::logEvent(const QString &_func, const QString &_msg,  DebugLogLevel _level)
{
    emit debugLog (_func, _msg, _level);
}
