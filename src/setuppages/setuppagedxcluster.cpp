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
       //qDebug() << "SetupPageDxCluster::SetupPageDxCluster" << QT_ENDL;

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

       //qDebug() << "SetupPageDxCluster::SetupPageDxCluster - END" << QT_ENDL;
}

SetupPageDxCluster::~SetupPageDxCluster()
{
       //qDebug() << "SetupPageDxCluster::~SetupPageDxCluster" << QT_ENDL;
}


void SetupPageDxCluster::createActions()
{
       //qDebug() << "SetupPageDxCluster::createActions" << QT_ENDL;
    connect(addClusterButton, SIGNAL(clicked()), this, SLOT(slotAddButtonClicked()) );
    connect(deleteClusterButton, SIGNAL(clicked()), this, SLOT(slotDeleteButtonClicked()) );

}

void SetupPageDxCluster::slotAddButtonClicked()
{
       //qDebug() << "SetupPageDxCluster::slotAddButtonClicked" << QT_ENDL;

    bool ok;
    ok = false;
    while (!ok)
    {
        QString text = QInputDialog::getText (this,
                           tr("KLog: Add a DXCluster server"),
                           tr("Add the address followed by the :port\nExample: dxfun.com:8000\nIf no port is specified, 41112 will be used by default:"),
                           QLineEdit::Normal, QString(),
                           &ok);
          //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - SERVER: " << text << QT_ENDL;
        if (ok && !text.isEmpty ())
        {
              //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 01"  << QT_ENDL;
            if (checkIfValidDXCluster (text))
            {
                  //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 02"  << QT_ENDL;
                if (checkIfNewDXCluster (text))
                {
                      //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 03"  << QT_ENDL;
                    ok = true;
                    if ((text.contains (":")) == 0)
                    {
                          //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 04"  << QT_ENDL;
                        text = text + ":41112";
                    }
                    dxclusterServersComboBox->insertItem (0, text);
                      //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 05"  << QT_ENDL;
                }
                else
                {
                      //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 06"  << QT_ENDL;
                    ok = false;
                }
            }
            else
            {
                  //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 07"  << QT_ENDL;
                ok = false;
            }
        }
        else
        {
            // user entered nothing or pressed Cancel
              //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 08"  << QT_ENDL;
            ok = true;
        }
    }
}

void SetupPageDxCluster::slotDeleteButtonClicked()
{
       //qDebug() << "SetupPageDxCluster::slotDeleteButtonClicked" << QT_ENDL;
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
      //qDebug()  << "checkIfNewDXCluster: -" << tdxcluster << "-"<< QT_ENDL;
    //int numberOfDXClusterServers = dxclusterServersComboBox->count ();

    if (dxclusterServersComboBox->findText(tdxcluster)<0)
    {
          //qDebug()  << "checkIfNewDXCluster: true" << QT_ENDL;
        return true;
    }
    else
    {
          //qDebug()  << "checkIfNewDXCluster: false" << QT_ENDL;
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
       //qDebug() << "SetupPageDxCluster::getDxclusterServersComboBox" << QT_ENDL;
    QStringList servers;


    int numberOfDXClusterServers = dxclusterServersComboBox->count ();
    servers.clear();
    if(numberOfDXClusterServers>=1)
    {
        //stream << "DXClusterServerToUse=" << dxclusterServersComboBox->currentText () << QT_ENDL;
        //servers << dxclusterServersComboBox->currentText ();
        for (int i = 0; i <= numberOfDXClusterServers - 1; i++)
          {
            dxclusterServersComboBox->setCurrentIndex (i);
            servers << dxclusterServersComboBox->currentText ();
            //stream << "DXClusterServerPort=" << dxclusterServersComboBox->currentText () << QT_ENDL;
          }
    }

    return servers;
}

void SetupPageDxCluster::setDxclusterServersComboBox(const QStringList t)
{
       //qDebug() << "SetupPageDxCluster::setDxclusterServersComboBox" << QT_ENDL;
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
    if (saveAllDXClusterDataQCheckbox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

QString SetupPageDxCluster::getShowHFQCheckbox()
{

    if (showHFQCheckbox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

QString SetupPageDxCluster::getShowVHFQCheckbox()
{

    if (showVHFQCheckbox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

QString SetupPageDxCluster::getShowWARCQCheckbox()
{

    if (showWARCQCheckbox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

QString SetupPageDxCluster::getShowWorkedQCheckbox()
{

    if (showWorkedQCheckbox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

QString SetupPageDxCluster::getShowConfirmedQCheckbox()
{

    if (showConfirmedQCheckbox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

QString SetupPageDxCluster::getShowANNQCheckbox()
{

    if (showANNQCheckbox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

QString SetupPageDxCluster::getShowWWVQCheckbox()
{

    if (showWWVQCheckbox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

QString SetupPageDxCluster::getShowWCYQCheckbox()
{

    if (showWCYQCheckbox->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

void SetupPageDxCluster::setShowHFQCheckbox(const QString t)
{
    if ( (t.toUpper()) == "FALSE")
    {
        showHFQCheckbox->setChecked(false);
    }
    else
    {
        showHFQCheckbox->setChecked(true);
    }
}

void SetupPageDxCluster::setShowVHFQCheckbox(const QString t)
{
    if ( (t.toUpper()) == "FALSE")
    {
        showVHFQCheckbox->setChecked(false);
    }
    else
    {
       showVHFQCheckbox->setChecked(true);
    }
}

void SetupPageDxCluster::setShowWARCQCheckbox(const QString t)
{
    if ( (t.toUpper()) == "FALSE")
    {
        showWARCQCheckbox->setChecked(false);
    }
    else
    {
        showWARCQCheckbox->setChecked(true);
    }
}

void SetupPageDxCluster::setSaveActivityQCheckbox(const QString t)
{
    if ( (t.toUpper()) == "FALSE")
    {
        saveAllDXClusterDataQCheckbox->setChecked(false);
    }
    else
    {
        saveAllDXClusterDataQCheckbox->setChecked(true);
    }
}

void SetupPageDxCluster::setShowWorkedQCheckbox(const QString t)
{
    if ( (t.toUpper()) == "FALSE")
    {
        showWorkedQCheckbox->setChecked(false);
    }
    else
    {
        showWorkedQCheckbox->setChecked(true);
    }
}

void SetupPageDxCluster::setShowConfirmedQCheckbox(const QString t)
{

    if ( (t.toUpper()) == "FALSE")
    {
        showConfirmedQCheckbox->setChecked(false);
    }
    else
    {
        showConfirmedQCheckbox->setChecked(true);
    }
}

void SetupPageDxCluster::setShowANNQCheckbox(const QString t)
{

    if ( (t.toUpper()) == "FALSE")
    {
        showANNQCheckbox->setChecked(false);
    }
    else
    {
        showANNQCheckbox->setChecked(true);
    }
}

void SetupPageDxCluster::setShowWWVQCheckbox(const QString t)
{
    if ( (t.toUpper()) == "FALSE")
    {
        showWWVQCheckbox->setChecked(false);
    }
    else
    {
        showWWVQCheckbox->setChecked(true);
    }
}

void SetupPageDxCluster::setShowWCYQCheckbox(const QString t)
{
    if ( (t.toUpper()) == "FALSE")
    {
        showWCYQCheckbox->setChecked(false);
    }
    else
    {
        showWCYQCheckbox->setChecked(true);
    }
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


