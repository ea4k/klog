/***************************************************************************
                          contestcqwwdxssb.cpp  -  description
                             -------------------
    begin                : sept 2011
    copyright            : (C) 2011 by Jaime Robles
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

#include "contest_cqwwdxssb.h"
#include <QSqlError>

//#include <QDebug>
/*
V. MULTIPLIER: Two types of multiplier will be used.

    A multiplier of one (1) for each different zone contacted on each band.
    A multiplier of one (1) for each different country contacted on each band.
    Stations are permitted to contact their own country and zone for multiplier credit.
The CQ Zone Map, DXCC country list, WAE country list, and WAC boundaries are standards.
Maritime mobile stations count only for a zone multiplier.

VI. POINTS:

    Contacts between stations on different continents are worth three (3) points.
    Contacts between stations on the same continent but different countries, one (1) point.
Exception: For North American stations only, contacts between stations within the North American
boundaries count two (2) points.

    Contacts between stations in the same country are permitted for zone or country multiplier
credit but have zero (0) point value.

VII. SCORING: All stations: the final score is the result of the total QSO points multiplied by
the sum of your zone and country multipliers.

Example: 1000 QSO points × 100 multiplier (30 Zones + 70 Countries) = 100,000 (final score).

*/

ContestCQWWDXSSB::ContestCQWWDXSSB()
{
    //qDebug() << "ContestCQWWDXSSB::ContestCQWWDXSSB"  << endl;
    myEntity = "";
    myCQz = "";
    myContinent = "";
    NA = -1;
    mycategory = "";
    arrlSection = "";
    stationQrz = "";
    claimedScore = "";
    name = "";
    address = "";
    operators = "";
    soapbox = "";
    thiscontest = "";
    club = "";
    createdby = "";

}

ContestCQWWDXSSB::ContestCQWWDXSSB(const QStringList _qs)
{

    // Receives:  QStringList _qs;
    //_qs << myEntity << myCQz << myContinent << NA-id;
    myEntity = _qs.at(0);
    myCQz = _qs.at(1);
    myContinent = _qs.at(2);
    NA = _qs.at(3); // NA is North America for scoring purposes
    thiscontest = "CQ-WW-SSB";
    mycategory = "SINGLE-OP ALL HIGH";
    club = "NoClub";
    createdby ="KLog-Alpha";

    /* OFFICIAL CATEGORIES
    CATEGORY: SINGLE-OP ALL HIGH
    CATEGORY: SINGLE-OP 160M HIGH
    CATEGORY: SINGLE-OP 80M HIGH
    CATEGORY: SINGLE-OP 40M HIGH
    CATEGORY: SINGLE-OP 20M HIGH
    CATEGORY: SINGLE-OP 15M HIGH
    CATEGORY: SINGLE-OP 10M HIGH
    CATEGORY: SINGLE-OP ALL LOW
    CATEGORY: SINGLE-OP 160M LOW
    CATEGORY: SINGLE-OP 80M LOW
    CATEGORY: SINGLE-OP 40M LOW
    CATEGORY: SINGLE-OP 20M LOW
    CATEGORY: SINGLE-OP 15M LOW
    CATEGORY: SINGLE-OP 10M LOW
    CATEGORY: SINGLE-OP ALL QRP
    CATEGORY: SINGLE-OP 160M QRP
    CATEGORY: SINGLE-OP 80M QRP
    CATEGORY: SINGLE-OP 40M QRP
    CATEGORY: SINGLE-OP 20M QRP
    CATEGORY: SINGLE-OP 15M QRP
    CATEGORY: SINGLE-OP 10M QRP
    CATEGORY: SINGLE-OP-ASSISTED ALL
    CATEGORY: SINGLE-OP-ASSISTED 160M
    CATEGORY: SINGLE-OP-ASSISTED 80M
    CATEGORY: SINGLE-OP-ASSISTED 40M
    CATEGORY: SINGLE-OP-ASSISTED 20M
    CATEGORY: SINGLE-OP-ASSISTED 15M
    CATEGORY: SINGLE-OP-ASSISTED 10M
    CATEGORY: MULTI-ONE
    CATEGORY: MULTI-TWO
    CATEGORY: MULTI-MULTI
    CATEGORY: CHECKLOG
    */
    arrlSection = "DX";
    stationQrz = "";
    claimedScore = "000";
    name = "No-Name";
    address = "MyPoBox\nADDRESS: SecondLineOfAddress";
    operators = "EA4TV, EA4GCA";
    soapbox = "MySoapbox\nSOAPBOX: Another line\nSOAPBOX: and even one more";





    //qDebug() << "ContestCQWWDXSSB::ContestCQWWDXSSB: " << myEntity << "/" << myCQz << "/" << myContinent << "/" << NA << endl;
}

