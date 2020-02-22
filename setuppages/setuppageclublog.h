#ifndef SETUPPAGECLUBLOG_H
#define SETUPPAGECLUBLOG_H
/***************************************************************************
                          setuppclublog.h  -  description
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
 *    KLog is distributed in the hope that it will be useful,             *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/


#include <QtWidgets>

class SetupPageClubLog : public QWidget {
    Q_OBJECT

public:
    SetupPageClubLog(QWidget *parent=nullptr);
    ~SetupPageClubLog();


    void setEmail(const QString &c);
    void setPassword(const QString &c);
    void setCallsign(const QString &c);
    void setClubLog(const QString &_s);
    void setClubLogRealTime(const QString &_s);
    void setUseStationCall(const QString &_s);



    QString getEmail();
    QString getPassword();
    QString getCallsign();
    QString getClubLog();
    QString getClubLogRealTime();
    QString getUseQSOStationCallsign();



private slots:

    void slotClubLogActive(bool _s);
    void slotUseStationCall(bool _s);

private:

    //QString defaultFileName;

    QCheckBox *sendInRealTimeCheckBox, *clubLogActiveCheckBox, *useQSOStationCallCheckBox;
    QLineEdit *passwordLineEdit, *emailLineEdit, *callLineEdit;
    QString call, email, pass;
    QLabel *callLabel, *passwordLabel, *emailLabel;
    bool clubLogActive;

    //QPushButton *fileNameButton;


};

#endif // SETUPPAGECOLORS_H
