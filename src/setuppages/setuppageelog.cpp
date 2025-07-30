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

#include "setuppageelog.h"
#include "../callsign.h"

SetupPageELog::SetupPageELog(QWidget *parent) : QWidget(parent)
{
     //qDebug() << Q_FUNC_INFO " - Start";
    util = new Utilities(Q_FUNC_INFO);

    palRed.setColor(QPalette::Text, Qt::red);
    palBlack.setColor(QPalette::Text, Qt::black);

    clubLogEmailLineEdit = new QLineEdit;
    clubLogPasswordLineEdit = new QLineEdit;
    clubLogAppPasswordLineEdit = new QLineEdit;
    clubLogPasswordLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    clubLogAppPasswordLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    clubLogSentDefaultComboBox = new QComboBox;
    clubLogSentDefaultLabel = new QLabel(tr("Default"));
    clubLogPasswordLabel = new QLabel(tr("ClubLog password"));
    clubLogAppPasswordLabel = new QLabel(tr("ClubLog App password"));
    clubLogEmailLabel = new QLabel(tr("ClubLog email"));
    clubLogAppPasswordLabel->setBuddy (clubLogAppPasswordLineEdit);
    clubLogPasswordLabel->setBuddy(clubLogPasswordLineEdit);
    clubLogEmailLabel->setBuddy(clubLogEmailLineEdit);
    clubLogSentDefaultLabel->setBuddy(clubLogSentDefaultComboBox);

    clubLogEmailLineEdit->setToolTip(tr("Enter the email you used to register in ClubLog."));
    clubLogPasswordLineEdit->setToolTip(tr("Enter your password ClubLog here. Warning: The password will be save on clear in the KLog config file!! (If you don't want to enter the password, KLog will ask you when it is needed.)"));
    clubLogAppPasswordLineEdit->setToolTip(tr("Enter your application password ClubLog here. It can be obtained in ClubLog (Settings > App Passwords menu). Warning: The app password will be save on clear in the KLog config file!!"));

    QRegularExpression rx("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b",
                              QRegularExpression::CaseInsensitiveOption);
    clubLogEmailLineEdit->setValidator(new QRegularExpressionValidator(rx, this));

    clubLogSendInRealTimeCheckBox = new QCheckBox(tr("Send QSOs in real time"), this);
    clubLogActiveCheckBox = new QCheckBox(tr("Activate ClubLog"), this);
    clubLogSendInRealTimeCheckBox->setToolTip(tr("Send each QSO to ClubLog in real time, as they are added (or modified) in KLog."));
    clubLogActiveCheckBox->setToolTip(tr("Starts the ClubLog support in KLog."));
     //qDebug() << Q_FUNC_INFO << " -   00010";
    eQSLUserLineEdit = new QLineEdit;
    eQSLPasswordLineEdit = new QLineEdit;
    eQSLPasswordLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    eQSLActiveCheckBox = new QCheckBox(tr("Activate eQSL.cc"), this);
    eQSLUserLineEdit->setToolTip(tr("Enter your username of eQSL.cc."));
    eQSLPasswordLineEdit->setToolTip(tr("Enter your password eQSL.cc here. Warning: The password will be save on clear in the KLog config file!! (If you don't want to enter the password, KLog will ask you when it is needed.)"));
    //eQSLSendInRealTimeCheckBox = new QCheckBox(tr("Send QSOs in real time"), this);

    eQSLpasswordLabel = new QLabel(tr("eQSL.cc password"));
    eQSLemailLabel = new QLabel(tr("eQSL.cc user"));

    eQSLSentDefaultComboBox = new QComboBox;
    eQSLSentDefaultLabel = new QLabel("Default");
    eQSLSentDefaultLabel->setBuddy(eQSLSentDefaultComboBox);

    clubLogGroup = new QGroupBox (tr("ClubLog"));
    eQSLccGroup = new QGroupBox (tr("eQSL.cc"));
     //qDebug() << Q_FUNC_INFO << " -   00020";
    QGridLayout *cl1layout = new QGridLayout;
    cl1layout->addWidget(clubLogEmailLabel, 0, 0);
    cl1layout->addWidget(clubLogPasswordLabel, 1, 0);
    cl1layout->addWidget(clubLogAppPasswordLabel, 2, 0);
    cl1layout->addWidget(clubLogEmailLineEdit, 0, 1);
    cl1layout->addWidget(clubLogPasswordLineEdit, 1, 1);
    cl1layout->addWidget(clubLogAppPasswordLineEdit, 2, 1);

    clubLogSentDefaultLabel->setToolTip(tr("Select the default option for new QSOs in Clublog"));
    clubLogSentDefaultComboBox->setToolTip(tr("Select the default option for new QSOs in Clublog"));
    QHBoxLayout *clubLogSentDefaultLayout = new QHBoxLayout;
    clubLogSentDefaultLayout->addWidget(clubLogSentDefaultLabel);
    clubLogSentDefaultLayout->addWidget(clubLogSentDefaultComboBox);

    QHBoxLayout *clubLogTopLineLayout = new QHBoxLayout;
    clubLogTopLineLayout->addWidget(clubLogActiveCheckBox);
    clubLogTopLineLayout->addLayout(clubLogSentDefaultLayout);


    QVBoxLayout *cl2layout = new QVBoxLayout;
    //cl2layout->addWidget(clubLogActiveCheckBox);
    cl2layout->addLayout(clubLogTopLineLayout);
    cl2layout->addLayout(cl1layout);
    cl2layout->addWidget(clubLogSendInRealTimeCheckBox);
    clubLogGroup->setLayout(cl2layout);


    //qDebug() << Q_FUNC_INFO << " -   00030";
    QGridLayout *e1layout = new QGridLayout;
    e1layout->addWidget(eQSLemailLabel, 0, 0);
    e1layout->addWidget(eQSLpasswordLabel, 1, 0);
    e1layout->addWidget(eQSLUserLineEdit, 0, 1);
    e1layout->addWidget(eQSLPasswordLineEdit, 1, 1);

    QHBoxLayout *eQSLSentDefaultLayout = new QHBoxLayout;
    eQSLSentDefaultLayout->addWidget(eQSLSentDefaultLabel);
    eQSLSentDefaultLayout->addWidget(eQSLSentDefaultComboBox);

    eQSLSentDefaultLabel->setToolTip(tr("Select the default option for new QSOs in eQSL"));
    eQSLSentDefaultComboBox->setToolTip(tr("Select the default option for new QSOs in eQSL"));
    QHBoxLayout *eQSLTopLineLayout = new QHBoxLayout;
    eQSLTopLineLayout->addWidget(eQSLActiveCheckBox);
    eQSLTopLineLayout->addLayout(eQSLSentDefaultLayout);

    QVBoxLayout *e2layout = new QVBoxLayout;
    e2layout->addLayout(eQSLTopLineLayout);
    e2layout->addLayout(e1layout);
    //e2layout->addWidget(eQSLSendInRealTimeCheckBox);
    eQSLccGroup->setLayout(e2layout);
     //qDebug() << Q_FUNC_INFO << " -   00040";
    QRZCOMGroup = new QGroupBox (tr("QRZ.com"));
    QRZCOMSentDefaultComboBox = new QComboBox;
    QRZCOMSentDefaultLabel = new QLabel("Default");
    QRZCOMSentDefaultLabel->setBuddy(QRZCOMSentDefaultComboBox);

    QRZCOMUserLineEdit = new QLineEdit;
    QRZCOMUserLineEdit->setToolTip(tr("Enter the user of your QRZ.com account. You need to be subscribed to QRZ.com to use this service."));

    QRZCOMUserLabel = new QLabel(tr("User"));

    QRZCOMPasswordLineEdit = new QLineEdit;
    QRZCOMPasswordLineEdit->setToolTip(tr("Enter your password QRZ.com here. Warning: The password will be save on clear in the KLog config file!! (If you don't want to enter the password, KLog will ask you when it is needed.)"));
    QRZCOMPasswordLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);

    QRZCOMPasswordLabel = new QLabel(tr("Password"));
    QRZCOMActiveCheckBox = new QCheckBox(tr("Activate QRZ.com"), this);
    QRZCOMAutoCheckCheckBox = new QCheckBox(tr("Check automatically"), this);
    QRZCOMAutoCheckCheckBox->setToolTip(tr("Check in Qrz.com all Calls as they are entered"));
     //qDebug() << Q_FUNC_INFO << " -   00050";
    QRZCOMSubscriberCheckBox = new QCheckBox(tr("Paying Subscriber"), this);
    QRZCOMSubscriberCheckBox->setToolTip(tr("Check it if you are paying for a qrz.com subscription"));

    QRZLogBookKeyLabel = new QLabel(tr("LogBook Key"));
    QRZCOMLogBookKEYLineEdit = new QLineEdit;
    QRZCOMLogBookKEYLineEdit->setToolTip(tr("LogBook Key for QSO uploading. You can get this key in your QRZ.com logbook webpage. Remember that you need a QRZ.com subscription to use this feature."));


    QGridLayout *q1layout = new QGridLayout;
    q1layout->addWidget(QRZCOMUserLabel, 0, 0);
    q1layout->addWidget(QRZCOMPasswordLabel, 1, 0);
    q1layout->addWidget(QRZCOMUserLineEdit, 0, 1);
    q1layout->addWidget(QRZCOMPasswordLineEdit, 1, 1);
    q1layout->addWidget(QRZCOMSubscriberCheckBox,0,2);

    QHBoxLayout *qrzcomActive = new QHBoxLayout;
    qrzcomActive->addWidget(QRZCOMActiveCheckBox);
    qrzcomActive->addWidget(QRZCOMSubscriberCheckBox);

    QRZCOMSentDefaultLabel->setToolTip(tr("Select the default option for new QSOs in QRZ.com"));
    QRZCOMSentDefaultComboBox->setToolTip(tr("Select the default option for new QSOs in QRZ.com"));
    QHBoxLayout *QRZCOMSentDefaultLayout = new QHBoxLayout;
    QRZCOMSentDefaultLayout->addWidget(QRZCOMSentDefaultLabel);
    QRZCOMSentDefaultLayout->addWidget(QRZCOMSentDefaultComboBox);
    QHBoxLayout *QRZCOMTopLineLayout = new QHBoxLayout;
    QRZCOMTopLineLayout->addLayout(qrzcomActive);
    QRZCOMTopLineLayout->addLayout(QRZCOMSentDefaultLayout);


    QHBoxLayout *qrzcomKeyLayout = new QHBoxLayout;
    qrzcomKeyLayout->addWidget(QRZLogBookKeyLabel);
    qrzcomKeyLayout->addWidget(QRZCOMLogBookKEYLineEdit);


     //qDebug() << Q_FUNC_INFO << " -   00070";
    QVBoxLayout *qrzLayout = new QVBoxLayout;
    qrzLayout->addLayout(QRZCOMTopLineLayout);
    //qrzLayout->addLayout(qrzcomActive);
    qrzLayout->addLayout(q1layout);
    qrzLayout->addWidget(QRZCOMAutoCheckCheckBox);
    qrzLayout->addLayout(qrzcomKeyLayout);

    QRZCOMGroup->setLayout(qrzLayout);

     //qDebug() << Q_FUNC_INFO << " -   00080";

    lotwGroup = new QGroupBox (tr("LoTW"));
    lotwUpGroup = new QGroupBox (tr("Upload"));
    lotwDownGroup = new QGroupBox (tr("Download"));

    lotwUserLineEdit = new QLineEdit;
    lotwUserLineEdit->setValidator(new QRegularExpressionValidator(rx, this));
    lotwSentDefaultComboBox = new QComboBox;
    lotwSentDefaultLabel = new QLabel("Default");
    lotwSentDefaultLabel->setBuddy(lotwSentDefaultComboBox);

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

    lotwSentDefaultLabel->setToolTip(tr("Select the default option for new QSOs in LoTW"));
    lotwSentDefaultComboBox->setToolTip(tr("Select the default option for new QSOs in LoTW"));
    QHBoxLayout *lotwSentDefaultLayout = new QHBoxLayout;
    lotwSentDefaultLayout->addWidget(lotwSentDefaultLabel);
    lotwSentDefaultLayout->addWidget(lotwSentDefaultComboBox);
    QHBoxLayout *lotwTopLineLayout = new QHBoxLayout;
    lotwTopLineLayout->addWidget(lotwUseTQSLCheckBox);
    lotwTopLineLayout->addLayout(lotwSentDefaultLayout);

    QHBoxLayout *l1layout = new QHBoxLayout;
    l1layout->addWidget(lotwTQSLPathLineEdit);
    l1layout->addWidget(lotwSearchTQSLPushButton);
    //lotwUpGroup->setLayout(l1layout);

    QVBoxLayout *l2layout = new QVBoxLayout;
    l2layout->addLayout(lotwTopLineLayout);
    l2layout->addLayout(l1layout);
    lotwUpGroup->setLayout(l2layout);

    QGridLayout *l3layout = new QGridLayout;
    l3layout->addWidget(lotwemailLabel, 0, 0);
    l3layout->addWidget(lotwpasswordLabel, 1, 0);
    l3layout->addWidget(lotwUserLineEdit, 0, 1);
    l3layout->addWidget(lotwPasswordLineEdit, 1, 1);
    lotwDownGroup->setLayout(l3layout);
     //qDebug() << Q_FUNC_INFO << " -   00100";
    QVBoxLayout *llayout = new QVBoxLayout;
    //llayout->addLayout(l2layout);
    llayout->addWidget(lotwUpGroup);
    llayout->addWidget(lotwDownGroup);
    lotwGroup->setLayout(llayout);
     //qDebug() << Q_FUNC_INFO << " -   00110";
    QVBoxLayout *v1layout = new QVBoxLayout;
    v1layout->addWidget(clubLogGroup);
    v1layout->addWidget(eQSLccGroup);
     //qDebug() << Q_FUNC_INFO << " -   00120";
    QVBoxLayout *v2layout = new QVBoxLayout;
    v2layout->addWidget(QRZCOMGroup);
    v2layout->addWidget(lotwGroup);
     //qDebug() << Q_FUNC_INFO << " -   00130";
    QHBoxLayout *mlayout = new QHBoxLayout;
    mlayout->addLayout(v1layout);
    mlayout->addLayout(v2layout);
    setLayout(mlayout);
     //qDebug() << Q_FUNC_INFO << " -   00140";
    connect(clubLogActiveCheckBox, SIGNAL(toggled(bool) ), this, SLOT(slotClubLogActive(bool)));

    connect(clubLogPasswordLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(clubLogAppPasswordLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(clubLogEmailLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );

    connect(eQSLUserLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(eQSLPasswordLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(eQSLActiveCheckBox, SIGNAL(toggled(bool) ), this, SLOT(slotEQSLActive(bool)));
    connect(eQSLUserLineEdit, SIGNAL(textChanged(QString)), this, SLOT(sloteQSLCallTextChanged() ) );
     //qDebug() << Q_FUNC_INFO << " -   00150";
    connect(QRZCOMActiveCheckBox, SIGNAL(toggled(bool) ), this, SLOT(slotQRZCOMActive(bool)));
    //connect(QRZCOMAutoCheckCheckBox, SIGNAL(toggled(bool) ), this, SLOT(slotQRZCOMAuto(bool)));
    connect(QRZCOMUserLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(QRZCOMUserLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotQRZCallTextChanged() ) );
    connect(QRZCOMPasswordLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
     //qDebug() << Q_FUNC_INFO << " -   00160";

    connect(lotwUserLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(lotwPasswordLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(lotwTQSLPathLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(lotwSearchTQSLPushButton, SIGNAL(clicked()), this, SLOT(slotSelectTQSLClicked()) );
    connect(lotwTQSLPathLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotPathLineEditChanged(QString)) );
    connect(lotwUseTQSLCheckBox, SIGNAL(toggled(bool) ), this, SLOT(slotTQSLActive(bool)));

    connect(clubLogEmailLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotClubLogEmailDefineColor() ) );
    connect(lotwUserLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotLoTWEmailDefineColor() ) );

     //qDebug() << Q_FUNC_INFO << " -   00150";
    setDefaults();
    slotClubLogActive(false);
    setLoTWActive(false);
    slotTQSLActive(false);
    slotEQSLActive(false);
    slotQRZCOMActive(false);

     //qDebug() << Q_FUNC_INFO << " -   END";
}

SetupPageELog::~SetupPageELog()
{
    delete(util);
}


void SetupPageELog::setDefaultLoTW()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    Adif adif(Q_FUNC_INFO);
    lotwSentDefaultComboBox->addItems(adif.getQSLSentStatus());
    //qDebug() << Q_FUNC_INFO << " - Current-1: " << lotwSentDefaultComboBox->currentText();
    int index = lotwSentDefaultComboBox->findText("Q", Qt::MatchFixedString);
    lotwSentDefaultComboBox->setCurrentIndex(index >= 0 ? index : 0);
   //qDebug() << Q_FUNC_INFO << " - Current-2: " << lotwSentDefaultComboBox->currentText();


    lotwPasswordLineEdit->clear ();
    lotwUserLineEdit->clear ();
    lotwPasswordLineEdit->clear ();
   //qDebug() << Q_FUNC_INFO << " -   ";
}

void SetupPageELog::setDefaultClubLog()
{
    Adif adif(Q_FUNC_INFO);
    clubLogSentDefaultComboBox->addItems(adif.getQSOUploadStatus());
    int index = clubLogSentDefaultComboBox->findText("M", Qt::MatchFixedString);
    clubLogSentDefaultComboBox->setCurrentIndex(index >= 0 ? index : 0);

    clubLogActiveCheckBox->setChecked (false);
    clubLogSendInRealTimeCheckBox->setChecked (false);
    clubLogEmailLineEdit->clear ();
    clubLogPasswordLineEdit->clear();
    clubLogAppPasswordLineEdit->clear();
}

void SetupPageELog::setDefaultEQSL()
{
    Adif adif(Q_FUNC_INFO);
    eQSLSentDefaultComboBox->addItems(adif.getQSLSentStatus());
    int index = eQSLSentDefaultComboBox->findText("Q", Qt::MatchFixedString);
    eQSLSentDefaultComboBox->setCurrentIndex(index >= 0 ? index : 0);
    eQSLActiveCheckBox->setChecked(false);
    eQSLPasswordLineEdit->clear();
    eQSLUserLineEdit->clear();
}

void SetupPageELog::setDefaultQRZCOM()
{
    Adif adif(Q_FUNC_INFO);
    QRZCOMSentDefaultComboBox->addItems(adif.getQSOUploadStatus());
    int index = QRZCOMSentDefaultComboBox->findText("M", Qt::MatchFixedString);
    QRZCOMSentDefaultComboBox->setCurrentIndex(index >= 0 ? index : 0);

    QRZCOMActiveCheckBox->setChecked (false);
    QRZCOMAutoCheckCheckBox->setChecked (false);

    QRZCOMPasswordLineEdit->clear ();
    QRZCOMUserLineEdit->clear();
    QRZCOMLogBookKEYLineEdit->clear ();
}



void SetupPageELog::slotQRZCallTextChanged()
{
    //qDebug() << Q_FUNC_INFO << " -  ";
    int cursor = QRZCOMUserLineEdit->cursorPosition();
    //qDebug() << Q_FUNC_INFO << " -  -1";

    QString aux = util->getClearSQLi (QRZCOMUserLineEdit->text());
    //qDebug() << Q_FUNC_INFO << " -  -2";

    Callsign callsign(aux);
    if (callsign.isValid())
    {
        //qDebug() << Q_FUNC_INFO << " -  -2.1";
        QRZCOMUserLineEdit->setPalette(palBlack);
    }
    else
    {
        //qDebug() << Q_FUNC_INFO << " -  -2.2";
         QRZCOMUserLineEdit->setPalette(palRed);
    }

    //qDebug() << Q_FUNC_INFO << " -  -3";
    QRZCOMUserLineEdit->setText(aux.toUpper());
    //qDebug() << Q_FUNC_INFO << " -  -4";
    QRZCOMUserLineEdit->setCursorPosition(cursor);

    //qDebug() << Q_FUNC_INFO << " -   - END";
}

void SetupPageELog::sloteQSLCallTextChanged()
{
    //qDebug() << Q_FUNC_INFO << " - Start";
    int cursor = eQSLUserLineEdit->cursorPosition();

    QString aux = util->getClearSQLi (eQSLUserLineEdit->text());

    Callsign callsign(aux);
    if (callsign.isValid())
    {
        eQSLUserLineEdit->setPalette(palBlack);
    }
    else
    {
         eQSLUserLineEdit->setPalette(palRed);
    }

    eQSLUserLineEdit->setText(aux.toUpper());
    eQSLUserLineEdit->setCursorPosition(cursor);
    //qDebug() << Q_FUNC_INFO << " - END";
}

void SetupPageELog::slotClubLogEmailDefineColor()
{
    //qDebug() << Q_FUNC_INFO << " - Start"
    if(!clubLogEmailLineEdit->hasAcceptableInput())
        clubLogEmailLineEdit->setStyleSheet("QLineEdit { color: red;}");
    else
        clubLogEmailLineEdit->setStyleSheet("QLineEdit { color: black;}");
}

void SetupPageELog::slotLoTWEmailDefineColor()
{
    //qDebug() << Q_FUNC_INFO << ": " << lotwUserLineEdit->text();
    int cursor = lotwUserLineEdit->cursorPosition();
    QString aux = lotwUserLineEdit->text();
    Callsign callsign(aux);
    if (callsign.isValid())
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
     //qDebug() << Q_FUNC_INFO << " - Start"

    setDefaultClubLog();
    setDefaultEQSL();
    setDefaultLoTW();
    setDefaultQRZCOM();

     //qDebug() << Q_FUNC_INFO << " - END"
}


void SetupPageELog::setClubLogActive(const bool &_b)
{
    clubLogActiveCheckBox->setChecked(_b);
    clubLogSendInRealTimeCheckBox->setEnabled(_b);
}

void SetupPageELog::slotClubLogActive(const bool _s)
{
     //qDebug() << Q_FUNC_INFO << " - " << util->boolToQString(_s);
    clubLogEmailLabel->setEnabled(_s);
    clubLogPasswordLabel->setEnabled(_s);
    clubLogEmailLineEdit->setEnabled(_s);
    clubLogPasswordLineEdit->setEnabled(_s);
    clubLogAppPasswordLineEdit->setEnabled(_s);
    clubLogSendInRealTimeCheckBox->setEnabled(_s);
     //qDebug() << "SetupPageELog::slotClubLogActive";
}

 // END of CLubLog

 // Start of eQSL.CC


void SetupPageELog::slotEQSLActive(const bool _s)
{
     //qDebug() << Q_FUNC_INFO << " - "  << util->boolToQString(_s);
    eQSLemailLabel->setEnabled(_s);
    eQSLpasswordLabel->setEnabled(_s);
    eQSLUserLineEdit->setEnabled(_s);
    eQSLPasswordLineEdit->setEnabled(_s);
}

void SetupPageELog::slotTQSLActive(const bool _s)
{
     //qDebug() << Q_FUNC_INFO << " - " << util->boolToQString(_s);
    setLoTWActive(_s);
}

 // END of eQSL.CC

 // Start of LoTW


void SetupPageELog::setLoTWActive(const bool &_s)
{
    lotwUseTQSLCheckBox->setChecked(_s);
    lotwTQSLPathLineEdit->setEnabled(_s);
    lotwSearchTQSLPushButton->setEnabled(_s);
}

void SetupPageELog::slotSelectTQSLClicked()
{
       //qDebug() << Q_FUNC_INFO << " - " << QStandardPaths::ApplicationsLocation;
    QString appsDir= util->getTQSLsPath();
    //QString proposedName = util->getTQSLsFileName();

    QString filter;
    filter.clear();
    #if defined(Q_OS_WIN)
        filter = "TQSL (*.exe)";
    #elif defined(Q_OS_MACOS)
        filter = "TQSL (*.app)";
    #else
        filter = "TQSL (tqsl)";
        //filter = "TQSL (tqsl*)";
    #endif
    QString tqslFile;
    tqslFile.clear();
    tqslFile = QFileDialog::getOpenFileName(this, tr("Select File"), appsDir, filter);

    if (!tqslFile.isEmpty())
    {
        lotwTQSLPathLineEdit->setText(tqslFile);
    }
        //qDebug() << Q_FUNC_INFO << " - END";
}

void SetupPageELog::slotPathLineEditChanged(const QString &_q)
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

 // END of LoTW


void SetupPageELog::slotEnterKeyPressed()
{
    emit enterKey();
}

// QRZ.COM

void SetupPageELog::setQRZCOMActive(const bool _s)
{
     //qDebug() << Q_FUNC_INFO << " - Start";
    QRZCOMActiveCheckBox->setChecked(_s);
    QRZCOMAutoCheckCheckBox->setEnabled(_s);
}

void SetupPageELog::slotQRZCOMActive(bool _s)
{
     //qDebug() << Q_FUNC_INFO << " - "  << util->boolToQString(_s);
    QRZCOMUserLabel->setEnabled(_s);
     //qDebug() << Q_FUNC_INFO << " -   1";
    QRZCOMUserLineEdit->setEnabled(_s);
     //qDebug() << Q_FUNC_INFO << " -   2";
    QRZCOMPasswordLabel->setEnabled(_s);
     //qDebug() << Q_FUNC_INFO << " -   3";
    QRZCOMPasswordLineEdit->setEnabled(_s);
    QRZCOMAutoCheckCheckBox->setEnabled(_s);
     //qDebug() << Q_FUNC_INFO << " -   END";
    //if (!QRZCOMActiveCheckBox->isChecked())
    //{
    //    QRZCOMAutoCheckCheckBox->setChecked(false);
   // }
}

//void SetupPageELog::slotQRZCOMAuto(const bool _s)
//{
//    emit qrzcomAuto(_s);
//}

void SetupPageELog::setQRZCOMAutoCheck(const bool _s)
{
    QRZCOMAutoCheckCheckBox->setChecked(_s);
}

void SetupPageELog::showEvent(QShowEvent *event)
{
    //clubLogSendInRealTimeCheckBox->setChecked(clubLogRealTime);
     //qDebug() << Q_FUNC_INFO << ": " << util->boolToQString (clubLogSendInRealTimeCheckBox->isChecked ());
    //clubLogActiveCheckBox->setChecked(clubLogActive);
    //clubLogPasswordLineEdit->setText(clubLogPass);
    //clubLogEmailLineEdit->setText(clubLogEmail);

    //QRZCOMUserLineEdit->setText(qrzComUser);
    //QRZCOMPasswordLineEdit->setText(qrzcomPass);
    //QRZCOMActiveCheckBox->setChecked(qrzcomActive);
   // QRZCOMAutoCheckCheckBox->setChecked(qrzcomAutoFill);
    //QRZCOMLogBookKEYLineEdit->setText(qrcomLogbookKey);

    //eQSLUserLineEdit->setText(eqslUser);
    //eQSLPasswordLineEdit->setText(eqslPass);
   // eQSLActiveCheckBox->setChecked(eqslActive);

    //lotwUserLineEdit->setText(lotwUser);
    //lotwPasswordLineEdit->setText(lotwPass);
    //lotwUseTQSLCheckBox->setChecked(lotwTQSL);
    //lotwTQSLPathLineEdit->setText(lotwPath);

    event->accept();
}


void SetupPageELog::saveSettings()
{
    //qDebug() << Q_FUNC_INFO ;
    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    //settings.beginGroup ("eLogs");

    settings.beginGroup ("ClubLog");
    settings.setValue ("ClubLogActive", QVariant((clubLogActiveCheckBox->isChecked())));
    settings.setValue ("ClubLogRealTime", QVariant((clubLogSendInRealTimeCheckBox->isChecked())));
    settings.setValue ("ClubLogEmail", clubLogEmailLineEdit->text ());
    settings.setValue ("ClubLogPass", clubLogPasswordLineEdit->text());
    settings.setValue ("ClubLogAppPass", clubLogAppPasswordLineEdit->text());
    settings.setValue ("ClubLogSentDefault", clubLogSentDefaultComboBox->currentText());
    settings.endGroup ();

    settings.beginGroup ("eQSL");
    settings.setValue ("eQSLActive", QVariant((eQSLActiveCheckBox->isChecked())));
    settings.setValue ("eQSLCall", eQSLUserLineEdit->text ());
    settings.setValue ("eQSLPass", eQSLPasswordLineEdit->text());
    settings.setValue ("eQSLSentDefault", eQSLSentDefaultComboBox->currentText());
    settings.endGroup ();

    settings.beginGroup ("QRZcom");
    settings.setValue ("QRZcomActive", QVariant((QRZCOMActiveCheckBox->isChecked())));
    settings.setValue ("QRZcomUser", QRZCOMUserLineEdit->text ());
    settings.setValue ("QRZcomPass", QRZCOMPasswordLineEdit->text());
    settings.setValue ("QRZcomSubscriber", QVariant((QRZCOMSubscriberCheckBox->isChecked())));
    settings.setValue ("QRZcomAuto", QVariant((QRZCOMAutoCheckCheckBox->isChecked())));
    settings.setValue ("QRZcomLogBookKey", QRZCOMLogBookKEYLineEdit->text ());
    settings.setValue ("QRZcomSentDefault", QRZCOMSentDefaultComboBox->currentText());
    settings.endGroup ();

    settings.beginGroup ("LoTW");
    settings.setValue ("LoTWActive", QVariant((lotwUseTQSLCheckBox->isChecked())));
    settings.setValue ("LoTWPath", lotwTQSLPathLineEdit->text());
    settings.setValue ("LoTWUser", lotwUserLineEdit->text());
    settings.setValue ("LoTWPass", lotwPasswordLineEdit->text());
    settings.setValue ("LoTWSentDefault", lotwSentDefaultComboBox->currentText());
    settings.endGroup ();
}

void SetupPageELog::loadSettings()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("ClubLog");
    clubLogActiveCheckBox->setChecked (settings.value("ClubLogActive").toBool ());
    clubLogSendInRealTimeCheckBox->setChecked (settings.value("ClubLogRealTime").toBool ());
     //qDebug() << Q_FUNC_INFO << ": realTime: " << util->boolToQString (clubLogSendInRealTimeCheckBox->isChecked ());
    clubLogEmailLineEdit->setText (settings.value("ClubLogEmail").toString ());
    clubLogPasswordLineEdit->setText (settings.value("ClubLogPass").toString ());
    clubLogAppPasswordLineEdit->setText (settings.value("ClubLogAppPass").toString ());
    int indexBackup = clubLogSentDefaultComboBox->findText("M", Qt::MatchFixedString);
    int index = clubLogSentDefaultComboBox->findText(settings.value("ClubLogSentDefault").toString(), Qt::MatchFixedString);
    clubLogSentDefaultComboBox->setCurrentIndex(index >= 0 ? index : indexBackup);
    settings.endGroup ();

    settings.beginGroup ("eQSL");
    eQSLActiveCheckBox->setChecked (settings.value("eQSLActive", false).toBool ());
    eQSLUserLineEdit->setText (settings.value("eQSLCall").toString ());
    eQSLPasswordLineEdit->setText (settings.value("eQSLPass").toString ());
    indexBackup = eQSLSentDefaultComboBox->findText("Q", Qt::MatchFixedString);
    index = eQSLSentDefaultComboBox->findText(settings.value("eQSLSentDefault", "Q").toString(), Qt::MatchFixedString);
    eQSLSentDefaultComboBox->setCurrentIndex(index >= 0 ? index : indexBackup);

    settings.endGroup ();

    settings.beginGroup ("QRZcom");
    QRZCOMActiveCheckBox->setChecked (settings.value("QRZcomActive", false).toBool ());
    QRZCOMSubscriberCheckBox->setChecked (settings.value("QRZcomSubscriber").toBool ());
    QRZCOMAutoCheckCheckBox->setChecked (settings.value("QRZcomAuto", false).toBool ());
    QRZCOMUserLineEdit->setText (settings.value("QRZcomUser").toString ());
    QRZCOMPasswordLineEdit->setText (settings.value("QRZcomPass").toString ());
    QRZCOMLogBookKEYLineEdit->setText(settings.value("QRZcomLogBookKey").toString ());
    index = QRZCOMSentDefaultComboBox->findText(settings.value("QRZcomSentDefault", "M").toString(), Qt::MatchFixedString);
    indexBackup = QRZCOMSentDefaultComboBox->findText("M", Qt::MatchFixedString);
    QRZCOMSentDefaultComboBox->setCurrentIndex(index >= 0 ? index : indexBackup);
    settings.endGroup ();

    settings.beginGroup ("LoTW");
    lotwUseTQSLCheckBox->setChecked (settings.value("LoTWActive", false).toBool ());
    lotwUserLineEdit->setText (settings.value("LoTWUser").toString ());
    //qDebug() << Q_FUNC_INFO << ": LoTWUser: " << lotwUserLineEdit->text ();
    lotwPasswordLineEdit->setText (settings.value("LoTWPass").toString ());
    lotwTQSLPathLineEdit->setText (settings.value("LoTWPath").toString ());
    indexBackup = lotwSentDefaultComboBox->findText("Q", Qt::MatchFixedString);
    index = lotwSentDefaultComboBox->findText(settings.value("LoTWSentDefault", "Q").toString(), Qt::MatchFixedString);
    lotwSentDefaultComboBox->setCurrentIndex(index >= 0 ? index : indexBackup);
    settings.endGroup ();
}
