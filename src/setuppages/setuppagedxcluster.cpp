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

#include "setuppages/setuppagedxcluster.h"


SetupPageDxCluster::SetupPageDxCluster(QWidget *parent)  : QWidget(parent)
{
       //qDebug() << "SetupPageDxCluster::SetupPageDxCluster" << Qt::endl;

    //xClusterServers = new QStringList;
    //dxClusterServers << "dxfun.com:8000";

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

    showHFQCheckbox->setChecked(true);
    showVHFQCheckbox->setChecked(true);
    showWARCQCheckbox->setChecked(true);
    showWorkedQCheckbox->setChecked(true);
    showConfirmedQCheckbox->setChecked(true);
    showANNQCheckbox->setChecked(true);
    showWWVQCheckbox->setChecked(true);
    showWCYQCheckbox->setChecked(true);
    saveAllDXClusterDataQCheckbox->setChecked(false);


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

       //qDebug() << "SetupPageDxCluster::SetupPageDxCluster - END" << Qt::endl;
}

SetupPageDxCluster::~SetupPageDxCluster()
{
       //qDebug() << "SetupPageDxCluster::~SetupPageDxCluster" << Qt::endl;
}


void SetupPageDxCluster::createActions()
{
       //qDebug() << "SetupPageDxCluster::createActions" << Qt::endl;
    connect(addClusterButton, SIGNAL(clicked()), this, SLOT(slotAddButtonClicked()) );
    connect(deleteClusterButton, SIGNAL(clicked()), this, SLOT(slotDeleteButtonClicked()) );

}

void SetupPageDxCluster::slotAddButtonClicked()
{
       //qDebug() << "SetupPageDxCluster::slotAddButtonClicked" << Qt::endl;

    bool ok;
    ok = false;
    while (!ok)
    {
        QString text = QInputDialog::getText (this,
                           tr("KLog: Add a DXCluster server"),
                           tr("Add the address followed by the :port\nExample: dxfun.com:8000\nIf no port is specified, 41112 will be used by default:"),
                           QLineEdit::Normal, QString(),
                           &ok);
          //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - SERVER: " << text << Qt::endl;
        if (ok && !text.isEmpty ())
        {
              //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 01"  << Qt::endl;
            if (checkIfValidDXCluster (text))
            {
                  //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 02"  << Qt::endl;
                if (checkIfNewDXCluster (text))
                {
                      //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 03"  << Qt::endl;
                    ok = true;
                    if ((text.contains (":")) == 0)
                    {
                          //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 04"  << Qt::endl;
                        text = text + ":41112";
                    }
                    dxclusterServersComboBox->insertItem (0, text);
                      //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 05"  << Qt::endl;
                }
                else
                {
                      //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 06"  << Qt::endl;
                    ok = false;
                }
            }
            else
            {
                  //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 07"  << Qt::endl;
                ok = false;
            }
        }
        else
        {
            // user entered nothing or pressed Cancel
              //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 08"  << Qt::endl;
            ok = true;
        }
    }
}

void SetupPageDxCluster::slotDeleteButtonClicked()
{
       //qDebug() << "SetupPageDxCluster::slotDeleteButtonClicked" << Qt::endl;
    dxclusterServersComboBox->removeItem (dxclusterServersComboBox->currentIndex ());

}

bool SetupPageDxCluster::checkIfValidDXCluster (const QString & tdxcluster)
{
  QUrl url ("http://" + tdxcluster);
  if ((!url.host ().isEmpty ()) || (url.port () != -1))
  {
      return true;
  }
  else
  {
      return false;
  }

}

bool SetupPageDxCluster::checkIfNewDXCluster (const QString & tdxcluster) {
      //qDebug()  << "checkIfNewDXCluster: -" << tdxcluster << "-"<< Qt::endl;
    //int numberOfDXClusterServers = dxclusterServersComboBox->count ();

    if (dxclusterServersComboBox->findText(tdxcluster)<0)
    {
          //qDebug()  << "checkIfNewDXCluster: true" << Qt::endl;
        return true;
    }
    else
    {
          //qDebug()  << "checkIfNewDXCluster: false" << Qt::endl;
        return false;
    }
    /*
    for (int i = 0; i <= numberOfDXClusterServers - 1; i++)
    {


        if ((dxclusterServersComboBox->currentText ()) == (tdxcluster))
        {
            return false;
        }

        dxclusterServersComboBox->setCurrentIndex(i);
    }
    return true;
    */
}

