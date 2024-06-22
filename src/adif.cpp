/***************************************************************************
                          utilities.cpp  -  description
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
#include "adif.h"

Adif::Adif(const QString &_parentName)
{
    parentName = _parentName;
    //qDebug() << Q_FUNC_INFO << " (" << _parentName << ")";
    //init();
}

Adif::~Adif()
{
}

void Adif::init()
{
    setARRLSect();
    setContinents();
    setSponsorsList ();
}

void Adif::InitializeHash() {
    ADIFHash = {
        {"ADDRESS", "MultiLineString"},
        {"AGE", "Number"},
        {"ALTITUDE", "Number"},
        {"ANT_AZ", "Number"},
        {"ANT_EL", "Number"},
        {"ANT_PATH", "Enumeration"},
        {"ARRL_SECT", "Enumeration"},
        {"AWARD_SUBMITTED", "SponsoredAwardList"},
        {"AWARD_GRANTED", "SponsoredAwardList"},
        {"A_INDEX", "Number"},
        {"BAND", "Enumeration"},
        {"BAND_RX", "Enumeration"},
        {"CALL", "String"},
        {"CHECK", "String"},
        {"CLASS", "String"},
        {"CLUBLOG_QSO_UPLOAD_DATE", "Date"},
        {"CLUBLOG_QSO_UPLOAD_STATUS", "Enumeration"},
        {"CNTY", "Enumeration"},
        {"COMMENT", "String"},
        {"CONT", "Enumeration"},
        {"CONTACTED_OP", "String"},
        {"CONTEST_ID", "String"},
        {"COUNTRY", "String"},
        {"CQZ", "PositiveInteger"},
        {"CREDIT_SUBMITTED", "CreditList"},
        {"CREDIT_GRANTED", "CreditList"},
        {"DARC_DOK", "Enumeration"},
        {"DISTANCE", "Number"},
        {"DXCC", "Enumeration"},
        {"EMAIL", "String"},
        {"EQ_CALL", "String"},
        {"EQSL_QSLRDATE", "Date"},
        {"EQSL_QSLSDATE", "Date"},
        {"EQSL_QSL_RCVD", "Enumeration"},
        {"EQSL_QSL_SENT", "Enumeration"},
        {"FISTS", "PositiveInteger"},
        {"FISTS_CC", "PositiveInteger"},
        {"FORCE_INIT", "Boolean"},
        {"FREQ", "Number"},
        {"FREQ_RX", "Number"},
        {"GRIDSQUARE", "GridSquare"},
        {"GRIDSQUARE_EXT", "GridSquareExt"},
        {"GUEST_OP", "String"},
        {"HAMLOGEU_QSO_UPLOAD_DATE", "Date"},
        {"HAMLOGEU_UPLOAD_STATUS", "Enumeration"},
        {"HAMQTH_QSO_UPLOAD_DATE", "Date"},
        {"HAMQTH_UPLOAD_STATUS", "Enumeration"},
        {"HRDLOG_QSO_UPLOAD_DATE", "Date"},
        {"HRDLOG_QSO_UPLOAD_STATUS", "Enumeration"},
        {"IOTA", "IOTARefNo"},
        {"IOTA_ISLAND_ID", "PositiveInteger"},
        {"ITUZ", "PositiveInteger"},
        {"K_INDEX", "Integer"},
        {"LAT", "Location"},
        {"LON", "Location"},
        {"LOTW_QSLRDATE", "Date"},
        {"LOTW_QSLSDATE", "Date"},
        {"LOTW_QSL_RCVD", "Enumeration"},
        {"LOTW_QSL_SENT", "Enumeration"},
        {"MAX_BURSTS", "Number"},
        {"MODE", "Enumeration"},
        {"MS_SHOWER", "String"},
        {"MY_ALTITUDE", "Number"},
        {"MY_ANTENNA", "String"},
        {"MY_ARRL_SECT", "Enumeration"},
        {"MY_CITY", "String"},
        {"MY_CNTY", "Enumeration"},
        {"MY_COUNTRY", "String"},
        {"MY_CQ_ZONE", "PositiveInteger"},
        {"MY_DXCC", "Enumeration"},
        {"MY_FISTS", "PositiveInteger"},
        {"MY_GRIDSQUARE", "GridSquare"},
        {"MY_GRIDSQUARE_EXT", "GridSquareExt"},
        {"MY_IOTA", "IOTARefNo"},
        {"MY_IOTA_ISLAND_ID", "PositiveInteger"},
        {"MY_ITU_ZONE", "PositiveInteger"},
        {"MY_LAT", "Location"},
        {"MY_LON", "Location"},
        {"MY_NAME", "String"},
        {"MY_POSTAL_CODE", "String"},
        {"MY_POTA_REF", "POTARef"},
        {"MY_RIG", "String"},
        {"MY_SIG", "String"},
        {"MY_SIG_INFO", "String"},
        {"MY_SOTA_REF", "SOTARef"},
        {"MY_STATE", "Enumeration"},
        {"MY_STREET", "String"},
        {"MY_USACA_COUNTIES", "SecondarySubdivisionList"},
        {"MY_VUCC_GRIDS", "GridSquareList"},
        {"MY_WWFF_REF", "WWFFRef"},
        {"NAME", "String"},
        {"NOTES", "MultilineString"},
        {"NR_BURSTS", "Integer"},
        {"NR_PINGS", "Integer"},
        {"OPERATOR", "String"},
        {"OWNER_CALLSIGN", "String"},
        {"PFX", "String"},
        {"POTA_REF", "POTARef"},
        {"PRECEDENCE", "String"},
        {"PROP_MODE", "Enumeration"},
        {"PUBLIC_KEY", "String"},
        {"QRZCOM_QSO_UPLOAD_DATE", "Date"},
        {"QRZCOM_QSO_UPLOAD_STATUS", "Enumeration"},
        {"QSLMSG", "MultilineString"},
        {"QSLRDATE", "Date"},
        {"QSLSDATE", "Date"},
        {"QSL_RCVD", "Enumeration"},
        {"QSL_RCVD_VIA", "Enumeration"},
        {"QSL_SENT", "Enumeration"},
        {"QSL_SENT_VIA", "Enumeration"},
        {"QSL_VIA", "String"},
        {"QSO_COMPLETE", "Enumeration"},
        {"QSO_DATE", "Date"},
        {"QSO_DATE_OFF", "Date"},
        {"QSO_RANDOM", "Boolean"},
        {"QTH", "String"},
        {"REGION", "Enumeration"},
        {"RIG", "MultilineString"},
        {"RST_RCVD", "String"},
        {"RST_SENT", "String"},
        {"RX_PWR", "Number"},
        {"SAT_MODE", "String"},
        {"SAT_NAME", "String"},
        {"SFI", "Integer"},
        {"SIG", "String"},
        {"SIG_INFO", "String"},
        {"SILENT_KEY", "Boolean"},
        {"SKCC", "String"},
        {"SOTA_REF", "SOTARef"},
        {"SRX", "Integer"},
        {"SRX_STRING", "String"},
        {"STATE", "Enumeration"},
        {"STATION_CALLSIGN", "String"},
        {"STX", "Integer"},
        {"STX_STRING", "String"},
        {"SUBMODE", "String"},
        {"SWL", "Boolean"},
        {"TEN_TEN", "PositiveInteger"},
        {"TIME_OFF", "Time"},
        {"TIME_ON", "Time"},
        {"TX_PWR", "Number"},
        {"UKSMG", "PositiveInteger"},
        {"USACA_COUNTIES", "SecondarySubdivisionList"},
        {"VE_PROV", "String"},
        {"VUCC_GRIDS", "GridSquareList"},
        {"WEB", "String"},
        {"WWFF_REF", "WWFFRef"},
        {"APP_LOTW_RXQSL", "Date"},
        {"APP_LOTW_RXQSO", "Date"},
        {"APP_LOTW_QSO_TIMESTAMP", "Date"}
    };
    return;
}

void Adif::setLogLevel(DebugLogLevel _l)
{
    logLevel = _l;
}

void Adif::setARRLSect()
{
    QStringList pARRL_sects = {"AL", "AK", "AB", "AR", "AZ", "BC", "CO", "CT", "DE", "EB", "EMA", "ENY", "EPA", "EWA"
        "GA", "GTA", "ID", "IL", "IN", "IA", "KS", "KY", "LAX", "LA",
        "ME", "MB", "MAR", "MDC", "MI", "MN", "MS", "MO", "MT",
        "NE", "NV", "NH", "NM", "NLI", "NL", "NC", "ND", "NTX",
        "NFL", "NNJ", "NNY", "NT", "NWT", "OH", "OK", "ON",
        "ONE", "ONN", "ONS", "ORG", "OR", "PAC", "PE", "PR", "QC",
        "RI", "SV", "SDG", "SF", "SJV", "SB", "SCV", "SK", "SC", "SD",
        "STX", "SFL", "SNJ", "TN", "VI", "UT", "VT", "VA",
        "WCF", "WTX", "WV", "WMA", "WNY", "WPA", "WWA", "WI", "WY"};
    ARRL_sects = pARRL_sects;
}

void Adif::setContinents()
{
    QStringList pcontinents = {"NA", "SA", "EU", "AF", "OC", "AS", "AN"};
    continents = pcontinents;
}

void Adif::setSponsorsList()
{
    QStringList psponsorsList = {"ADIF_", "ARI_", "ARRL_", "CQ_", "DARC_", "EQSL_", "IARU_", "JARL_", "RSGB_", "TAG_", "WABAG_"};
    sponsorsList = psponsorsList;
}

bool Adif::isValidFreq(const QString &_b)
{
    //qDebug() << Q_FUNC_INFO << ": freq: " << _b;
    bool _ok;
    double f = _b.toDouble(&_ok);
    if ((_ok) && f>0)
    {
        //qDebug() << Q_FUNC_INFO << ": Freq OK" ;
        return true;
    }
   /* if (!_ok)
        //qDebug() << Q_FUNC_INFO << ": Freq NOK: NOK" ;
    if (f<=0)
        //qDebug() << Q_FUNC_INFO << ": Freq NOK: f=0" ;
    else
        //qDebug() << Q_FUNC_INFO << ": Freq NOK: NOT-KNOWN" ;*/
    return false;
}

