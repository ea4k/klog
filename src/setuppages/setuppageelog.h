#ifndef KLOG_SETUPPAGES_SETUPPAGEELOG_H
#define KLOG_SETUPPAGES_SETUPPAGEELOG_H
/***************************************************************************
                          setuppageelog.h  -  description
                             -------------------
    begin                : oct 2020
    copyright            : (C) 2020 by Jaime Robles
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


#include <QtWidgets>
#include "../utilities.h"

class SetupPageELog : public QWidget {
    Q_OBJECT

public:
    SetupPageELog(QWidget *parent=nullptr);
    ~SetupPageELog();

    //Clublog
    void setClubLogActive(const bool &_b);

    // LoTW
    void setLoTWActive(const bool &_s);

    //QRZ.COM

    void setQRZCOMActive(const bool _s);
    void setQRZCOMAutoCheck(const bool _s);

    void saveSettings();
    void loadSettings();

private slots:

    void slotClubLogActive(const bool _s);
    void slotEQSLActive(const bool _s);
    void slotTQSLActive(const bool _s);
    void slotQRZCOMActive(const bool _s);
    //void slotQRZCOMAuto(const bool _s);
    void slotQRZCallTextChanged();

    void slotSelectTQSLClicked();
    void slotPathLineEditChanged(const QString &_q);
    void slotEnterKeyPressed();
    void slotClubLogEmailDefineColor();
    void slotLoTWEmailDefineColor();
    void sloteQSLCallTextChanged();

signals:
    void enterKey();
    //void qrzcomAuto(bool _s);

protected:
    //void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);

private:
    void setDefaults();
    //QString defaultFileName;
    QGroupBox *clubLogGroup;
    QCheckBox *clubLogSendInRealTimeCheckBox, *clubLogActiveCheckBox;//, *useQSOStationCallCheckBox;
    QLineEdit *clubLogAppPasswordLineEdit, *clubLogPasswordLineEdit, *clubLogEmailLineEdit;//, *callLineEdit;
    QLabel *clubLogAppPasswordLabel, *clubLogPasswordLabel, *clubLogEmailLabel; //*callLabel,

    QGroupBox *eQSLccGroup;
    QLineEdit *eQSLUserLineEdit, *eQSLPasswordLineEdit;
    QCheckBox *eQSLActiveCheckBox; //*eQSLSendInRealTimeCheckBox,
    QLabel *eQSLpasswordLabel, *eQSLemailLabel;
    //bool eqslActive;
    //QString eqslUser, eqslPass;

    QGroupBox *lotwGroup, *lotwUpGroup, *lotwDownGroup;
    QLineEdit *lotwUserLineEdit, *lotwPasswordLineEdit;
    QLineEdit *lotwTQSLPathLineEdit;
    QPushButton *lotwSearchTQSLPushButton;
    QCheckBox *lotwUseTQSLCheckBox;
    QLabel *lotwpasswordLabel, *lotwemailLabel;
    //bool lotwTQSL;
    //QString lotwPath, lotwUser, lotwPass;

    QGroupBox *QRZCOMGroup;
    //QLineEdit *QRZCOMAPILineEdit;
    QLineEdit *QRZCOMUserLineEdit, *QRZCOMPasswordLineEdit, *QRZCOMLogBookKEYLineEdit;
    QLabel *QRZCOMPasswordLabel, *QRZCOMUserLabel, *QRZLogBookKeyLabel;
    QCheckBox *QRZCOMActiveCheckBox, *QRZCOMAutoCheckCheckBox, *QRZCOMSubscriberCheckBox;

    //bool qrzcomAutoFill;
   // QString  qrcomLogbookKey;

    Utilities *util;

    QPalette palRed, palBlack;
};

#endif // SETUPPAGEELOG_H
