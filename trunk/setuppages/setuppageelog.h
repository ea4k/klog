#ifndef SETUPPAGEELOG_H
#define SETUPPAGEELOG_H
/***************************************************************************
                          setuppageelog.h  -  description
                             -------------------
    begin                : oct 2020
    copyright            : (C) 2020 by Jaime Robles
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
#include "utilities.h"

class SetupPageELog : public QWidget {
    Q_OBJECT

public:
    SetupPageELog(QWidget *parent=nullptr);
    ~SetupPageELog();

    //Clublog
    void setClubLogEmail(const QString &c);
    void setClubLogPassword(const QString &c);
    void setClubLogActive(const QString &_s);
    void setClubLogRealTime(const QString &_s);

    QString getClubLogEmail();
    QString getClubLogPassword();
    QString getClubLogActive();
    QString getClubLogRealTime();


    // LoTW
    void setTQSLPath(const QString &c);
    QString getTQSLPath();
    void setLoTWActive(const QString &_s);
    QString getLoTWActive();
    void setLoTWUser(const QString &_s);
    QString getLoTWUser();
    void setLoTWPass(const QString &_s);
    QString getLoTWPass();

    // eQSL
    void setEQSLEmail(const QString &c);
    void setEQSLPassword(const QString &c);
    void setEQSLActive(const QString &_s);
    //void setEQSLRealTime(const QString &_s);

    QString getEQSLEmail();
    QString getEQSLPassword();
    QString getEQSLActive();
    //QString getEQSLRealTime();

    //QRZ.COM
    void setQRZCOMUser(const QString &_s);
    void setQRZCOMPassword(const QString &_s);

    //void setQRZCOMAPI(const QString &c);
    void setQRZCOMActive(const QString &_s);
    void setQRZCOMAutoCheck(const QString &_s);

    QString getQRZCOMUser();
    QString getQRZCOMPassword();

    //QString getQRZCOMAPI();
    QString getQRZCOMActive();    
    QString getQRZCOMAutoCheck();


private slots:

    void slotClubLogActive(bool _s);
    void slotEQSLActive(bool _s);
    void slotTQSLActive(bool _s);
    void slotQRZCOMActive(bool _s);
    void slotQRZCOMAuto(bool _s);

    void slotSelectTQSLClicked();
    void slotPathLineEditChanged(const QString _q);
    void slotEnterKeyPressed();
    void slotClubLogEmailDefineColor();
    void slotLoTWEmailDefineColor();
signals:
    void enterKey();
    void qrzcomAuto(bool _s);

protected:
    //void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);
private:
    void setDefaults();
    //QString defaultFileName;
    QGroupBox *clubLogGroup;
    QCheckBox *clubLogSendInRealTimeCheckBox, *clubLogActiveCheckBox;//, *useQSOStationCallCheckBox;
    QLineEdit *clubLogPasswordLineEdit, *clubLogEmailLineEdit;//, *callLineEdit;   
    QLabel *clubLogPasswordLabel, *clubLogEmailLabel; //*callLabel,
    QString clubLogEmail, clubLogPass;
    bool clubLogActive, clubLogRealTime;

    QGroupBox *eQSLccGroup;
    QLineEdit *eQSLUserLineEdit, *eQSLPasswordLineEdit;    
    QCheckBox *eQSLActiveCheckBox; //*eQSLSendInRealTimeCheckBox,
    QLabel *eQSLpasswordLabel, *eQSLemailLabel;
    bool eqslActive;
    QString eqslUser, eqslPass;

    QGroupBox *lotwGroup, *lotwUpGroup, *lotwDownGroup;
    QLineEdit *lotwUserLineEdit, *lotwPasswordLineEdit;
    QLineEdit *lotwTQSLPathLineEdit;
    QPushButton *lotwSearchTQSLPushButton;
    QCheckBox *lotwUseTQSLCheckBox;
    QLabel *lotwpasswordLabel, *lotwemailLabel;
    bool lotwTQSL;
    QString lotwPath, lotwUser, lotwPass;

    QGroupBox *QRZCOMGroup;
    //QLineEdit *QRZCOMAPILineEdit;
    QLineEdit *QRZCOMUserLineEdit, *QRZCOMPasswordLineEdit;
    QLabel *QRZCOMPasswordLabel, *QRZCOMUserLabel;
    QCheckBox *QRZCOMActiveCheckBox, *QRZCOMAutoCheckCheckBox;

    bool qrzcomActive, qrzcomAutoFill;
    QString qrzComUser, qrzcomPass;

    Utilities *util;



};

#endif // SETUPPAGEELOG_H