bool Adif::isValidPower(const QString &_b)
{
    return (_b.toDouble()>0);
}

QString Adif::getADIFBoolFromBool(const bool _b)
{// Will produce the ADIF format if a bool is received
    if (_b)
        return "Y";
    return "N";
}


bool Adif::isValidK_Index(const QString &_b)
{
    return ((_b.toInt()>=0) && (_b.toInt()<=9));
}

bool Adif::isValidCQz(const QString &_b)
{
   return ((_b.toInt()>=1) && (_b.toInt()<=40));
}

bool Adif::isValidITUz(const QString &_b)
{
    return ((_b.toInt()>=1) && (_b.toInt()<=90));
}

bool Adif::isValidDXCC(const QString &_b)
{
    return ((_b.toInt()>=0) && (_b.toInt()<=522));
}

bool Adif::isValidAge(const QString &_b)
{
    return ((_b.toInt()>=0) && (_b.toInt()<=120));
}

bool Adif::isValidAnt_EL(const QString &_b)
{
    return ((_b.toInt()>=-90) && (_b.toInt()<=900));
}

bool Adif::isValidAnt_AZ(const QString &_b)
{
    return ((_b.toInt()>=0) && (_b.toInt()<=360));
}

bool Adif::isValidA_Index(const QString &_b)
{
    return ((_b.toInt()>=0) && (_b.toInt()<=400));
}

