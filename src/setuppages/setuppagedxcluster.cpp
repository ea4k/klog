/***************************************************************************
                          setuppagedxcluster.cpp  -  description
                             -------------------
    begin                : nov 2011
    copyright            : (C) 2011 by Jaime Robles
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

#include "setuppagedxcluster.h"


SetupPageDxCluster::SetupPageDxCluster(QWidget *parent)  : QWidget(parent)
{
       //qDebug() << "SetupPageDxCluster::SetupPageDxCluster";
    util = new Utilities(Q_FUNC_INFO);

    dxclusterServersComboBox = new QComboBox;
    dxclusterServersComboBox->addItem("dxfun.com:8000");
    addClusterButton = new QPushButton;
    deleteClusterButton = new QPushButton;

    showHFQCheckbox = new QCheckBox;
    showVHFQCheckbox = new QCheckBox;
    showWARCQCheckbox = new QCheckBox;
    showWorkedQCheckbox = new QCheckBox;
    showConfirmedQCheckbox = new QCheckBox;
    showANNQCheckbox = new QCheckBox;
    showWWVQCheckbox = new QCheckBox;
    showWCYQCheckbox = new QCheckBox;
    sendSpotsToMapCheckbox = new QCheckBox;

    saveAllDXClusterDataQCheckbox = new QCheckBox;

    saveAllDXClusterDataQCheckbox->setAutoExclusive(false);
    showHFQCheckbox->setAutoExclusive(false);
    showVHFQCheckbox->setAutoExclusive(false);
    showWARCQCheckbox->setAutoExclusive(false);
    showWorkedQCheckbox->setAutoExclusive(false);
    showConfirmedQCheckbox->setAutoExclusive(false);
    showANNQCheckbox->setAutoExclusive(false);
    showWWVQCheckbox->setAutoExclusive(false);
    showWCYQCheckbox->setAutoExclusive(false);
    sendSpotsToMapCheckbox->setAutoExclusive (false);

    showHFQCheckbox->setChecked(true);
    showVHFQCheckbox->setChecked(true);
    showWARCQCheckbox->setChecked(true);
    showWorkedQCheckbox->setChecked(true);
    showConfirmedQCheckbox->setChecked(true);
    showANNQCheckbox->setChecked(true);
    showWWVQCheckbox->setChecked(true);
    showWCYQCheckbox->setChecked(true);
    saveAllDXClusterDataQCheckbox->setChecked(false);
    sendSpotsToMapCheckbox->setChecked (false);

    addClusterButton->setText(tr("Add"));
    deleteClusterButton->setText(tr("Delete"));

    showHFQCheckbox->setText(tr("Show &HF spots"));
    showVHFQCheckbox->setText(tr("Show V/&UHF spots"));
    showWARCQCheckbox->setText(tr("Show W&ARC spots"));
    showWorkedQCheckbox->setText(tr("Show &worked spots"));
    showConfirmedQCheckbox->setText(tr("Show &confirmed spots"));
    showANNQCheckbox->setText(tr("Show ANN/&FULL messages"));
    showWWVQCheckbox->setText(tr("Show WW&V messages"));
    showWCYQCheckbox->setText(tr("Show WC&Y messages"));
    saveAllDXClusterDataQCheckbox->setText(tr("Save DX Cluster activity"));
    saveAllDXClusterDataQCheckbox->setToolTip(tr("Saves all the DX-Cluster activity to a file in the KLog folder"));

    sendSpotsToMapCheckbox->setText (tr("Send DX Spots to the map"));
    sendSpotsToMapCheckbox->setToolTip (tr("Sends the received DX Spots to the map to know where is the activity."));

    QGroupBox *spotsGroupBox = new QGroupBox(tr("DX Spots"));

    QVBoxLayout *spotsVBoxLayout = new QVBoxLayout;
    spotsVBoxLayout->addWidget(showHFQCheckbox);
    spotsVBoxLayout->addWidget(showVHFQCheckbox);
    spotsVBoxLayout->addWidget(showWARCQCheckbox);
    spotsVBoxLayout->addWidget(showWorkedQCheckbox);
    spotsVBoxLayout->addWidget(showConfirmedQCheckbox);
    spotsVBoxLayout->addStretch(1);
    spotsGroupBox->setLayout(spotsVBoxLayout);

    QGroupBox *miscGroupBox = new QGroupBox(tr("Others"));

    QVBoxLayout *miscVBoxLayout = new QVBoxLayout;
    miscVBoxLayout->addWidget(saveAllDXClusterDataQCheckbox);
    miscVBoxLayout->addWidget (sendSpotsToMapCheckbox);
    miscVBoxLayout->addStretch(1);
    miscGroupBox->setLayout(miscVBoxLayout);

    QGroupBox *messagesGroupBox = new QGroupBox(tr("Messages"));

    QVBoxLayout *messagesVBoxLayout = new QVBoxLayout;
    messagesVBoxLayout->addWidget(showANNQCheckbox);
    messagesVBoxLayout->addWidget(showWWVQCheckbox);
    messagesVBoxLayout->addWidget(showWCYQCheckbox);
    messagesVBoxLayout->addStretch(1);
    messagesGroupBox->setLayout(messagesVBoxLayout);
// ( QWidget * widget, int fromRow, int fromColumn, int rowSpan,
    //int columnSpan, Qt::Alignment alignment = 0 )
    QHBoxLayout *serversButtonsLayout = new QHBoxLayout;

    serversButtonsLayout->addSpacerItem(new QSpacerItem(10,0,QSizePolicy::Expanding,QSizePolicy::Maximum));
    serversButtonsLayout->addWidget(addClusterButton);
    serversButtonsLayout->addWidget(deleteClusterButton);

    QVBoxLayout *serversLayout = new QVBoxLayout;
    serversLayout->addWidget(dxclusterServersComboBox);
    serversLayout->addLayout(serversButtonsLayout);

    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->addLayout(serversLayout, 0, 0);
    mainLayout->addWidget(spotsGroupBox, 1, 0);
    mainLayout->addWidget(messagesGroupBox, 1, 1);
    mainLayout->addWidget(miscGroupBox, 2, 0, 1, -1);

    setLayout(mainLayout);

    createActions();

       //qDebug() << "SetupPageDxCluster::SetupPageDxCluster - END";
}

SetupPageDxCluster::~SetupPageDxCluster()
{
       //qDebug() << "SetupPageDxCluster::~SetupPageDxCluster";
    delete(util);
}


void SetupPageDxCluster::createActions()
{
       //qDebug() << "SetupPageDxCluster::createActions";
    connect(addClusterButton, SIGNAL(clicked()), this, SLOT(slotAddButtonClicked()) );
    connect(deleteClusterButton, SIGNAL(clicked()), this, SLOT(slotDeleteButtonClicked()) );
}

void SetupPageDxCluster::slotAddButtonClicked()
{
   //qDebug() << "SetupPageDxCluster::slotAddButtonClicked";

    bool ok;
    ok = false;
    while (!ok)
    {
        QString text = QInputDialog::getText (this,
                           tr("KLog: Add a DXCluster server"),
                           tr("Add the address followed by the :port\nExample: dxfun.com:8000\nIf no port is specified, 41112 will be used by default:"),
                           QLineEdit::Normal, QString(),
                           &ok);
        //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - SERVER: " << text;
        if (ok && !text.isEmpty ())
        {
            //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 01" ;
            if (checkIfValidDXCluster (text))
            {
                //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 02" ;
                if (checkIfNewDXCluster (text))
                {
                    //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 03" ;
                    ok = true;
                    if ((text.contains (":")) == 0)
                    {
                       //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 04" ;
                        text = text + ":41112";
                    }
                    dxclusterServersComboBox->insertItem (0, text);
                    //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 05" ;
                }
                else
                {
                   //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 06" ;
                    ok = false;
                }
            }
            else
            {
                //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 07" ;
                ok = false;
            }
        }
        else
        {
            // user entered nothing or pressed Cancel
            //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 08" ;
            ok = true;
        }
    }
}

void SetupPageDxCluster::slotDeleteButtonClicked()
{
    //qDebug() << Q_FUNC_INFO;
    dxclusterServersComboBox->removeItem (dxclusterServersComboBox->currentIndex ());
}

bool SetupPageDxCluster::checkIfValidDXCluster (const QString & tdxcluster)
{
  QUrl url ("http://" + tdxcluster);
  //return !((!url.host ().isEmpty ()) || (url.port () != -1));
    return ((!url.host ().isEmpty ()) || (url.port () != -1));

/*
    if ((!url.host ().isEmpty ()) || (url.port () != -1))
    {
      return true;
    }
    else
    {
      return false;
    }
*/
}

