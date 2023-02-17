/***************************************************************************
                          SetupPageSatsNew.h  -  description
                             -------------------
    begin                : feb 2015
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
//
// This class implements the Dialog to add a new log
//

#include "setuppagesatsnew.h"


SetupPageSatsNew::SetupPageSatsNew(DataProxy_SQLite *dp, QWidget *parent)
{
       //qDebug() << "SetupPageSatsNew::SetupPageSatsNew"  ;
    dataProxy = dp;
    util = new Utilities(Q_FUNC_INFO);
    editing = false;
    //checking = false;

    shortNameLineEdit = new QLineEdit();
    nameLineEdit = new QLineEdit();
    uplinkLineEdit = new QLineEdit();
    downlinkLineEdit = new QLineEdit();
    modesLineEdit = new QLineEdit();

    shortNameLabel = new QLabel(tr("Short name"));
    nameLabel = new QLabel(tr("Sat name"));
    uplinkLabel = new QLabel(tr("UpLink"));
    downlinkLabel = new QLabel(tr("DownLink"));
    modesLabel = new QLabel(tr("Modes"));

    clear();

    //satData.clear();

    okButton = new QPushButton(tr("&Ok"), this);
    cancelButton = new QPushButton(tr("&Cancel"), this);
    createUI();

    okButton->setEnabled(true);

       //qDebug() << "SetupPageSatsNew::SetupPageSatsNew - END"  ;
}

SetupPageSatsNew::~SetupPageSatsNew()
{
    delete(dataProxy);
    delete(util);
}

void SetupPageSatsNew::clear()
{
    shortName = QString();
    name = QString();
    uplink = QString();
    downlink = QString();
    modes = QString();

    hasShortName = false;
    hasName = false;
    hasUplink = false;
    hasDownlink = false;
    hasModes = false;

    shortNameLineEdit->clear();
    nameLineEdit->clear();
    uplinkLineEdit->clear();
    downlinkLineEdit->clear();
    modesLineEdit->clear();

}

void SetupPageSatsNew::createUI()
{
       //qDebug() << "SetupPageSatsNew::createWidget";

    shortNameLabel->setWordWrap(true);
    nameLabel->setWordWrap(true);
    uplinkLabel->setWordWrap(true);
    downlinkLabel->setWordWrap(true);
    modesLabel->setWordWrap(true);

    shortNameLabel->setBuddy(shortNameLineEdit);
    nameLabel->setBuddy(nameLineEdit);
    uplinkLabel->setBuddy(uplinkLineEdit);
    downlinkLabel->setBuddy(downlinkLineEdit);
    modesLabel->setBuddy(modesLineEdit);

    shortNameLineEdit->setToolTip(tr("Enter the short name. Try to use the LoTW short name so you can upload your QSO to LoTW afterwards."));
    nameLineEdit->setToolTip(tr("Enter the name of the satellite."));
    uplinkLineEdit->setToolTip(tr("Enter the uplink frequencies in this format: 144.300"));
    downlinkLineEdit->setToolTip(tr("Enter the downlink frequencies in this format: 144.300"));
    modesLineEdit->setToolTip(tr("Enter the modes in this format: USB"));

    connect(shortNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotShortNameTextChanged() ) );
    connect(nameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotNameTextChanged() ) );
    connect(uplinkLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotUpLinkTextChanged() ) );
    connect(downlinkLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotDownLinkTextChanged() ) );
    connect(modesLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotModesTextChanged() ) );

    connect(okButton,SIGNAL(clicked()), this, SLOT(slotOKButtonClicked() ) );
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(slotCancelButtonClicked() ) );

    QGridLayout *dataLayout = new QGridLayout; // Widget, row, column

    dataLayout->addWidget(shortNameLabel, 0, 0);
    dataLayout->addWidget(shortNameLineEdit, 0, 1);
    dataLayout->addWidget(nameLabel, 1, 0);
    dataLayout->addWidget(nameLineEdit, 1, 1);
    dataLayout->addWidget(uplinkLabel, 2, 0);
    dataLayout->addWidget(uplinkLineEdit, 2, 1);
    dataLayout->addWidget(downlinkLabel, 3, 0);
    dataLayout->addWidget(downlinkLineEdit, 3, 1);
    dataLayout->addWidget(modesLabel, 4, 0);
    dataLayout->addWidget(modesLineEdit, 4, 1);


    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    //buttonsLayout->addWidget(validCats);
    buttonsLayout->addSpacerItem(new QSpacerItem(10,0,QSizePolicy::Expanding,QSizePolicy::Maximum));
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(dataLayout);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
    clear();

}

void SetupPageSatsNew::slotShortNameTextChanged()
{
    int cursorP = shortNameLineEdit->cursorPosition();

    shortNameLineEdit->setText((shortNameLineEdit->text()).toUpper());
    if ((shortNameLineEdit->text()).length()>2)
    {
        hasShortName = true;
    }
    else
    {
        hasShortName = false;
    }
    shortNameLineEdit->setCursorPosition(cursorP);
}

void SetupPageSatsNew::slotNameTextChanged()
{
    int cursorP = nameLineEdit->cursorPosition();
    nameLineEdit->setText((nameLineEdit->text()).toUpper());
    if ((nameLineEdit->text()).length()>2)
    {
        hasName = true;
    }
    else
    {
        hasName = false;
    }

    nameLineEdit->setCursorPosition(cursorP);
}

void SetupPageSatsNew::slotUpLinkTextChanged()
{
/*
    Format: Single freq:            145.600
            Range:                  145.600-145.700
            Two freqs or ranges:    145.600,145.650 or 145.600,145.600-145.700
*/
      //qDebug() << "SetupPageSatsNew::slotUpLinkTextChanged:" <<  uplinkLineEdit->text() ;

    if ((uplinkLineEdit->text()).length()<2)
    {
        return;
    }
    QStringList freqs;
    freqs.clear();

    QString aux = uplinkLineEdit->text();
   // QString str = QString();

    freqs << aux.split(',');
    hasUplink = true;
    foreach (aux, freqs)
    {
        if (aux.contains('-'))
        { // It is a range
              //qDebug() << "SetupPageSatsNew::slotUpLinkTextChanged: It is a range: " <<  aux;
        }
        else
        { //It is just one freq
            if (!isItAFreq(aux))
            {
                hasUplink = false;
            }
        }
    }
}

