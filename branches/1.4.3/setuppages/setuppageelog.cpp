/***************************************************************************
                          setuppageelog.cpp  -  description
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

//#include <QDebug>

#include "setuppages/setuppageelog.h"


SetupPageELog::SetupPageELog(QWidget *parent) : QWidget(parent)
{
    //qDebug() << "SetupPageELog::SetupPageELog" << endl;

    palRed.setColor(QPalette::Text, Qt::red);
    palBlack.setColor(QPalette::Text, Qt::black);

    clubLogEmailLineEdit = new QLineEdit;
    clubLogPasswordLineEdit = new QLineEdit;
    clubLogPasswordLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);


    clubLogPasswordLabel = new QLabel(tr("ClubLog password"));
    clubLogEmailLabel = new QLabel(tr("ClubLog email"));
    clubLogPasswordLabel->setBuddy(clubLogPasswordLineEdit);
    clubLogEmailLabel->setBuddy(clubLogEmailLineEdit);

    clubLogEmailLineEdit->setToolTip(tr("Enter the email you used to register in ClubLog."));
    clubLogPasswordLineEdit->setToolTip(tr("Enter your password ClubLog here. Warning: The password will be save on clear in the KLog config file!! (If you don't want to enter the password, KLog will ask you when it is needed.)"));

    QRegularExpression rx("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b",
                              QRegularExpression::CaseInsensitiveOption);
    clubLogEmailLineEdit->setValidator(new QRegularExpressionValidator(rx, this));

    clubLogSendInRealTimeCheckBox = new QCheckBox(tr("Send QSOs in real time"), this);
    clubLogActiveCheckBox = new QCheckBox(tr("Activate ClubLog"), this);
    clubLogSendInRealTimeCheckBox->setToolTip(tr("Send each QSO to ClubLog in real time, as they are added (or modified) in KLog."));
    clubLogActiveCheckBox->setToolTip(tr("Starts the ClubLog support in KLog."));
    //qDebug() << "SetupPageELog::SetupPageELog - 00010" << endl;
    eQSLUserLineEdit = new QLineEdit;
    eQSLPasswordLineEdit = new QLineEdit;
    eQSLPasswordLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    eQSLActiveCheckBox = new QCheckBox(tr("Activate eQSL.cc"), this);
    eQSLUserLineEdit->setToolTip(tr("Enter your username of eQSL.cc."));
    eQSLPasswordLineEdit->setToolTip(tr("Enter your password eQSL.cc here. Warning: The password will be save on clear in the KLog config file!! (If you don't want to enter the password, KLog will ask you when it is needed.)"));
    //eQSLSendInRealTimeCheckBox = new QCheckBox(tr("Send QSOs in real time"), this);

    eQSLpasswordLabel = new QLabel(tr("eQSL.cc password"));
    eQSLemailLabel = new QLabel(tr("eQSL.cc user"));

    clubLogGroup = new QGroupBox (tr("ClubLog"));
    eQSLccGroup = new QGroupBox (tr("eQSL.cc"));
    //qDebug() << "SetupPageELog::SetupPageELog - 00020" << endl;
    QGridLayout *cl1layout = new QGridLayout;
    cl1layout->addWidget(clubLogEmailLabel, 0, 0);
    cl1layout->addWidget(clubLogPasswordLabel, 1, 0);
    cl1layout->addWidget(clubLogEmailLineEdit, 0, 1);
    cl1layout->addWidget(clubLogPasswordLineEdit, 1, 1);

    QVBoxLayout *cl2layout = new QVBoxLayout;
    cl2layout->addWidget(clubLogActiveCheckBox);
    //cl2layout->addWidget(warningLabel);
    cl2layout->addLayout(cl1layout);
    cl2layout->addWidget(clubLogSendInRealTimeCheckBox);
    clubLogGroup->setLayout(cl2layout);
    //qDebug() << "SetupPageELog::SetupPageELog - 00030" << endl;
    QGridLayout *e1layout = new QGridLayout;
    e1layout->addWidget(eQSLemailLabel, 0, 0);
    e1layout->addWidget(eQSLpasswordLabel, 1, 0);
    e1layout->addWidget(eQSLUserLineEdit, 0, 1);
    e1layout->addWidget(eQSLPasswordLineEdit, 1, 1);

    QVBoxLayout *e2layout = new QVBoxLayout;
    e2layout->addWidget(eQSLActiveCheckBox);
    e2layout->addLayout(e1layout);
    //e2layout->addWidget(eQSLSendInRealTimeCheckBox);
    eQSLccGroup->setLayout(e2layout);
    //qDebug() << "SetupPageELog::SetupPageELog - 00040" << endl;
    QRZCOMGroup = new QGroupBox (tr("QRZ.com"));

    QRZCOMUserLineEdit = new QLineEdit;
    QRZCOMUserLineEdit->setToolTip(tr("Enter the user of your QRZ.com account. You need to be subscribed to QRZ.com to use this service."));

    QRZCOMUserLabel = new QLabel(tr("User"));

    QRZCOMPasswordLineEdit = new QLineEdit;
    QRZCOMPasswordLineEdit->setToolTip(tr("Enter your password QRZ.com here. Warning: The password will be save on clear in the KLog config file!! (If you don't want to enter the password, KLog will ask you when it is needed.)"));
    QRZCOMPasswordLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);

    QRZCOMPasswordLabel = new QLabel(tr("Password"));
    QRZCOMActiveCheckBox = new QCheckBox(tr("Activate QRZ.com"), this);
    QRZCOMAutoCheckCheckBox = new QCheckBox(tr("Check automatically"), this);
    QRZCOMAutoCheckCheckBox->setToolTip(tr("Check in Qrz.com all QRZ as they are entered"));
    //qDebug() << "SetupPageELog::SetupPageELog - 00050" << endl;

    QRZLogBookKeyLabel = new QLabel(tr("LogBook Key"));
    QRZCOMLogBookKEYLineEdit = new QLineEdit;
    QRZCOMLogBookKEYLineEdit->setToolTip(tr("LogBook Key for QSO uploading. You can get this key in your QRZ.com logbook webpage. Remember that you need a QRZ.com subscription to use this feature."));


    QGridLayout *q1layout = new QGridLayout;
    q1layout->addWidget(QRZCOMUserLabel, 0, 0);
    q1layout->addWidget(QRZCOMPasswordLabel, 1, 0);
    q1layout->addWidget(QRZCOMUserLineEdit, 0, 1);
    q1layout->addWidget(QRZCOMPasswordLineEdit, 1, 1);

    QHBoxLayout *qrzcomKeyLayout = new QHBoxLayout;
    qrzcomKeyLayout->addWidget(QRZLogBookKeyLabel);
    qrzcomKeyLayout->addWidget(QRZCOMLogBookKEYLineEdit);

    //qDebug() << "SetupPageELog::SetupPageELog - 00070" << endl;
    QVBoxLayout *qrzLayout = new QVBoxLayout;
    qrzLayout->addWidget(QRZCOMActiveCheckBox);
    qrzLayout->addLayout(q1layout);
    qrzLayout->addWidget(QRZCOMAutoCheckCheckBox);
    qrzLayout->addLayout(qrzcomKeyLayout);

    QRZCOMGroup->setLayout(qrzLayout);

    //qDebug() << "SetupPageELog::SetupPageELog - 00080" << endl;

    lotwGroup = new QGroupBox (tr("LoTW"));
    lotwUpGroup = new QGroupBox (tr("Upload"));
    lotwDownGroup = new QGroupBox (tr("Download"));

    lotwUserLineEdit = new QLineEdit;
    //lotwUserLineEdit->setValidator(new QRegularExpressionValidator(rx, this));

    lotwPasswordLineEdit = new QLineEdit;
    lotwPasswordLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    lotwTQSLPathLineEdit = new QLineEdit;
    lotwSearchTQSLPushButton = new QPushButton(tr("TQSL path"), this);
    lotwUseTQSLCheckBox = new QCheckBox(tr("Use TQSL"), this);
    lotwpasswordLabel = new QLabel(tr("LoTW password"));
    lotwemailLabel = new QLabel(tr("LoTW user"));

    lotwUserLineEdit->setToolTip(tr("Enter your LoTW user."));
    lotwPasswordLineEdit->setToolTip(tr("Enter your password LoTW here. Warning: The password will be save on clear in the KLog config file!! (If you don't want to enter the password, KLog will ask you when it is needed.)"));
    lotwTQSLPathLineEdit->setToolTip(tr("Path to the TQSL software."));
    lotwSearchTQSLPushButton->setToolTip(tr("Enable the LoTW integration with TQSL. You will need to have TQSL installed"));


    QHBoxLayout *l1layout = new QHBoxLayout;
    l1layout->addWidget(lotwTQSLPathLineEdit);
    l1layout->addWidget(lotwSearchTQSLPushButton);
    //lotwUpGroup->setLayout(l1layout);

    QVBoxLayout *l2layout = new QVBoxLayout;
    l2layout->addWidget(lotwUseTQSLCheckBox);
    l2layout->addLayout(l1layout);
    lotwUpGroup->setLayout(l2layout);

    QGridLayout *l3layout = new QGridLayout;
    l3layout->addWidget(lotwemailLabel, 0, 0);
    l3layout->addWidget(lotwpasswordLabel, 1, 0);
    l3layout->addWidget(lotwUserLineEdit, 0, 1);
    l3layout->addWidget(lotwPasswordLineEdit, 1, 1);
    lotwDownGroup->setLayout(l3layout);
    //qDebug() << "SetupPageELog::SetupPageELog - 00100" << endl;
    QVBoxLayout *llayout = new QVBoxLayout;
    //llayout->addLayout(l2layout);
    llayout->addWidget(lotwUpGroup);
    llayout->addWidget(lotwDownGroup);
    lotwGroup->setLayout(llayout);
    //qDebug() << "SetupPageELog::SetupPageELog - 00110" << endl;
    QVBoxLayout *v1layout = new QVBoxLayout;
    v1layout->addWidget(clubLogGroup);
    v1layout->addWidget(eQSLccGroup);
    //qDebug() << "SetupPageELog::SetupPageELog - 00120" << endl;
    QVBoxLayout *v2layout = new QVBoxLayout;
    v2layout->addWidget(QRZCOMGroup);
    v2layout->addWidget(lotwGroup);
    //qDebug() << "SetupPageELog::SetupPageELog - 00130" << endl;
    QHBoxLayout *mlayout = new QHBoxLayout;
    mlayout->addLayout(v1layout);
    mlayout->addLayout(v2layout);
    setLayout(mlayout);
    //qDebug() << "SetupPageELog::SetupPageELog - 00140" << endl;
    connect(clubLogActiveCheckBox, SIGNAL(toggled(bool) ), this, SLOT(slotClubLogActive(bool)));

    connect(clubLogPasswordLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(clubLogEmailLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );

    connect(eQSLUserLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(eQSLPasswordLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(eQSLActiveCheckBox, SIGNAL(toggled(bool) ), this, SLOT(slotEQSLActive(bool)));
    connect(eQSLUserLineEdit, SIGNAL(textChanged(QString)), this, SLOT(sloteQSLCallTextChanged() ) );
    //qDebug() << "SetupPageELog::SetupPageELog - 00150" << endl;
    connect(QRZCOMActiveCheckBox, SIGNAL(toggled(bool) ), this, SLOT(slotQRZCOMActive(bool)));
    connect(QRZCOMAutoCheckCheckBox, SIGNAL(toggled(bool) ), this, SLOT(slotQRZCOMAuto(bool)));
    connect(QRZCOMUserLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(QRZCOMUserLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotQRZCallTextChanged() ) );
    connect(QRZCOMPasswordLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    //qDebug() << "SetupPageELog::SetupPageELog - 00160" << endl;

    connect(lotwUserLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(lotwPasswordLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(lotwTQSLPathLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(lotwSearchTQSLPushButton, SIGNAL(clicked()), this, SLOT(slotSelectTQSLClicked()) );
    connect(lotwTQSLPathLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotPathLineEditChanged(QString)) );
    connect(lotwUseTQSLCheckBox, SIGNAL(toggled(bool) ), this, SLOT(slotTQSLActive(bool)));

    connect(clubLogEmailLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotClubLogEmailDefineColor() ) );
    connect(lotwUserLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotLoTWEmailDefineColor() ) );

    //qDebug() << "SetupPageELog::SetupPageELog - 00150" << endl;
    setDefaults();
    slotClubLogActive(false);
    setLoTWActive(false);
    slotTQSLActive(false);
    slotEQSLActive(false);
    slotQRZCOMActive(false);

    //qDebug() << "SetupPageELog::SetupPageELog - END" << endl;
}

SetupPageELog::~SetupPageELog()
{
}

void SetupPageELog::slotQRZCallTextChanged()
{
   //qDebug() << "SetupPageELog::slotQRZCallTextChanged" << endl;
    int cursor = QRZCOMUserLineEdit->cursorPosition();
   //qDebug() << "SetupPageELog::slotQRZCallTextChanged-1" << endl;
    QString aux = QRZCOMUserLineEdit->text();
   //qDebug() << "SetupPageELog::slotQRZCallTextChanged-2" << endl;

    if (util->isValidCall(aux))
    {
       //qDebug() << "SetupPageELog::slotQRZCallTextChanged-2.1" << endl;
        QRZCOMUserLineEdit->setPalette(palBlack);
    }
    else
    {
       //qDebug() << "SetupPageELog::slotQRZCallTextChanged-2.2" << endl;
         QRZCOMUserLineEdit->setPalette(palRed);
    }
   //qDebug() << "SetupPageELog::slotQRZCallTextChanged-3" << endl;
    QRZCOMUserLineEdit->setText(aux.toUpper());
   //qDebug() << "SetupPageELog::slotQRZCallTextChanged-4" << endl;
    QRZCOMUserLineEdit->setCursorPosition(cursor);

   //qDebug() << "SetupPageELog::slotQRZCallTextChanged - END" << endl;
}

void SetupPageELog::sloteQSLCallTextChanged()
{
   //qDebug() << "SetupPageELog::sloteQSLCallTextChanged" << endl;
    int cursor = eQSLUserLineEdit->cursorPosition();
    QString aux = eQSLUserLineEdit->text();
    if (util->isValidCall(aux))
    {
        eQSLUserLineEdit->setPalette(palBlack);
    }
    else
    {
         eQSLUserLineEdit->setPalette(palRed);
    }
    eQSLUserLineEdit->setText(aux.toUpper());
    eQSLUserLineEdit->setCursorPosition(cursor);
   //qDebug() << "SetupPageELog::sloteQSLCallTextChanged - END" << endl;
}

void SetupPageELog::slotClubLogEmailDefineColor()
{
   //qDebug() << "SetupPageELog::slotClubLogEmailDefineColor" << endl;
    if(!clubLogEmailLineEdit->hasAcceptableInput())
        clubLogEmailLineEdit->setStyleSheet("QLineEdit { color: red;}");
    else
        clubLogEmailLineEdit->setStyleSheet("QLineEdit { color: black;}");
}

void SetupPageELog::slotLoTWEmailDefineColor()
{
   //qDebug() << "SetupPageELog::slotLoTWEmailDefineColor" << endl;
    int cursor = lotwUserLineEdit->cursorPosition();
    QString aux = lotwUserLineEdit->text();

    if (util->isValidCall(aux))
    {
        lotwUserLineEdit->setPalette(palBlack);
    }
    else
    {
         lotwUserLineEdit->setPalette(palRed);
    }
    lotwUserLineEdit->setText(aux.toUpper());
    lotwUserLineEdit->setCursorPosition(cursor);

}

void SetupPageELog::setDefaults()
{
    //qDebug() << "SetupPageELog::setDefaults()" << endl;
    qrzcomActive = false;
    qrzcomAutoFill = false;
    qrzComUser = QString();
    qrzcomPass = QString();
    qrcomLogbookKey = QString();

    clubLogEmail = QString();
    clubLogPass = QString();
    clubLogActive = false;
    clubLogRealTime = false;

    eqslActive = false;
    eqslUser = QString();
    eqslPass = QString();

    lotwTQSL = false;
    lotwPath = QString();
    lotwUser = QString();
    lotwPass = QString();
    //qDebug() << "SetupPageELog::setDefaults() - END" << endl;
}

void SetupPageELog::setClubLogEmail(const QString &c)
{
    clubLogEmail = c;
    clubLogEmailLineEdit->setText(clubLogEmail);
}

void SetupPageELog::setClubLogPassword(const QString &c)
{
    clubLogPass = c;
    clubLogPasswordLineEdit->setText(clubLogPass);
}

QString SetupPageELog::getClubLogEmail()
{
    clubLogEmail = clubLogEmailLineEdit->text();
    return (clubLogEmail);
}

QString SetupPageELog::getClubLogPassword()
{
    clubLogPass = clubLogPasswordLineEdit->text();
    return clubLogPass;
}

bool SetupPageELog::getClubLogActive()
{
    clubLogActive = clubLogActiveCheckBox->isChecked();
    return clubLogActive;
}

void SetupPageELog::setClubLogActive(const bool &_b)
{
    clubLogActive = _b;
    clubLogActiveCheckBox->setEnabled(clubLogActive);
}

void SetupPageELog::slotClubLogActive(const bool _s)
{
    //qDebug() << "SetupPageELog::slotClubLogActive: " << util->boolToQString(_s) << endl;
    clubLogEmailLabel->setEnabled(_s);
    clubLogPasswordLabel->setEnabled(_s);
    clubLogEmailLineEdit->setEnabled(_s);
    clubLogPasswordLineEdit->setEnabled(_s);
    clubLogSendInRealTimeCheckBox->setEnabled(_s);
    //qDebug() << "SetupPageELog::slotClubLogActive" << endl;
}

void SetupPageELog::setClubLogRealTime(const bool &_s)
{
    clubLogRealTime = _s;
    clubLogSendInRealTimeCheckBox->setEnabled(clubLogRealTime);
}

bool SetupPageELog::getClubLogRealTime()
{
    clubLogRealTime = clubLogSendInRealTimeCheckBox->isChecked();
    return clubLogRealTime;
}

 // END of CLubLog

 // Start of eQSL.CC

void SetupPageELog::setEQSLEmail(const QString &c)
{
    eqslUser = c;
    eQSLemailLabel->setText(eqslUser);
}

void SetupPageELog::setEQSLPassword(const QString &c)
{
    eqslPass = c;
    eQSLPasswordLineEdit->setText(eqslPass);
}

QString SetupPageELog::getEQSLUser()
{
    eqslUser = eQSLUserLineEdit->text();
    return eqslUser;
}

QString SetupPageELog::getEQSLPassword()
{
    eqslPass = eQSLPasswordLineEdit->text();
    return eqslPass;
}

bool SetupPageELog::getEQSLActive()
{
    eqslActive = eQSLActiveCheckBox->isChecked();
    return eqslActive;
}

void SetupPageELog::setEQSLActive(const bool _b)
{
    eqslActive = _b;
    eQSLActiveCheckBox->setEnabled(eqslActive);
}

void SetupPageELog::slotEQSLActive(const bool _s)
{   
    //qDebug() << "SetupPageELog::slotEQSLActive: "  << util->boolToQString(_s) << endl;
    eQSLemailLabel->setEnabled(_s);
    eQSLpasswordLabel->setEnabled(_s);
    eQSLUserLineEdit->setEnabled(_s);
    eQSLPasswordLineEdit->setEnabled(_s);
}

void SetupPageELog::slotTQSLActive(const bool _s)
{    
    //qDebug() << "SetupPageELog::slotTQSLActive: " << util->boolToQString(_s) << endl;
    setLoTWActive(_s);
}

 // END of eQSL.CC

 // Start of LoTW


void SetupPageELog::setLoTWActive(const bool &_s)
{
    lotwTQSL = _s;
    lotwUseTQSLCheckBox->setChecked(lotwTQSL);
    lotwTQSLPathLineEdit->setEnabled(lotwTQSL);
    lotwSearchTQSLPushButton->setEnabled(_s);
    lotwUserLineEdit->setEnabled(_s);
    lotwPasswordLineEdit->setEnabled(_s);
}

QString SetupPageELog::getLoTWActive()
{
    lotwTQSL = lotwUseTQSLCheckBox->isChecked();
    return util->boolToQString(lotwTQSL);
}

void SetupPageELog::setTQSLPath(const QString &c)
{
    lotwPath = c;
    lotwTQSLPathLineEdit->setText(lotwPath);
}

QString SetupPageELog::getTQSLPath()
{
    lotwPath = lotwTQSLPathLineEdit->text();
    return lotwPath;
}

void SetupPageELog::slotSelectTQSLClicked()
{
      //qDebug() << "SetupPageELogr::slotSelectTQSLClicked: " << QStandardPaths::ApplicationsLocation << endl;
    QString appsDir;
    QString executableName, proposedName;
    proposedName = util->getTQSLsFileName();
    appsDir = util->getTQSLsPath();
    QString filter;
    filter.clear();
    #if defined(Q_OS_WIN)
        filter = "TQSL (*.exe)";
    #else
        filter = "TQSL (tqsl)";
    #endif
    QString tqslFile;
    tqslFile.clear();
    tqslFile = QFileDialog::getOpenFileName(this, tr("Select File"), appsDir, filter);

    if (tqslFile.length()>0)
    {
        lotwTQSLPathLineEdit->setText(tqslFile);
    }
       //qDebug() << "SetupPageELogr::slotSelectTQSLClicked - END" << endl;
}

void SetupPageELog::slotPathLineEditChanged(const QString _q)
{

    if (QFile::exists(_q))
    {
        lotwTQSLPathLineEdit->setPalette(palBlack);
    }
    else
    {
        lotwTQSLPathLineEdit->setPalette(palRed);
    }
}

void SetupPageELog::setLoTWUser(const QString &_s)
{
    lotwUser = _s;
    lotwUserLineEdit->setText(lotwUser);
}

QString SetupPageELog::getLoTWUser()
{
    lotwUser = lotwUserLineEdit->text();
    return lotwUser;
}

void SetupPageELog::setLoTWPass(const QString &_s)
{
    lotwPass = _s;
    lotwPasswordLineEdit->setText(lotwPass);
}

QString SetupPageELog::getLoTWPass()
{
    lotwPass = lotwPasswordLineEdit->text();
    return lotwPass;
}


 // END of LoTW


void SetupPageELog::slotEnterKeyPressed()
{
    emit enterKey();
}

// QRZ.COM

void SetupPageELog::setQRZCOMActive(const QString &_s)
{
    //qDebug() << "SetupPageELog::setQRZCOMActive " << endl;
    qrzcomActive = util->trueOrFalse(_s);
    QRZCOMActiveCheckBox->setEnabled(qrzcomActive);
}

bool SetupPageELog::SetupPageELog::getQRZCOMActive()
{
    //qDebug() << "SetupPageELog::getQRZCOMActive :" << endl;
    qrzcomActive = QRZCOMActiveCheckBox->isChecked();
    return qrzcomActive;
    //qDebug() << "SetupPageELog::getQRZCOMActive : " << util->boolToQString(qrzcomActive) << endl;
}

void SetupPageELog::slotQRZCOMActive(bool _s)
{

    //qDebug() << "SetupPageELog::slotQRZCOMActive: "  << util->boolToQString(_s) << endl;
    QRZCOMUserLabel->setEnabled(_s);
    //qDebug() << "SetupPageELog::slotQRZCOMActive - 1" << endl;
    QRZCOMUserLineEdit->setEnabled(_s);
    //qDebug() << "SetupPageELog::slotQRZCOMActive - 2" << endl;
    QRZCOMPasswordLabel->setEnabled(_s);
    //qDebug() << "SetupPageELog::slotQRZCOMActive - 3" << endl;
    QRZCOMPasswordLineEdit->setEnabled(_s);
    QRZCOMAutoCheckCheckBox->setEnabled(_s);
    //qDebug() << "SetupPageELog::slotQRZCOMActive - END" << endl;
}

void SetupPageELog::slotQRZCOMAuto(const bool _s)
{
    emit qrzcomAuto(_s);
}

void SetupPageELog::setQRZCOMUser(const QString &_s)
{
    if (_s.length()>0)
    {
        qrzComUser = _s;
        QRZCOMUserLineEdit->setText(qrzComUser);
    }
}

void SetupPageELog::setQRZCOMLogBookKEY(const QString &_s)
{
    if (_s.length()>0)
    {
        qrcomLogbookKey = _s;
        QRZCOMLogBookKEYLineEdit->setText( qrcomLogbookKey);
    }
}

void SetupPageELog::setQRZCOMPassword(const QString &_s)
{
    if (_s.length()>0)
    {
        qrzcomPass = _s;
        QRZCOMPasswordLineEdit->setText(qrzcomPass);
    }
}

QString SetupPageELog::getQRZCOMUser()
{
    qrzComUser = QRZCOMUserLineEdit->text();
    return qrzComUser;
}

QString SetupPageELog::getQRZCOMLogBookKEY()
{
    qrcomLogbookKey = QRZCOMLogBookKEYLineEdit->text();
    return qrcomLogbookKey;
}
QString SetupPageELog::getQRZCOMPassword()
{
    qrzcomPass = QRZCOMPasswordLineEdit->text();
    return qrzcomPass;
}

bool SetupPageELog::getQRZCOMAutoCheck()
{
    qrzcomAutoFill = QRZCOMAutoCheckCheckBox->isChecked();
    return qrzcomAutoFill;
}

void SetupPageELog::setQRZCOMAutoCheck(const QString &_s)
{
    qrzcomAutoFill = util->trueOrFalse(_s);
    QRZCOMAutoCheckCheckBox->setEnabled(qrzcomAutoFill);
}

void SetupPageELog::showEvent(QShowEvent *event)
{
    clubLogSendInRealTimeCheckBox->setChecked(clubLogRealTime);
    clubLogActiveCheckBox->setChecked(clubLogActive);
    clubLogPasswordLineEdit->setText(clubLogPass);
    clubLogEmailLineEdit->setText(clubLogEmail);

    QRZCOMUserLineEdit->setText(qrzComUser);
    QRZCOMPasswordLineEdit->setText(qrzcomPass);
    QRZCOMActiveCheckBox->setChecked(qrzcomActive);
    QRZCOMAutoCheckCheckBox->setChecked(qrzcomAutoFill);
    QRZCOMLogBookKEYLineEdit->setText(qrcomLogbookKey);

    eQSLUserLineEdit->setText(eqslUser);
    eQSLPasswordLineEdit->setText(eqslPass);
    eQSLActiveCheckBox->setChecked(eqslActive);

    lotwUserLineEdit->setText(lotwUser);
    lotwPasswordLineEdit->setText(lotwPass);
    lotwUseTQSLCheckBox->setChecked(lotwTQSL);
    lotwTQSLPathLineEdit->setText(lotwPath);

    event->accept();
}
