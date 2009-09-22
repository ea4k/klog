/***************************************************************************
                          dxspot.h  -  description
                             -------------------
    begin                : lun ene  7 18:42:45 CET 2007
    copyright            : (C) 2007 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/******************************************************************************
 *                                                                            *
 *   This program is free software; you can redistribute it and/or modify     *
*  it under the terms of the GNU General Public License as published by     *
*  the Free Software Foundation; either version 2 of the License, or        *
*  (at your option) any later version.                                      *
 *                                                                            *
*  This program is distributed in the hope that it will be useful,          *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
*  GNU General Public License for more details.                             *
 *                                                                            *
*  You should have received a copy of the GNU General Public License        *
*  along with this program; if not, write to the Free Software              *
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA*
 *****************************************************************************/


#ifndef DXSPOT_H
#define DXSPOT_H
#include <QString>
#include <QDateTime>

 #define CALL_LENGTH 15

class DxSpot{
    public:
        DxSpot();
//		DxSpot(const QString& tqrz, int tnumb, QString& tmode);
        ~DxSpot();

        void setDxCall(const QString &tdxcall);
        QString getDxCall() const;

        void setSpotter(const QString &tdxcall);
        QString getSpotter() const;

        void setNumb (int tnumb);
        int getNumb() const;

        void setDateTime(QDateTime tdateTime);
        QDateTime getDateTime() const;

        void setDate(QDate tdate);
        QDate getDate() const;

        void setTime(QTime ttime);
        QTime getTime() const;

        void setFreq(double tfreq);
        double getFreq() const;

        void clearDXSpot();

        void setcomment(const QString &tcomment);
        QString getcomment() const;


    private:
        QString dxcall;        // CALL
        QString dxspotter;        // CALL
        int numb;           // STX
        QDateTime dateTime; // QSO_DATE/TIME_ON YYYYMMDD - Time HHMM (0000-2359)
        double freq;       // Frecuency
        QString comment;    // COMMENT

};


#endif