bool SetupPageSatsNew::isItAFreq(const QString &_st)
{
      //qDebug() << "SetupPageSatsNew::isItAFreq: It is one freq: " << _st;
    bool ok = false;
    double freq = _st.toDouble(&ok);
    return ok;
}

void SetupPageSatsNew::slotDownLinkTextChanged()
{
    /*
        Format: Single freq:            145.600
                Range:                  145.600-145.700
                Two freqs or ranges:    145.600,145.650 or 145.600,145.600-145.700
    */
    if ((downlinkLineEdit->text()).length()<2)
    {
        return;
    }
    QStringList freqs;
    freqs.clear();

    QString aux = downlinkLineEdit->text();
   // QString str = QString();

    freqs << aux.split(',');
    hasDownlink = true;
    foreach (aux, freqs)
    {
        if (aux.contains('-'))
        { // It is a range
              //qDebug() << "SetupPageSatsNew::slotUpLinkTextChanged: It is a range: " <<  aux;
        }
        else
        { //It is just one freq
            if (isItAFreq(aux))
            {
                hasDownlink = false;
            }
        }
    }

}

void SetupPageSatsNew::slotModesTextChanged()
{
    /*
        Format: Single mode:            USB
                Two modes:              USB,LSB
                One mode per up/down    USB/LSB
    */
      //qDebug() << "SetupPageSatsNew::slotModesTextChanged: " <<  modesLineEdit->text();
    int cursorP = modesLineEdit->cursorPosition();

    modesLineEdit->setText((modesLineEdit->text()).toUpper());
    QString aux = QString();
    QString aux2 = QString();
    hasModes = false;
    if ((modesLineEdit->text()).length()<2)
    {
        return;
    }

      //qDebug() << "SetupPageSatsNew::slotModesTextChanged: length >=2";
    QStringList _modes;
    _modes.clear();
    _modes = (modesLineEdit->text()).split(',');

      //qDebug() << "SetupPageSatsNew::slotModesTextChanged: Starting to process the list" ;
    foreach (aux, _modes)
    {
          //qDebug() << "SetupPageSatsNew::slotModesTextChanged: Processing: " <<  aux;
        if (aux.contains('/'))
        {// It s a pair of modes (up and down link)
            QStringList _pair;
            _pair.clear();
            _pair << aux.split('/');
            foreach (aux2, _pair)
            {
                  //qDebug() << "SetupPageSatsNew::slotModesTextChanged: Processing aux2: " <<  aux2;

               if (dataProxy->getIdFromModeName(aux2)>0)
               {
                     //qDebug() << "SetupPageSatsNew::slotModesTextChanged: Checking2: true: " <<  aux2;
                   hasModes = true;
               }
               else
               {
                     //qDebug() << "SetupPageSatsNew::slotModesTextChanged: Checking2: false: " <<  aux2;
                   hasModes = false;
               }
            }

        }
        else
        {
              //qDebug() << "SetupPageSatsNew::slotModesTextChanged: Checking: " <<  aux;

            if (dataProxy->getSubModeIdFromSubMode(aux)>0)
            {
                  //qDebug() << "SetupPageSatsNew::slotModesTextChanged: Checking: true: " <<  aux;
                hasModes = true;
            }
            else
            {
                  //qDebug() << "SetupPageSatsNew::slotModesTextChanged: Checking: false: " <<  aux;
                hasModes = false;
            }
        }
    }
    modesLineEdit->setCursorPosition(cursorP);
}

