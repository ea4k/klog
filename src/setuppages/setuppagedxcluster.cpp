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

void SetupPageDxCluster::init()
{
    showHFQCheckbox->setChecked(true);
    showVHFQCheckbox->setChecked(true);
    showWARCQCheckbox->setChecked(true);
    showWCYQCheckbox->setChecked(true);
    showWWVQCheckbox->setChecked(true);
    showANNQCheckbox->setChecked(true);
    showConfirmedQCheckbox->setChecked(true);
    showWorkedQCheckbox->setChecked(true);
    dxclusterServersComboBox->addItem("dxfun.com:8000");
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

    bool ok = false;

    while (!ok)
    {
        QString text = QInputDialog::getText(this,
                                             tr("KLog: Add a DXCluster server"),
                                             tr("Add the address followed by the :port\nExample: dxfun.com:8000\nIf no port is specified, 41112 will be used by default:"),
                                             QLineEdit::Normal, QString(), &ok);

        if (!ok || text.isEmpty()) // User pressed cancel or entered nothing
        {
            //qDebug() << Q_FUNC_INFO << " -  - User canceled or entered nothing";
            break;
        }

        //qDebug() << Q_FUNC_INFO << " -  - SERVER: " << text;

        if (!checkIfValidDXCluster(text))
        {
            //qDebug() << Q_FUNC_INFO << " -  - Invalid DXCluster";
            continue; // Ask again
        }

        if (!checkIfNewDXCluster(text))
        {
            //qDebug() << Q_FUNC_INFO << " -  - DXCluster already exists";
            continue; // Ask again
        }

        // Append default port if not specified
        if (!text.contains(":"))
        {
            text += ":41112";
            //qDebug() << Q_FUNC_INFO << " -  - Default port added: " << text;
        }

        dxclusterServersComboBox->insertItem(0, text);
        //qDebug() << Q_FUNC_INFO << " -  - DXCluster added: " << text;
        ok = true; // Exit the loop
    }

    QString aux;
    foreach(aux, getDxclusterServersComboBox())
    {
        //qDebug() << Q_FUNC_INFO << " - " << aux;
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


void SetupPageDxCluster::saveSettings()
{
    //qDebug() << Q_FUNC_INFO ;
    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("DXCluster");
    settings.setValue ("DXClusterServerToUse", getSelectedDxClusterServer());

    QList<QString> clusters = getDxclusterServersComboBox ();

    settings.beginWriteArray("DXClusterServers");
     for (int i = 0; i < clusters.size(); ++i) {
         settings.setArrayIndex(i);
         settings.setValue("Server", clusters.at(i));
         //qDebug() << Q_FUNC_INFO << " - Writting Servers: " << clusters.at(i);
     }
    settings.endArray();

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
    QSettings settings(util->getCfgFile (), QSettings::IniFormat);
    settings.beginGroup ("DXCluster");

    showHFQCheckbox->setChecked (settings.value("DXClusterShowHF", true).toBool ());
    showVHFQCheckbox->setChecked (settings.value("DXClusterShowVHF", true).toBool ());
    showWARCQCheckbox->setChecked (settings.value("DXClusterShowWARC", true).toBool ());
    showWorkedQCheckbox->setChecked (settings.value("DXClusterShowWorked", true).toBool ());
    showConfirmedQCheckbox->setChecked (settings.value("DXClusterShowConfirmed", true).toBool ());
    showANNQCheckbox->setChecked (settings.value("DXClusterShowAnn", true).toBool ());
    showWWVQCheckbox->setChecked (settings.value("DXClusterShowWWV", true).toBool ());
    showWCYQCheckbox->setChecked (settings.value("DXClusterShowWCY", true).toBool ());
    saveAllDXClusterDataQCheckbox->setChecked (settings.value("DXClusterSave", false).toBool ());
    sendSpotsToMapCheckbox->setChecked (settings.value("DXClusterSendToMap", true).toBool ());

    QList<QString> servers;
    servers.clear ();

    int size = settings.beginReadArray("DXClusterServers");
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        servers.append (settings.value("Server").toString());
        //qDebug() << Q_FUNC_INFO << " - Reading Servers: " << settings.value("Server").toString();
    }
    settings.endArray();

    setDxclusterServersComboBox(servers);
    QString aux = settings.value ("DXClusterServerToUse").toString ();
    //qDebug() << Q_FUNC_INFO << ": " << aux;
    dxclusterServersComboBox->setCurrentIndex(dxclusterServersComboBox->findText(aux));
    settings.endGroup ();
}