QStringList SetupPageDxCluster::getDxclusterServersComboBox()
{
       //qDebug() << "SetupPageDxCluster::getDxclusterServersComboBox" << Qt::endl;
    QStringList servers;


    int numberOfDXClusterServers = dxclusterServersComboBox->count ();
    servers.clear();
    if(numberOfDXClusterServers>=1)
    {
        //stream << "DXClusterServerToUse=" << dxclusterServersComboBox->currentText () << Qt::endl;
        //servers << dxclusterServersComboBox->currentText ();
        for (int i = 0; i <= numberOfDXClusterServers - 1; i++)
          {
            dxclusterServersComboBox->setCurrentIndex (i);
            servers << dxclusterServersComboBox->currentText ();
            //stream << "DXClusterServerPort=" << dxclusterServersComboBox->currentText () << Qt::endl;
          }
    }

    return servers;
}

void SetupPageDxCluster::setDxclusterServersComboBox(const QStringList t)
{
       //qDebug() << "SetupPageDxCluster::setDxclusterServersComboBox" << Qt::endl;
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

bool SetupPageDxCluster::getSaveActivityQCheckbox()
{
    return saveAllDXClusterDataQCheckbox->isChecked();
}

bool SetupPageDxCluster::getShowHFQCheckbox()
{
    return showHFQCheckbox->isChecked();
}

bool SetupPageDxCluster::getShowVHFQCheckbox()
{
    return showVHFQCheckbox->isChecked();
}

bool SetupPageDxCluster::getShowWARCQCheckbox()
{
    return showWARCQCheckbox->isChecked();
}

bool SetupPageDxCluster::getShowWorkedQCheckbox()
{
    return showWorkedQCheckbox->isChecked();
}

bool SetupPageDxCluster::getShowConfirmedQCheckbox()
{
    return showConfirmedQCheckbox->isChecked();
}

bool SetupPageDxCluster::getShowANNQCheckbox()
{
    return showANNQCheckbox->isChecked();
}

bool SetupPageDxCluster::getShowWWVQCheckbox()
{
    return showWWVQCheckbox->isChecked();
}

bool SetupPageDxCluster::getShowWCYQCheckbox()
{
    return showWCYQCheckbox->isChecked();
}

void SetupPageDxCluster::setShowHFQCheckbox(const bool t)
{
    showHFQCheckbox->setChecked(t);
}

void SetupPageDxCluster::setShowVHFQCheckbox(const bool t)
{
    showVHFQCheckbox->setChecked(t);
}

void SetupPageDxCluster::setShowWARCQCheckbox(const bool t)
{
    showWARCQCheckbox->setChecked(t);
}

void SetupPageDxCluster::setSaveActivityQCheckbox(const bool t)
{
    saveAllDXClusterDataQCheckbox->setChecked(t);
}

void SetupPageDxCluster::setShowWorkedQCheckbox(const bool t)
{
    showWorkedQCheckbox->setChecked(t);
}

void SetupPageDxCluster::setShowConfirmedQCheckbox(const bool t)
{
    showConfirmedQCheckbox->setChecked(t);
}

void SetupPageDxCluster::setShowANNQCheckbox(const bool t)
{
    showANNQCheckbox->setChecked(t);
}

void SetupPageDxCluster::setShowWWVQCheckbox(const bool t)
{
    showWWVQCheckbox->setChecked(t);
}

void SetupPageDxCluster::setShowWCYQCheckbox(const bool t)
{
    showWCYQCheckbox->setChecked(t);
}

QString SetupPageDxCluster::getSelectedDxClusterServer()
{
    //return dxclusterServersComboBox->currentText();
    int dxclusterServerListItems = dxclusterServersComboBox->count ();
    if (dxclusterServerListItems >= 1)
    {
        return dxclusterServersComboBox->currentText ();
    }
    else
    {
        return QString();
    }
    //return QString();

}


void SetupPageDxCluster::setSelectedDxClusterServer(const QString t)
{
    dxclusterServersComboBox->setCurrentIndex(dxclusterServersComboBox->findText(t));
}