bool SetupPageDxCluster::checkIfNewDXCluster (const QString & tdxcluster)
{
    //qDebug()  << "checkIfNewDXCluster: -" << tdxcluster << "-"<< QT_ENDL;
    return (dxclusterServersComboBox->findText(tdxcluster)<0);
}

QStringList SetupPageDxCluster::getDxclusterServersComboBox()
{
    //qDebug() << "SetupPageDxCluster::getDxclusterServersComboBox";
    QStringList servers;

    int numberOfDXClusterServers = dxclusterServersComboBox->count ();
    servers.clear();
    if(numberOfDXClusterServers>=1)
    {
        //stream << "DXClusterServerToUse=" << dxclusterServersComboBox->currentText ();
        //servers << dxclusterServersComboBox->currentText ();
        for (int i = 0; i <= numberOfDXClusterServers - 1; i++)
          {
            dxclusterServersComboBox->setCurrentIndex (i);
            servers << dxclusterServersComboBox->currentText ();
            //stream << "DXClusterServerPort=" << dxclusterServersComboBox->currentText ();
          }
    }
    return servers;
}

void SetupPageDxCluster::setDxclusterServersComboBox(const QStringList t)
{
    //qDebug() << "SetupPageDxCluster::setDxclusterServersComboBox";
    if (t.count()>0)
    {
        QString text;
        for (int i=0; i < t.count(); i++)
        {
            text.clear();
            text = t.at(i);
            if (checkIfValidDXCluster (text))
            {
                if (checkIfNewDXCluster (text))
                {
                    if ((text.contains (":")) == 0)
                    {
                        text = text + ":41112";
                    }
                    dxclusterServersComboBox->insertItem (0, text);
                }
                else
                {
                  // Not added
                }
            }
        }
    }
}

