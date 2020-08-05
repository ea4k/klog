/***************************************************************************
                          SetupPageLoTW.cpp  -  description
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

#include "setuppages/setuppagelotw.h"


SetupPageLoTW::SetupPageLoTW(QWidget *parent) : QWidget(parent)
{
     //qDebug() << "SetupPageLoTW::SetupPageLoTW" << endl;

    util = new Utilities;

    lotwActiveCheckBox = new QCheckBox;
    tqslPathLineEdit = new QLineEdit;
    lotwUserLineEdit = new QLineEdit;
    lotwPassLineEdit = new QLineEdit;
    pathLabel = new QLabel;
    searchTQSLPushButton = new QPushButton;
    //testPushButton = new QPushButton;


   createUI();
    //qDebug() << "SetupPageLoTW::SetupPageLoTW-END" << endl;
    //connect(clubLogActiveCheckBox, SIGNAL(toggled(bool) ), this, SLOT(slotClubLogActive(bool)));
}

SetupPageLoTW::~SetupPageLoTW()
{
}

void SetupPageLoTW::createUI()
{

    QGroupBox *uploadGroupBox = new QGroupBox(tr("LoTW upload"));
    QHBoxLayout *uhbox = new QHBoxLayout;
    uhbox->addWidget(lotwActiveCheckBox);
    uhbox->addWidget(pathLabel);
    uhbox->addWidget(tqslPathLineEdit);
    uhbox->addWidget(searchTQSLPushButton);
    uhbox->addStretch(1);
    uploadGroupBox->setLayout(uhbox);

    searchTQSLPushButton->setText(tr("TQSL Path"));
    pathLabel = new QLabel;
    pathLabel->setText(tr("Route to TQSL"));
    pathLabel->setBuddy(tqslPathLineEdit);


    // Upload Tooltips
    lotwActiveCheckBox->setText(tr("Use TQSL"));
    tqslPathLineEdit->setToolTip(tr("Path to the TQSL software."));
    lotwActiveCheckBox->setToolTip(tr("Enable the LoTW integration with TQSL. You will need to have TQSL installed"));


    // Download
    lotwPassLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    QLabel *userlabel = new QLabel;
    QLabel *passlabel = new QLabel;
    userlabel->setText(tr("User:"));
    passlabel->setText(tr("Password:"));

    QTextEdit *warningTextEdit = new QTextEdit;
    warningTextEdit->setAlignment(Qt::AlignJustify);
    warningTextEdit->setReadOnly(true);
    //QLabel *warninglabel = new QLabel;
    warningTextEdit->setText(tr("Enter your password LoTW here. Warning: The password will be save on clear in the KLog config file!! If you don't want to enter the password, KLog will ask you when it is needed."));

    QGroupBox *downloadGroupBox = new QGroupBox(tr("LoTW download"));
    QGridLayout *dhbox = new QGridLayout;
    dhbox->addWidget(userlabel, 0, 0);
    dhbox->addWidget(lotwUserLineEdit, 0, 1);
    dhbox->addWidget(passlabel, 1, 0);
    dhbox->addWidget(lotwPassLineEdit, 1, 1);
    dhbox->addWidget(warningTextEdit, 2, 0, 2, -1);
    //dhbox->addWidget(QLayoutItem::spacerItem(this), 0, 1, -1, 3);

    //dhbox->addWidget(testPushButton, 0, 2);

    downloadGroupBox->setLayout(dhbox);

    // Download Tooltips
    userlabel->setToolTip(tr("Enter your LoTW user"));
    lotwUserLineEdit->setToolTip(tr("Enter your LoTW user"));
    passlabel->setToolTip(tr("Enter your password LoTW here. Warning: The password will be save on clear in the KLog config file!! (If you don't want to enter the password, KLog will ask you when it is needed.)"));
    lotwPassLineEdit->setToolTip(tr("Enter your password LoTW here. Warning: The password will be save on clear in the KLog config file!! (If you don't want to enter the password, KLog will ask you when it is needed.)"));
    //testPushButton->setToolTip(tr("Push this button once you have entered username and password to test the connection to LoTW"));
    // Main widget
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(uploadGroupBox);
    mainLayout->addWidget(downloadGroupBox);
    setLayout(mainLayout);
    /*
    QHBoxLayout *pathLayout = new QHBoxLayout;
    pathLayout->addWidget(lotwActiveCheckBox);
    pathLayout->addWidget(pathLabel);
    pathLayout->addWidget(tqslPathLineEdit);
    pathLayout->addWidget(searchTQSLPushButton);

    setLayout(pathLayout);
    */

    connect(searchTQSLPushButton, SIGNAL(clicked()), this, SLOT(slotSelectTQSLClicked()) );
    connect(tqslPathLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotPathLineEditChanged(QString)) );
}

void SetupPageLoTW::setLoTW(const QString &_s)
{
    if ( (_s.toUpper()) == "FALSE")
    {
        lotwActiveCheckBox->setChecked(false);
    }
    else
    {
        lotwActiveCheckBox->setChecked(true);
    }
}

QString SetupPageLoTW::getLoTW()
{
    if (lotwActiveCheckBox->isChecked() )
    {
        return "True";
    }
    else
    {
        return "False";
    }
}

void SetupPageLoTW::setPath(const QString &c)
{
    tqslPathLineEdit->setText(c);
}

QString SetupPageLoTW::getPath()
{
    return tqslPathLineEdit->text();
}

void SetupPageLoTW::slotSelectTQSLClicked()
{
      //qDebug() << "SetupPageLoTWr::slotSelectTQSLClicked: " << QStandardPaths::ApplicationsLocation << endl;
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
        tqslPathLineEdit->setText(tqslFile);
    }

       //qDebug() << "SetupPageLoTWr::slotSelectTQSLClicked - END" << endl;
}

void SetupPageLoTW::slotPathLineEditChanged(const QString _q)
{
    QPalette palRed;
    palRed.setColor(QPalette::Text, Qt::red);
    QPalette palBlack;
    palBlack.setColor(QPalette::Text, Qt::black);
    if (QFile::exists(_q))
    {
        tqslPathLineEdit->setPalette(palBlack);
    }
    else
    {
        tqslPathLineEdit->setPalette(palRed);
    }

}

void SetupPageLoTW::setLoTWUser(const QString &_s)
{
    lotwUserLineEdit->setText(_s);
}

QString SetupPageLoTW::getLoTWUser()
{
    return lotwUserLineEdit->text();
}

void SetupPageLoTW::setLoTWPass(const QString &_s)
{
    lotwPassLineEdit->setText(_s);
}

QString SetupPageLoTW::getLoTWPass()
{
    return lotwPassLineEdit->text();
}