void SetupPageSatsNew::setShortName(const QString st)
{
    shortNameLineEdit->setText(st.toUpper());
}

void SetupPageSatsNew::setName(const QString st)
{
    nameLineEdit->setText(st.toUpper());
}

void SetupPageSatsNew::setUpLink(const QString st)
{
    uplinkLineEdit->setText(st);
}

void SetupPageSatsNew::setDownLink(const QString st)
{
    downlinkLineEdit->setText(st);
}

void SetupPageSatsNew::setModes(const QString st)
{
    modesLineEdit->setText(st.toUpper());
}


void SetupPageSatsNew::slotOKButtonClicked()
{

       //qDebug() << "SetupPageSatsNew::slotOkButtonClicked";
    shortName = shortNameLineEdit->text();
    name = nameLineEdit->text();
    uplink = uplinkLineEdit->text();
    downlink = downlinkLineEdit->text();
    modes = modesLineEdit->text();

    if (hasShortName && hasName && (hasUplink || hasDownlink) && hasModes)
    {

        gatherAndSend();
        editing = false;
        close();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("Some of the data you have entered is not correct; the satellite can't be added."));
        msgBox.exec();

        if (!hasShortName)
        {
              //qDebug() << "SetupPageSatsNew::slotOkButtonClicked hasShortName is FALSE";
        }
        else if (!hasName)
        {
              //qDebug() << "SetupPageSatsNew::slotOkButtonClicked hasName is FALSE";
        }
        else if (!hasUplink)
        {
              //qDebug() << "SetupPageSatsNew::slotOkButtonClicked hasUpLink is FALSE";
        }
        else if (!hasDownlink)
        {
              //qDebug() << "SetupPageSatsNew::slotOkButtonClicked hasDownLink is FALSE";
        }
        else if (!hasModes)
        {
              //qDebug() << "SetupPageSatsNew::slotOkButtonClicked hasModes is FALSE";
        }
    }
    //close();
}

void SetupPageSatsNew::gatherAndSend()
{
     //qDebug() << "SetupPageSatsNew::gatherAndSend: ";


    // The previous lines will be removed once more contest types have been added

    satData.clear();
    satData << shortNameLineEdit->text() << nameLineEdit->text() << downlinkLineEdit->text() <<
               uplinkLineEdit->text() << modesLineEdit->text();

    int satId = dataProxy->getDBSatId(shortNameLineEdit->text());
      //qDebug() << "SetupPageSatsNew::gatherAndSend: satId: " << QString::number(satId);
    if ((editing) && (satId>0))
    {
        satData << "1";
        editing = false;
        dataProxy->addSatellite(shortNameLineEdit->text(), nameLineEdit->text(), downlinkLineEdit->text(), uplinkLineEdit->text(), modesLineEdit->text(), satId);
    }
    else
    {
        satData << "0";
        dataProxy->addSatellite(shortNameLineEdit->text(), nameLineEdit->text(), downlinkLineEdit->text(), uplinkLineEdit->text(), modesLineEdit->text());
    }

       //qDebug() << "SetupPageSatsNew::gatherAndSend: EMITED";


    emit newSatData(satData);

}

void SetupPageSatsNew::slotCancelButtonClicked()
{
       //qDebug() << "SetupPageSatsNew::slotCancelButtonClicked";
    satData.clear();
    clear();
    emit cancelled(true);
    close();
}

void SetupPageSatsNew::setEditing(const bool b)
{
    editing = b;
    if (!editing)
    {
        clear();
    }

}

