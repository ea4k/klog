/***************************************************************************
                          softwareupdatedialog.cpp  -  description
                             -------------------
    begin                : feb 2017
    copyright            : (C) 2017 by Jaime Robles
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

#include "softwareupdatedialog.h"

//#include <QDebug>


SoftwareUpdateDialog::SoftwareUpdateDialog()
{
      //qDebug() << "SoftwareUpdateDialog::SoftwareUpdateDialog"  << Qt::endl;

    textBrowser = new QTextBrowser;
    textBrowser->setOpenLinks(true);
    textBrowser->setOpenExternalLinks(true);

    //textBrowser->setFrameShadow(QFrame::Raised);
    //textBrowser->setFrameStyle(QFrame::StyledPanel);

    QPushButton *acceptButton = new QPushButton(tr("Ok"));


    textBrowser->setOpenExternalLinks(true);
    //textBrowser->setHTML(url);


    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(acceptButton);


    ///
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(textBrowser);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);

    setWindowTitle(tr("KLog update"));

    ///

    connect(acceptButton, SIGNAL(clicked()), this, SLOT(slotAcceptButtonClicked()));
     //qDebug() << "SoftwareUpdateDialog::SoftwareUpdateDialog - END"  << Qt::endl;
}

void SoftwareUpdateDialog::setVersion(const QString tversion, const bool updateNeeded)
{
     //qDebug() << "SoftwareUpdateDialog::setVersion: " << tversion << Qt::endl;
    _version = tversion;
    if (updateNeeded)
    {
        //text = "<center><h2>KLog new version ("+ tversion + ") is available! </h2></center><br>There is a new version of KLog available.<br><br><b>You can get the new version from:<br><br><center><a href=https://www.klog.xyz>https://www.klog.xyz</a></center>";
        text = tr("<center><h2>KLog new version (%1) is available! </h2></center><br>There is a new version of KLog available.<br><br><b>You can get the new version from:<br><br><center><a href=https://www.klog.xyz>https://www.klog.xyz</a></center>").arg(tversion);

    }
    else
    {
        text = "<center><h2>" + tr("Congratulations!") + "</h2></center><br><br>" + tr("Your KLog has been updated.") + "<br><br>" + tr("You already have the latest version.") + "<br><center>("+ tversion + ")</center>";
    }

    textBrowser->setHtml(text);
     //qDebug() << "SoftwareUpdateDialog::setVersion: END"<< Qt::endl;
}

SoftwareUpdateDialog::~SoftwareUpdateDialog()
{
      //qDebug() << "SoftwareUpdateDialog::~SoftwareUpdateDialog"  << Qt::endl;
}

void SoftwareUpdateDialog::slotAcceptButtonClicked()
{
      //qDebug() << "SoftwareUpdateDialog::slotAcceptButtonClicked"  << Qt::endl;
    accept();
     //qDebug() << "SoftwareUpdateDialog::slotAcceptButtonClicked END"  << Qt::endl;
}



void SoftwareUpdateDialog::keyPressEvent(QKeyEvent *event)
{
    //qDebug() << "SoftwareUpdateDialog::keyPressEvent"  << Qt::endl;
    //switch (event->key()) {
    // break;
    //default:
    slotAcceptButtonClicked();

    //}
    //qDebug() << "SoftwareUpdateDialog::keyPressEvent END"  << Qt::endl;
}