bool Adif::isValidDistance(const QString &_b)
{
    return (_b.toDouble()>0);
}

bool Adif::isValidIOTA_islandID(const QString &_b)
{
    return ((_b.toInt()>=1) && (_b.toInt()<=90));
}

bool Adif::isValidNRBursts(const QString &_b)
{
    return (_b.toInt()>0);
}

bool Adif::isValidPings(const QString &_b)
{
    return (_b.toInt()>0);
}

bool Adif::isValidSFI(const QString &_b)
{
    return ((_b.toInt()>=0) && (_b.toInt()<=300));
}

bool Adif::isValidSTX(const QString &_b)
{
    return (_b.toInt()>=0);
}

bool Adif::isValidSRX(const QString &_b)
{
    return (_b.toInt()>=0);
}

bool Adif::isValidFISTS(const QString &_b)
{
   return (_b.toInt()>0);
}

bool Adif::isValidUKSMG(const QString &_b)
{
    return (_b.toInt()>0);
}

bool Adif::isValidTenTen(const QString &_b)
{
    return (_b.toInt()>0);
}

bool Adif::isValidLogId(const QString &_b)
{
    return (_b.toInt()>0);
}

bool Adif::isValidAntPath(const QString &_s)
{
    return ((_s == "G") || (_s == "O") || (_s == "S") || (_s == "L"));
}

bool Adif::isValidQSO_COMPLETE(const QString &_s)
{    // "Y", "N", "NIL", "?"
    return ((_s == "Y") || (_s == "N") || (_s == "NIL") || (_s == "?"));
}

bool Adif::isValidPOTA(const QString &_s)
{
    if (_s.length()<1)
        return false;
    return true;
}

bool Adif::isValidWWFF_Ref(const QString &_s)
{
    if (_s.length()<1)
        return false;
    return true;
}