QString SetupPageDxCluster::getSaveActivityQCheckbox()
{
    return util->boolToQString (saveAllDXClusterDataQCheckbox->isChecked());
}

QString SetupPageDxCluster::getShowHFQCheckbox()
{
    return util->boolToQString (showHFQCheckbox->isChecked());
}

QString SetupPageDxCluster::getShowVHFQCheckbox()
{
    return util->boolToQString (showVHFQCheckbox->isChecked());
}

QString SetupPageDxCluster::getShowWARCQCheckbox()
{
    return util->boolToQString (showWARCQCheckbox->isChecked());
}

QString SetupPageDxCluster::getShowWorkedQCheckbox()
{
    return util->boolToQString (showWorkedQCheckbox->isChecked());
}

QString SetupPageDxCluster::getShowConfirmedQCheckbox()
{
    return util->boolToQString (showConfirmedQCheckbox->isChecked());
}

QString SetupPageDxCluster::getShowANNQCheckbox()
{
    return util->boolToQString (showANNQCheckbox->isChecked());
}

QString SetupPageDxCluster::getShowWWVQCheckbox()
{
    return util->boolToQString (showWWVQCheckbox->isChecked());
}

QString SetupPageDxCluster::getShowWCYQCheckbox()
{
    return util->boolToQString (showWCYQCheckbox->isChecked());
}

void SetupPageDxCluster::setShowHFQCheckbox(const QString t)
{
    showHFQCheckbox->setChecked(util->trueOrFalse (t));
}

void SetupPageDxCluster::setShowVHFQCheckbox(const QString t)
{
    showVHFQCheckbox->setChecked(util->trueOrFalse (t));
}

void SetupPageDxCluster::setShowWARCQCheckbox(const QString t)
{
    showWARCQCheckbox->setChecked(util->trueOrFalse (t));
}

void SetupPageDxCluster::setSaveActivityQCheckbox(const QString t)
{
    saveAllDXClusterDataQCheckbox->setChecked(util->trueOrFalse (t));
}

void SetupPageDxCluster::setShowWorkedQCheckbox(const QString t)
{
    showWorkedQCheckbox->setChecked(util->trueOrFalse (t));
}

void SetupPageDxCluster::setShowConfirmedQCheckbox(const QString t)
{
    showConfirmedQCheckbox->setChecked(util->trueOrFalse (t));
}

void SetupPageDxCluster::setShowANNQCheckbox(const QString t)
{
    showANNQCheckbox->setChecked(util->trueOrFalse (t));
}

