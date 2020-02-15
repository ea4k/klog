/***************************************************************************
                          setuppageclublog.cpp  -  description
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

//#include <QDebug>

#include "setuppageclublog.h"


SetupPageClubLog::SetupPageClubLog(QWidget *parent) : QWidget(parent)
{
     //qDebug() << "SetupPageClubLog::SetupPageClubLog" << endl;
    clubLogActive = false;
    call = QString();
    email = QString();
    pass = QString();

    emailLineEdit = new QLineEdit;
    callLineEdit = new QLineEdit;
    passwordLineEdit = new QLineEdit;


    callLabel = new QLabel(tr("&Callsign"));
    passwordLabel = new QLabel(tr("ClubLog &password"));
    emailLabel = new QLabel(tr("ClubLog &email"));

    callLabel->setBuddy(callLineEdit);
    passwordLabel->setBuddy(passwordLineEdit);
    emailLabel->setBuddy(emailLineEdit);

    emailLineEdit->setToolTip(tr("Enter the email you used to register in ClubLog."));
    callLineEdit->setToolTip(tr("Enter the callsign you used to register in ClubLog."));
    passwordLineEdit->setToolTip(tr("Enter your password in ClubLog."));


    //passwordLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);

    sendInRealTimeCheckBox = new QCheckBox(tr("&Send QSOs in real time"), this);
    clubLogActiveCheckBox = new QCheckBox(tr("&Activate ClubLog"), this);
    useQSOStationCallCheckBox = new QCheckBox(tr("Use QSO Station &Callsign"), this);
    sendInRealTimeCheckBox->setToolTip(tr("Send each QSO to ClubLog in real time, as they are added (or modified) in KLog."));
    clubLogActiveCheckBox->setToolTip(tr("Starts the ClubLog support in KLog."));
    useQSOStationCallCheckBox->setToolTip(tr("Use the Station Callsign defined in each QSO instead of the one defined here."));

    QHBoxLayout *callSLayout = new QHBoxLayout;
    callSLayout->addWidget(callLineEdit);
    callSLayout->addWidget(useQSOStationCallCheckBox);


    QGridLayout *glayout = new QGridLayout;

    glayout->addWidget(callLabel, 0, 0);
    glayout->addWidget(emailLabel, 1, 0);
    glayout->addWidget(passwordLabel, 2, 0);
    glayout->addLayout(callSLayout, 0, 1);
    //glayout->addWidget(callLineEdit, 0, 1);
    glayout->addWidget(emailLineEdit, 1, 1);
    glayout->addWidget(passwordLineEdit, 2, 1);

    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(clubLogActiveCheckBox);
    layout->addLayout(glayout);
    layout->addWidget(sendInRealTimeCheckBox);


    setLayout(layout);

    //connect(newOneColorButton, SIGNAL(clicked()), this, SLOT(slotNewOneColorButtonClicked()) );

    connect(clubLogActiveCheckBox, SIGNAL(toggled(bool) ), this, SLOT(slotClubLogActive(bool)));
    connect(useQSOStationCallCheckBox, SIGNAL(toggled(bool) ), this, SLOT(slotUseStationCall(bool)));
    //connect(sendInRealTimeCheckBox, SIGNAL(toggled(bool) ), this, SLOT(slotClubLogActive(bool)));
    slotClubLogActive(clubLogActive);
     //qDebug() << "SetupPageClubLog::SetupPageClubLog - END" << endl;

}

SetupPageClubLog::~SetupPageClubLog()
{
}

void SetupPageClubLog::setEmail(const QString c)
{
    emailLineEdit->setText(c);
}

void SetupPageClubLog::setPassword(const QString c)
{
    passwordLineEdit->setText(c);
}

void SetupPageClubLog::setCallsign(const QString c)
{
    callLineEdit->setText(c.toUpper());
}


QString SetupPageClubLog::getEmail()
{
    return (emailLineEdit->text());
}

QString SetupPageClubLog::getPassword()
{
    return passwordLineEdit->text();
}

QString SetupPageClubLog::getCallsign()
{
    return (callLineEdit->text()).toUpper();
}







QString SetupPageClubLog::getUseQSOStationCallsign()
{
    if (useQSOStationCallCheckBox->isChecked() )
    {
        return "True";
    }
    else
    {
        return "False";
    }
}


void SetupPageClubLog::setUseStationCall(const QString _s)
{
    if ( (_s.toUpper()) == "FALSE")
    {
        useQSOStationCallCheckBox->setChecked(false);
    }
    else
    {
        useQSOStationCallCheckBox->setChecked(true);
    }
}











QString SetupPageClubLog::getClubLog()
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


void SetupPageClubLog::setClubLog(const QString _s)
{
    if ( (_s.toUpper()) == "FALSE")
    {
        clubLogActive = false;
        clubLogActiveCheckBox->setChecked(false);
    }
    else
    {
        clubLogActive = true;
        clubLogActiveCheckBox->setChecked(true);
    }
}

void SetupPageClubLog::slotUseStationCall(bool _s)
{
    //qDebug() << "SetupPageClubLog::slotUseStationCall" << endl;
    if (useQSOStationCallCheckBox->isChecked())
    {
        callLineEdit->setEnabled(false);
        callLabel->setEnabled(false);
    }
    else
    {
        callLineEdit->setEnabled(true);
        callLabel->setEnabled(true);
    }

}

void SetupPageClubLog::slotClubLogActive(bool _s)
{
    //qDebug() << "SetupPageClubLog::slotClubLogActive" << endl;
    if (_s)
    {
        //qDebug() << "SetupPageClubLog::slotClubLogActive TRUE" << endl;
        callLabel->setEnabled(true);
        emailLabel->setEnabled(true);
        passwordLabel->setEnabled(true);
        callLineEdit->setEnabled(true);
        emailLineEdit->setEnabled(true);
        passwordLineEdit->setEnabled(true);
        sendInRealTimeCheckBox->setEnabled(true);
        useQSOStationCallCheckBox->setEnabled(true);
        clubLogActive = true;
    }
    else
    {
        //qDebug() << "SetupPageClubLog::slotClubLogActive FALSE" << endl;
        callLabel->setEnabled(false);
        emailLabel->setEnabled(false);
        passwordLabel->setEnabled(false);
        callLineEdit->setEnabled(false);
        emailLineEdit->setEnabled(false);
        passwordLineEdit->setEnabled(false);
        sendInRealTimeCheckBox->setEnabled(false);
        useQSOStationCallCheckBox->setEnabled(false);
        clubLogActive = false;
    }

}

void SetupPageClubLog::setClubLogRealTime(const QString _s)
{
    if ( (_s.toUpper()) == "FALSE")
    {

        sendInRealTimeCheckBox->setChecked(false);
    }
    else
    {
        sendInRealTimeCheckBox->setChecked(true);
    }
}

QString SetupPageClubLog::getClubLogRealTime()
{
    if (sendInRealTimeCheckBox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }
    //return sendInRealTimeCheckBox->isChecked();
}