/*
bool Utilities::isValidComment(const QString &_b)
{
    return (_b.length()>0);
}

bool Utilities::isValidName(const QString &_b)
{
    return (_b.length()>0);
}

bool Utilities::isValidADIFField(const QString &_b)
{
       //qDebug() << "Utilities::isValidADIFField: " << _b ;

    //    This functions checks if the ADIF field has the proper format.
    //    <Field:length:Data type>Data


    if (!((_b.startsWith('<')) &&  (_b.count('>')) == 1 ))
    {
           //qDebug() << "Utilities::isValidADIFField: BAD FORMAT: No < or > delimiters: " << _b ;
        return false;
    }
    if (_b.simplified() == "<EOR>")
    {
        return true;
    }

    QStringList validDataTypes = {"B", "N", "D", "T", "S", "I", "M", "G", "E", "L"};
    QStringList qs;
    qs.clear();
    qs.append(_b.split('>'));

    if (qs.size()!= 2)
    {
           //qDebug() << "Utilities::isValidADIFField-0 (not two): " << QString::number(qs.size()) ;
        return false;
    }

    QString field = (qs.at(0)).right((qs.at(0)).length() - 1);
    QString data = (qs.at(1)).simplified();
    //data = data.simplified();
    QString dataType = QString();

      //qDebug() << "Utilities::isValidADIFField-Field: " << field ;
      //qDebug() << "Utilities::isValidADIFField_Data: " << data ;

    int length = data.length();
    int separatorPosition = 0;
    int i = (field).count(":"); //Check how many ":" do we have, to see if we have a data type or not

    if (i == 2) // We have data type
    { // DATE:8:D / 20141020
        separatorPosition = (field.section(':', 1, 1)).toInt();
        dataType = field.section(':', 2, 2);
        if (!validDataTypes.contains(dataType.toUpper()))
        {
               //qDebug() << "Utilities::isValidADIFField - FORMAT ERROR: Wrong data type: " << dataType ;
            return false;
        }
    }
    else if (i == 1)
    { // DATE:8 / 20141020
        separatorPosition = (field.section(':', 1, 1)).toInt();
    }
    else
    {
           //qDebug() << "Utilities::isValidADIFField - FORMAT ERROR, more than 2 \":\" - " << field ;
        return false;
    }

    if ( length != separatorPosition)
    {
           //qDebug() << "Utilities::isValidADIFField: Data Length problem: " << (field) << "/" << data << " - " << QString::number(length) << "/" << QString::number(separatorPosition) ;
        return false;
    }

    if (separatorPosition <= 0)
    {
        //qDebug() << "Utilities::isValidADIFField: Length problem <= 0" ;
        return false;
    }
       //qDebug() << "FileManager::checkADIFValidFormat: Return true" ;
    return true;
}

bool Utilities::isValidQSL_Rcvd(const QString &c)
{
    return ((c == "Y") || (c == "N") || (c == "R") || (c == "I") || (c == "V"));
}

bool Utilities::isValidQSL_Sent(const QString &c)
{
    return ((c == "Y") || (c == "N") || (c == "R") || (c == "Q") || (c == "I"));
}

bool Utilities::isValidUpload_Status(const QString &c)
{
    return ((c == "Y") || (c == "N") || (c == "M"));
}


bool Utilities::isValidFISTS(const QString &c)
{
    return (c.toInt ()>0);
}

QStringList Utilities::getValidADIFFieldAndData(const QString &_b)
{
   //qDebug() << "Utilities::getValidADIFFieldAndData: " << _b ;

       // This functions checks if the ADIF field has the proper format.
       // <Field:length:Data type>Data

    QStringList result;
    result.clear();

    if (!(_b.startsWith('<')))
    {
        //qDebug() << "Utilities::getValidADIFFieldAndData: BAD FORMAT: No < or > delimiters: " << _b ;
        return QStringList();
    }
    if (_b.simplified() == "<EOR>")
    {
        //qDebug() << "Utilities::getValidADIFFieldAndData: EOR" ;
        result << "EOR" << "EOR";
        return result;
    }
    QString aux = _b;
    QStringList qs;
    qs.clear();

    if ((aux.contains("APP_LOTW_")) && aux.contains("//"))
    { // Trying to fix a LoTW ADIF bug
        qs.append(aux.split("//"));
        aux = qs.at(0);
    }

    //qDebug() << "Utilities::getValidADIFFieldAndData: -20" ;
    QStringList validDataTypes = {"B", "N", "D", "T", "S", "I", "M", "G", "E", "L"};

    qs.clear();
    qs.append(aux.split('>'));

    if (qs.size()!= 2)
    {
        //qDebug() << "Utilities::getValidADIFFieldAndData-0 (not two): " << QString::number(qs.size()) ;
        return result;
    }
    //qDebug() << "Utilities::getValidADIFFieldAndData: -30" ;
    //QString field = (qs.at(0)).right((qs.at(0)).length() - 1);
    QString field = (qs.at(0)).right((qs.at(0)).length() - 1);
    QString data = (qs.at(1)).simplified();
    //data = data.simplified();
    QString dataType = QString();

    //qDebug() << "Utilities::getValidADIFFieldAndData-Field: " << field ;
    //qDebug() << "Utilities::getValidADIFFieldAndData_Data: " << data ;

    int length = data.length();
    int separatorPosition = 0;
    int i = (field).count(":"); //Check how many ":" do we have, to see if we have a data type or not

    if (i == 2) // We have data type
    { // DATE:8:D / 20141020
        separatorPosition = (field.section(':', 1, 1)).toInt();
        dataType = field.section(':', 2, 2);
          //qDebug() << "Utilities::getValidADIFFieldAndData - DataType: -" << dataType << "-" ;
        if (!validDataTypes.contains(dataType.toUpper()))
        {
              //qDebug() << "Utilities::getValidADIFFieldAndData - FORMAT ERROR: Wrong data type: " << dataType ;
            return result;
        }
    }
    else if (i == 1)
    { // DATE:8 / 20141020
        separatorPosition = (field.section(':', 1, 1)).toInt();
    }
    else
    {
        //qDebug() << "Utilities::getValidADIFFieldAndData - FORMAT ERROR, more than 2 \":\" - " << field ;
        return result;
    }
    //qDebug() << "Utilities::getValidADIFFieldAndData: -60" ;
    if ( length != separatorPosition)
    {
        //qDebug() << "Utilities::getValidADIFFieldAndData: Data Length problem: " << (field) << "/" << data << " - " << QString::number(length) << "/" << QString::number(separatorPosition) ;
        return result;
    }

    if (separatorPosition <= 0)
    {
        //qDebug() << "Utilities::getValidADIFFieldAndData: Length problem <= 0" ;
        return result;
    }
     //qDebug() << "Utilities::getValidADIFFieldAndData: -90: f: " << field ;
      //qDebug() << "Utilities::getValidADIFFieldAndData: -90: d: " << data;ield = field.section(':', 0, 0);
    result.clear();
    result << field.section(':', 0, 0) << data;
     //qDebug() << "Utilities::checkADIFValidFormat: Return true: " << result.at(0) << "/" << result.at(1) ;
    return result;
}

QString Utilities::getADIFField(const QString &_fieldName, const QString &_data)
{// Receives the ADIF field and the data and returns the ADIF field with a blank space at the end.
    // Check if _fieldName is a valid ADIF
    //
    if (ADIFHash.empty()) {
        InitializeHash();
    }
    if (!ADIFHash.contains(_fieldName)) {
        //qDebug() << Q_FUNC_INFO << " - No valid ADIF: " << _fieldName;
        return QString();
    }
    return QString ("<%1:%2>%3 ").arg(_fieldName).arg(_data.length ()).arg(_data);
}

bool Utilities::isValidARRLSect(const QString &_s)
{
    return (ARRL_sects.contains (_s.toUpper ()));
}


bool Utilities::isValidContinent(const QString &_s)
{
    return (continent.contains (_s.toUpper ()));
}

bool Utilities::isValidPropMode(const QString &_s)
{
    QStringList propModes;
    propModes.clear ();
    propModes << "AS" << "AUE" << "AUR" << "BS" << "ECH" << "EME" << "ES"
                << "F2" << "FAI" << "GWAVE" << "INTERNET" << "ION" << "IRL"
                << "LOS" << "MS" << "RPT" << "RS" << "SAT" << "TEP" << "TR";
    return propModes.contains (_s.toUpper ());
}

bool Utilities::isValidDistance(const double _d)
{
    return (_d>=0);
}

bool Utilities::isValidSponsor(const QString &_s)
{
    return (sponsorsList.contains (_s.toUpper ()));
}
*/

QString Adif::getADIFField(const QString &_fieldName, const QString &_data)
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
    return QString ("<%1:%2>%3 ").arg(fieldN).arg(_data.length ()).arg(_data);
}

bool Adif::isValidCall(const QString &_c)
{
    if (_c.length ()>=3)
        return true;
    return false;
}

void Adif::logEvent(const QString &_func, const QString &_msg,  DebugLogLevel _level)
{
    emit debugLog (_func, _msg, _level);
}