void SetupPageDxCluster::setShowWWVQCheckbox(const QString t)
{
    showWWVQCheckbox->setChecked(util->trueOrFalse (t));
}

void SetupPageDxCluster::setShowWCYQCheckbox(const QString t)
{
    showWCYQCheckbox->setChecked(util->trueOrFalse (t));
}

QString SetupPageDxCluster::getSelectedDxClusterServer()
{
    //qDebug() << Q_FUNC_INFO;
    int dxclusterServerListItems = dxclusterServersComboBox->count();
    if (dxclusterServerListItems >= 1)
    {
        //qDebug() << Q_FUNC_INFO << ": " << dxclusterServersComboBox->currentText ();
        return dxclusterServersComboBox->currentText ();
    }
    else
    {
        return QString();
    }
}

void SetupPageDxCluster::setSelectedDxClusterServer(const QString t)
{
    //qDebug() << Q_FUNC_INFO << ": " << t;
    dxclusterServersComboBox->setCurrentIndex(dxclusterServersComboBox->findText(t));
}

QString SetupPageDxCluster::getSendSpotsToMap()
{
    return util->boolToQString (sendSpotsToMapCheckbox->isChecked());
}

void SetupPageDxCluster::setSendSpotstoMap(const QString t)
{
    sendSpotsToMapCheckbox->setChecked(util->trueOrFalse (t));
}

void SetupPageDxCluster::saveSettings()
{
    QSettings settings(util->getSetFile (), QSettings::IniFormat);
    settings.beginGroup ("DXCluster");
    settings.setValue ("DXClusterServerToUse", getSelectedDxClusterServer());
    settings.setValue ("DXClusterServers", (getDxclusterServersComboBox ()).join (','));
    settings.setValue ("DXClusterShowHF", QVariant((showHFQCheckbox->isChecked())));
    settings.setValue ("DXClusterShowVHF", QVariant((showVHFQCheckbox->isChecked())));
    settings.setValue ("DXClusterShowWARC", QVariant((showWARCQCheckbox->isChecked())));
    settings.setValue ("DXClusterShowWorked", QVariant((showWorkedQCheckbox->isChecked())));
    settings.setValue ("DXClusterShowConfirmed", QVariant((showConfirmedQCheckbox->isChecked())));
    settings.setValue ("DXClusterShowAnn", QVariant((showANNQCheckbox->isChecked())));
    settings.setValue ("DXClusterShowWWV", QVariant((showWWVQCheckbox->isChecked())));
    settings.setValue ("DXClusterShowWCY", QVariant((showWCYQCheckbox->isChecked())));
    settings.setValue ("DXClusterSave", QVariant((saveAllDXClusterDataQCheckbox->isChecked())));
    settings.setValue ("DXClusterSendToMap", QVariant((sendSpotsToMapCheckbox->isChecked())));
    settings.endGroup ();
}

void SetupPageDxCluster::loadSettings()
{
    QSettings settings(util->getSetFile (), QSettings::IniFormat);
    settings.beginGroup ("DXCluster");
    setDxclusterServersComboBox(settings.value("DXClusterServers").toStringList ());
    dxclusterServersComboBox->setCurrentIndex(dxclusterServersComboBox->findText(settings.value ("DXClusterServerToUse").toString ()));
    showHFQCheckbox->setChecked (settings.value("DXClusterShowHF").toBool ());
    showVHFQCheckbox->setChecked (settings.value("DXClusterShowVHF").toBool ());
    showWARCQCheckbox->setChecked (settings.value("DXClusterShowWARC").toBool ());
    showWorkedQCheckbox->setChecked (settings.value("DXClusterShowWorked").toBool ());
    showConfirmedQCheckbox->setChecked (settings.value("DXClusterShowConfirmed").toBool ());
    showANNQCheckbox->setChecked (settings.value("DXClusterShowAnn").toBool ());
    showWWVQCheckbox->setChecked (settings.value("DXClusterShowWWV").toBool ());
    showWCYQCheckbox->setChecked (settings.value("DXClusterShowWCY").toBool ());
    saveAllDXClusterDataQCheckbox->setChecked (settings.value("DXClusterSave").toBool ());
    sendSpotsToMapCheckbox->setChecked (settings.value("DXClusterSendToMap").toBool ());
    settings.endGroup ();
}