ContestCQWWDXSSB::~ContestCQWWDXSSB(){
}

bool ContestCQWWDXSSB::isMultiplier(const QStringList _qs){
//qDebug() << "ContestCQWWDXSSB::isMultiplier"  << endl;
for (int i = 0; i<_qs.length(); i++){
    ////qDebug() << _qs.at(i) <<", ";
}
    // Receives:  QStringList _qs;
    //_qs << DX-Entity << DXCQz << DX-band;

//    V. MULTIPLIER: Two types of multiplier will be used.
//
//        A multiplier of one (1) for each different zone contacted on each band.
//        A multiplier of one (1) for each different country contacted on each band.
//        Stations are permitted to contact their own country and zone for multiplier credit.
//    The CQ Zone Map, DXCC country list, WAE country list, and WAC boundaries are standards.
//    Maritime mobile stations count only for a zone multiplier.

    if (_qs.length() != 3){
        //qDebug() << "ContestCQWWDXSSB::isMultiplier: diferent THAN 3"  << endl;
        return false;
    }


    QString dxEntity = _qs.at(0);
    QString dxCQz  = _qs.at(1);

    if (!( (isValidCQz(dxCQz) ) && (isValidEntity(dxEntity) ) ) ){
        return false;
    }
    //TODO: Check if the band is a valid string
    QString dxBand = _qs.at(2);

//        A multiplier of one (1) for each different zone contacted on each band.
    QString queryString;
    QSqlQuery query;
    queryString = QString("SELECT id FROM log WHERE bandid='%1' AND stx='%2'").arg(dxBand).arg(dxCQz);
    //qDebug() << "ContestCQWWDXSSB::isMultiplier-1: " << queryString << endl;
    bool sqlOK = query.exec(queryString);
    if (!sqlOK)
    {

        //emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number());
    }
    query.next();

    if (!(query.isValid())){
        //qDebug() << "ContestCQWWDXSSB::isMultiplier: Not Worked Zone"  << endl;
        return true;
    }


//        A multiplier of one (1) for each different country contacted on each band.    
    queryString = QString("SELECT id FROM log WHERE bandid='%1' AND dxcc='%2'").arg(dxBand).arg(dxEntity);
    //qDebug() << "ContestCQWWDXSSB::isMultiplier-2: " << queryString << endl;
    sqlOK = query.exec(queryString);
    if (!sqlOK)
    {
        //emit queryError(Q_FUNC_INFO, query.lastError().databaseText(), query.lastError().number());
    }

    query.next();

    if (!(query.isValid())){
        //qDebug() << "ContestCQWWDXSSB::isMultiplier: MULT2 - Not worked DXCC"  << endl;
        return true;
    }

    return false;
}

int ContestCQWWDXSSB::getQSOPoints(const QStringList _qs){
//qDebug() << "ContestCQWWDXSSB::getQSOPoints"  << endl;
// Receives:  QStringList _qs;
//_qs << DX-Entity << DX-Continent

//    VI. POINTS:
//
//        Contacts between stations on different continents are worth three (3) points.
//        Contacts between stations on the same continent but different countries, one (1) point.
//    Exception: For North American stations only, contacts between stations within the North American
//    boundaries count two (2) points.
//
//        Contacts between stations in the same country are permitted for zone or country multiplier
//    credit but have zero (0) point value.
    if (_qs.length() != 2){
        return false;
    }
    QString dxEntity =_qs.at(0);
    QString dxContinent = _qs.at(1);

    if (myContinent != dxContinent){
        return 3;
    }else if (myContinent == NA) {
        return 2;
    } else if (myEntity != dxEntity){
        return 1;
    }else{
        return 0;
    }

    return 0;
}

