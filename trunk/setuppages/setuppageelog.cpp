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

    clubLogEmail = QString();
    clubLogPass = QString();

    clubLogEmailLineEdit = new QLineEdit;
    clubLogPasswordLineEdit = new QLineEdit;
    clubLogPasswordLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);


    clubLogPasswordLabel = new QLabel(tr("ClubLog password"));
    clubLogEmailLabel = new QLabel(tr("ClubLog email"));
    clubLogPasswordLabel->setBuddy(clubLogPasswordLineEdit);
    clubLogEmailLabel->setBuddy(clubLogEmailLineEdit);

    clubLogEmailLineEdit->setToolTip(tr("Enter the email you used to register in ClubLog."));
    clubLogPasswordLineEdit->setToolTip(tr("Enter your password ClubLog here. Warning: The password will be save on clear in the KLog config file!! (If you don't want to enter the password, KLog will ask you when it is needed.)"));

    clubLogSendInRealTimeCheckBox = new QCheckBox(tr("Send QSOs in real time"), this);
    clubLogActiveCheckBox = new QCheckBox(tr("Activate ClubLog"), this);
    clubLogSendInRealTimeCheckBox->setToolTip(tr("Send each QSO to ClubLog in real time, as they are added (or modified) in KLog."));
    clubLogActiveCheckBox->setToolTip(tr("Starts the ClubLog support in KLog."));

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

    lotwGroup = new QGroupBox (tr("LoTW"));
    lotwUpGroup = new QGroupBox (tr("Upload"));
    lotwDownGroup = new QGroupBox (tr("Download"));

    lotwUserLineEdit = new QLineEdit;
    lotwPasswordLineEdit = new QLineEdit;
    lotwPasswordLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    lotwTQSLPathLineEdit = new QLineEdit;
    lotwSearchTQSLPushButton = new QPushButton(tr("TQSL path"), this);
    lotwUseTQSLCheckBox = new QCheckBox(tr("Use TQSL"), this);
    lotwpasswordLabel = new QLabel(tr("LoTW password"));
    lotwemailLabel = new QLabel(tr("LoTW email"));

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

    QVBoxLayout *llayout = new QVBoxLayout;
    //llayout->addLayout(l2layout);
    llayout->addWidget(lotwUpGroup);
    llayout->addWidget(lotwDownGroup);
    lotwGroup->setLayout(llayout);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(clubLogGroup);
    layout->addWidget(eQSLccGroup);

    QHBoxLayout *mlayout = new QHBoxLayout;
    mlayout->addLayout(layout);
    mlayout->addWidget(lotwGroup);
    setLayout(mlayout);

    connect(clubLogActiveCheckBox, SIGNAL(toggled(bool) ), this, SLOT(slotClubLogActive(bool)));

    connect(clubLogPasswordLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(clubLogEmailLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );

    connect(eQSLUserLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(eQSLPasswordLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(eQSLActiveCheckBox, SIGNAL(toggled(bool) ), this, SLOT(slotEQSLActive(bool)));

    connect(lotwUserLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(lotwPasswordLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(lotwTQSLPathLineEdit, SIGNAL(returnPressed()), this, SLOT(slotEnterKeyPressed() ) );
    connect(lotwSearchTQSLPushButton, SIGNAL(clicked()), this, SLOT(slotSelectTQSLClicked()) );
    connect(lotwTQSLPathLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotPathLineEditChanged(QString)) );
    connect(lotwUseTQSLCheckBox, SIGNAL(toggled(bool) ), this, SLOT(slotTQSLActive(bool)));

    slotClubLogActive(false);
    slotTQSLActive(false);
    slotEQSLActive(false);


       //qDebug() << "SetupPageELog::SetupPageELog - END" << endl;
}

SetupPageELog::~SetupPageELog()
{
}

void SetupPageELog::setClubLogEmail(const QString &c)
{
    clubLogEmailLineEdit->setText(c);
}

void SetupPageELog::setClubLogPassword(const QString &c)
{
    clubLogPasswordLineEdit->setText(c);
}

QString SetupPageELog::getClubLogEmail()
{
    return (clubLogEmailLineEdit->text());
}

QString SetupPageELog::getClubLogPassword()
{
    return clubLogPasswordLineEdit->text();
}

QString SetupPageELog::getClubLogActive()
{
    if (clubLogActiveCheckBox->isChecked() )
    {
        return "True";
    }
    else
    {
        return "False";
    }
}

void SetupPageELog::setClubLogActive(const QString &_s)
{
    if ( (_s.toUpper()) == "FALSE")
    {
        clubLogActiveCheckBox->setChecked(false);
    }
    else
    {
        clubLogActiveCheckBox->setChecked(true);
    }
}

void SetupPageELog::slotClubLogActive(bool _s)
{
      //qDebug() << "SetupPageELog::slotClubLogActive" << endl;
    if (_s)
    {
          //qDebug() << "SetupPageELog::slotClubLogActive TRUE" << endl;
        clubLogEmailLabel->setEnabled(true);
        clubLogPasswordLabel->setEnabled(true);
        clubLogEmailLineEdit->setEnabled(true);
        clubLogPasswordLineEdit->setEnabled(true);
        clubLogSendInRealTimeCheckBox->setEnabled(true);
    }
    else
    {
          //qDebug() << "SetupPageELog::slotClubLogActive FALSE" << endl;
        clubLogEmailLabel->setEnabled(false);
        clubLogPasswordLabel->setEnabled(false);
        clubLogEmailLineEdit->setEnabled(false);
        clubLogPasswordLineEdit->setEnabled(false);
        clubLogSendInRealTimeCheckBox->setEnabled(false);
    }
}

void SetupPageELog::setClubLogRealTime(const QString &_s)
{
    if ( (_s.toUpper()) == "FALSE")
    {
        clubLogSendInRealTimeCheckBox->setChecked(false);
    }
    else
    {
        clubLogSendInRealTimeCheckBox->setChecked(true);
    }
}

QString SetupPageELog::getClubLogRealTime()
{
    if (clubLogSendInRealTimeCheckBox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }
}

 // END of CLubLog

 // Start of eQSL.CC

void SetupPageELog::setEQSLEmail(const QString &c)
{
    eQSLUserLineEdit->setText(c);
}

void SetupPageELog::setEQSLPassword(const QString &c)
{
    eQSLPasswordLineEdit->setText(c);
}

QString SetupPageELog::getEQSLEmail()
{
    return (eQSLUserLineEdit->text());
}

QString SetupPageELog::getEQSLPassword()
{
    return eQSLPasswordLineEdit->text();
}

QString SetupPageELog::getEQSLActive()
{
    if (eQSLActiveCheckBox->isChecked() )
    {
        return "True";
    }
    else
    {
        return "False";
    }
}

void SetupPageELog::setEQSLActive(const QString &_s)
{
    if ( (_s.toUpper()) == "FALSE")
    {
        eQSLActiveCheckBox->setChecked(false);
    }
    else
    {
        eQSLActiveCheckBox->setChecked(true);
    }
}

void SetupPageELog::slotEQSLActive(bool _s)
{
    //qDebug() << "SetupPageELog::slotEQSLActive" << endl;
    if (_s)
    {
        //qDebug() << "SetupPageELog::slotEQSLActive TRUE" << endl;
        eQSLemailLabel->setEnabled(true);
        eQSLpasswordLabel->setEnabled(true);
        eQSLUserLineEdit->setEnabled(true);
        eQSLPasswordLineEdit->setEnabled(true);
        //eQSLSendInRealTimeCheckBox->setEnabled(true);
    }
    else
    {
        //qDebug() << "SetupPageELog::slotEQSLActive FALSE" << endl;
        eQSLemailLabel->setEnabled(false);
        eQSLpasswordLabel->setEnabled(false);
        eQSLUserLineEdit->setEnabled(false);
        eQSLPasswordLineEdit->setEnabled(false);
        //eQSLSendInRealTimeCheckBox->setEnabled(false);
    }
}

void SetupPageELog::slotTQSLActive(bool _s)
{
    //qDebug() << "SetupPageELog::slotTQSLActive" << endl;
    if (_s)
    {
        //qDebug() << "SetupPageELog::slotTQSLActive TRUE" << endl;
        lotwTQSLPathLineEdit->setEnabled(true);
        lotwSearchTQSLPushButton->setEnabled(true);
    }
    else
    {
        //qDebug() << "SetupPageELog::slotTQSLActive FALSE" << endl;
        lotwTQSLPathLineEdit->setEnabled(false);
        lotwSearchTQSLPushButton->setEnabled(false);
    }
}
/*
void SetupPageELog::setEQSLRealTime(const QString &_s)
{
    if ( (_s.toUpper()) == "FALSE")
    {
        eQSLSendInRealTimeCheckBox->setChecked(false);
    }
    else
    {
        eQSLSendInRealTimeCheckBox->setChecked(true);
    }
}

QString SetupPageELog::getEQSLRealTime()
{
    if (eQSLSendInRealTimeCheckBox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }
}
*/

 // END of eQSL.CC

 // Start of LoTW


void SetupPageELog::setLoTWActive(const QString &_s)
{
    if ( (_s.toUpper()) == "FALSE")
    {
        lotwUseTQSLCheckBox->setChecked(false);
    }
    else
    {
        lotwUseTQSLCheckBox->setChecked(true);
    }
}

QString SetupPageELog::getLoTWActive()
{
    if (lotwUseTQSLCheckBox->isChecked() )
    {
        return "True";
    }
    else
    {
        return "False";
    }
}

void SetupPageELog::setTQSLPath(const QString &c)
{
    lotwTQSLPathLineEdit->setText(c);
}

QString SetupPageELog::getTQSLPath()
{
    return lotwTQSLPathLineEdit->text();
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
    QPalette palRed;
    palRed.setColor(QPalette::Text, Qt::red);
    QPalette palBlack;
    palBlack.setColor(QPalette::Text, Qt::black);
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
    lotwUserLineEdit->setText(_s);
}

QString SetupPageELog::getLoTWUser()
{
    return lotwUserLineEdit->text();
}

void SetupPageELog::setLoTWPass(const QString &_s)
{
    lotwPasswordLineEdit->setText(_s);
}

QString SetupPageELog::getLoTWPass()
{
    return lotwPasswordLineEdit->text();
}


 // END of LoTW


void SetupPageELog::slotEnterKeyPressed()
{
    emit enterKey();
}
