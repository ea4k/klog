#ifndef ELOGCLUBLOG_H
#define ELOGCLUBLOG_H
/***************************************************************************
                          elogclublog.h  -  description
                             -------------------
    begin                : apt 2015
    copyright            : (C) 2015 by Jaime Robles
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
 *    KLog is distributed in the hope that it will be useful,                *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/

/*
    This class implements the interface with http://www.clublog.org

Realtime QSO uploading:
    http://clublog.freshdesk.com/support/solutions/articles/54906-how-to-upload-qsos-in-real-time
ADIF ClubLog (16 fields)
    http://clublog.freshdesk.com/support/solutions/articles/53202-which-adif-fields-does-club-log-use-

*/

#include <QNetworkAccessManager>
 #include <QStringList>


class eLogClubLog
{
public:
    eLogClubLog();

    int sendQSO(const QString _qso);

private:
    QString getCLubLogAdif(const QStringList _q);

    //QHttp *http;

};

#endif // ELOGCLUBLOG_H