bool ContestCQWWDXSSB::isValidCQz(const QString _cqz){
//qDebug() << "ContestCQWWDXSSB::isValidCQz: " << _cqz << endl;
    if (  (_cqz.toInt()>0) && (_cqz.toInt()<41) ){
        return true;
    }else{
        return false;
    }
    return false;
}

bool ContestCQWWDXSSB::isValidEntity(const QString _ent){
    //qDebug() << "ContestCQWWDXSSB::isValidEntity: " << _ent << endl;
    if (  (_ent.toInt()>0) ) {
        return true;
    }else{
        return false;
    }
    return false;
}

bool ContestCQWWDXSSB::saveFileToSend(const QString& _fileName)
{
    //qDebug() << "Contest::saveFileToSend"  << _fileName << endl;

    QFile file(_fileName);
    QSqlQuery query1;

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);

//_qs << myEntity << myCQz << myContinent << NA-id;
//myARRLSect << StationQRZ << myCategory << myClub <<
//myAddress << operators(comma separated EA4TV, EA4GCA, ...)
//SoapBox


    out << "START-OF-LOG: 3" << endl;
    out << "ARRL-SECTION: " << arrlSection << endl;
    out << "CALLSIGN: " << stationQrz << endl;
    out << "CATEGORY: " + mycategory << endl;

    out << "CLAIMED-SCORE: " << claimedScore << endl;
    out << "CLUB: " << club  << endl;
    out << "CONTEST: " << thiscontest << endl;
    out << "NAME: " << name << endl;
    out << "ADDRESS: " << address  << endl;
    out << "CREATED-BY: " <<  createdby  << endl;
    out << "OPERATORS: " << operators  << endl;// [required for multi-op stations]
    out << "SOAPBOX: " << soapbox << endl; //[add lines if needed]






    QSqlQuery query("SELECT * FROM log");

    QSqlRecord rec = query.record();
    int nameCol;
    QString aux1, aux2, queryString;
    /*
START-OF-LOG: 3.0
CONTEST: CQ-WW-RTTY
CONTEST: CQ-WW-SSB
CONTEST: CQ-WW-CW
CALLSIGN: LU6ETB
CATEGORY-OPERATOR: SINGLE-OP
CATEGORY-BAND: 10M
CATEGORY-POWER: HIGH
CATEGORY-ASSISTED: NON-ASSISTED
CLAIMED-SCORE: 1410
OPERATORS: N5KO
CLUB: Radio Club Quilmes
NAME: Trey Garlough
ADDRESS: 7375 Oak Ridge Road
ADDRESS-CITY: Aptos
ADDRESS-STATE: CA
ADDRESS-POSTALCODE: 95003
ADDRESS-COUNTRY: USA
QSO:  3799 PH 2000-11-26 0711 N6TW          59  03     K9QZO         59  04     0
END-OF-LOG:

    */

    while (query.next()) {
        if ( query.isValid())
        {

            out << "QSO: ";
            nameCol = rec.indexOf("freq");
            aux1 = (query.value(nameCol)).toString();

            if (aux1.length()<1)
            {
                nameCol = rec.indexOf("bandid");
                aux1 = (query.value(nameCol)).toString();
                queryString = QString("SELECT cabrillo FROM band WHERE id='%1'").arg(aux1);
                bool sqlOK = query1.exec(queryString);
                if (!sqlOK)
                {
                    //emit queryError(Q_FUNC_INFO, query1.lastError().databaseText(), query1.lastError().number());
                }
                query1.next();

                if (query1.isValid())
                {
                    aux1 = query1.value(0).toString();
                    //qDebug() << "ContestCQWWDXSSB::saveFileToSend: band: "  << queryString << "/" << aux1 << endl;

                    if (!(aux1.length()>1))
                    {
                        aux1 = " NULL";
                    }
                }
            }
            aux1.remove(QChar('.'), Qt::CaseInsensitive);

            out << aux1.rightJustified(5, ' ', true) << " ";


            nameCol = rec.indexOf("modeid");
            aux1 = (query.value(nameCol)).toString();
            queryString = QString("SELECT name FROM mode WHERE id='%1'").arg(aux1);
            bool sqlOK = query1.exec(queryString);
            if (!sqlOK)
            {
                //emit queryError(Q_FUNC_INFO, query1.lastError().databaseText(), query1.lastError().number());
            }
            query1.next();

            if (query1.isValid())
            {
                aux1 = (query1.value(0)).toString();
                if (aux1=="RTTY")
                {
                    out << "RY ";
                }
                    else if (aux1=="CW")
                {
                    out << "CW ";
                }
                else
                {
                    out << "PH ";
                }
            }

            nameCol = rec.indexOf("qso_date");

            aux1 = (query.value(nameCol)).toString();
            //qDebug() << "ContestCQWWDXSSB::saveFileToSend date: " << aux1 << endl;
            aux1.replace(QString("/"), QString("-"));

            if ((aux1.length()) == 10)
            {
                out << aux1 << " ";
            }

            nameCol = rec.indexOf("time_on");
            aux1 = (query.value(nameCol)).toString();
            //qDebug() << "ContestCQWWDXSSB::saveFileToSend time1: " << aux1 << endl;
            aux1.remove(QChar(':'), Qt::CaseInsensitive);
            aux1.truncate(4);
            aux1 = aux1.leftJustified(4, '0', true);
            out << aux1 << " ";

            //qDebug() << "ContestCQWWDXSSB::saveFileToSend time2: " << aux1 << endl;

            nameCol = rec.indexOf("station_callsign");
            aux1 = (query.value(nameCol)).toString();
            if (aux1.length()<3)
            {
                nameCol = rec.indexOf("operator");
                aux1 = (query.value(nameCol)).toString();
            }

            out << aux1.leftJustified(13, ' ', true) << " ";

            nameCol = rec.indexOf("rst_sent");
            aux1 = (query.value(nameCol)).toString();
            aux1.truncate(3);
            out << aux1.leftJustified(3, ' ', true) << " ";

            //qDebug() << "ContestCQWWDXSSB::saveFileToSend: rsttx" << aux1 << endl;

            nameCol = rec.indexOf("stx");
            aux1 = (query.value(nameCol)).toString();
            aux1.truncate(6);
            out << aux1.leftJustified(6, ' ', true) << " ";

            nameCol = rec.indexOf("call");
            aux1 = (query.value(nameCol)).toString();
            aux1.truncate(13);
            out << aux1.leftJustified(13, ' ') << " ";

            nameCol = rec.indexOf("rst_rcvd");
            aux1 = (query.value(nameCol)).toString();

            out << aux1.leftJustified(3, ' ', true) << " ";

            nameCol = rec.indexOf("srx");
            aux1 = (query.value(nameCol)).toString();
            aux1.truncate(6);
            out << aux1.leftJustified(6, ' ', true) << " ";

            nameCol = rec.indexOf("transmiterid");
            aux1 = (query.value(nameCol)).toString();
            aux1.truncate(1);
            out << aux1 ;

        // out << "0"; // This is the RIG used to log the QSO. 1 is used as

            out << endl;
        }
    }

        out << "END-OF-LOG:" << endl;
    return true;
}

int ContestCQWWDXSSB::getTotalScore()
{

    return (getMultipliers()) * (getPoints());

}
int ContestCQWWDXSSB::getMultipliers()
{
    int multipliers = 0;
    QSqlQuery query("SELECT * FROM log");
    QSqlRecord rec = query.record();
    int nameCol = rec.indexOf("multiplier");

    while (query.next()) {
        if (query.isValid())
        {
            multipliers += (query.value(nameCol)).toInt();
        }
    }

    return multipliers;
}

int ContestCQWWDXSSB::getPoints()
{
    int points = 0;
    QSqlQuery query("SELECT * FROM log");
    QSqlRecord rec = query.record();
    int nameCol = rec.indexOf("points");

    while (query.next()) {
        if (query.isValid())
        {
            points += (query.value(nameCol)).toInt();
        }
    }

    return points;
}
