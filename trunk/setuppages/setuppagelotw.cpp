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
    pathLabel = new QLabel;
    searchTQSLPushButton = new QPushButton;

    searchTQSLPushButton->setText(tr("TQSL Path"));
    pathLabel = new QLabel;
    pathLabel->setText(tr("Route to TQSL"));
    pathLabel->setBuddy(tqslPathLineEdit);

    lotwActiveCheckBox->setText(tr("Use TQSL"));
    tqslPathLineEdit->setToolTip(tr("Path to the TQSL software."));
    lotwActiveCheckBox->setToolTip(tr("Enable the LoTW integration with TQSL. You will need to have TQSL installed"));

    QHBoxLayout *pathLayout = new QHBoxLayout;
    pathLayout->addWidget(lotwActiveCheckBox);
    pathLayout->addWidget(pathLabel);
    pathLayout->addWidget(tqslPathLineEdit);
    pathLayout->addWidget(searchTQSLPushButton);

    setLayout(pathLayout);

    connect(searchTQSLPushButton, SIGNAL(clicked()), this, SLOT(slotSelectTQSLClicked()) );
    connect(tqslPathLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotPathLineEditChanged(QString)) );
    //connect(clubLogActiveCheckBox, SIGNAL(toggled(bool) ), this, SLOT(slotClubLogActive(bool)));
}

SetupPageLoTW::~SetupPageLoTW()
{
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
    qDebug() << "SetupPageLoTWr::slotSelectTQSLClicked: " << QStandardPaths::ApplicationsLocation << endl;
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
