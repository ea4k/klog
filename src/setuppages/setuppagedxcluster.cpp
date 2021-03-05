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
       //qDebug() << "SetupPageDxCluster::SetupPageDxCluster" << endl;

    //xClusterServers = new QStringList;
    //dxClusterServers << "dxfun.com:8000";

    dxclusterServersComboBox = new QComboBox;
    dxclusterServersComboBox->addItem("dxfun.com:8000");
    addClusterButton = new QPushButton;
    deleteClusterButton = new QPushButton;

    showHFRadiobutton = new QRadioButton;
    showVHFRadiobutton = new QRadioButton;
    showWARCRadiobutton = new QRadioButton;
    showWorkedRadiobutton = new QRadioButton;
    showConfirmedRadiobutton = new QRadioButton;
    showANNRadiobutton = new QRadioButton;
    showWWVRadiobutton = new QRadioButton;
    showWCYRadiobutton = new QRadioButton;

    saveAllDXClusterDataRadiobutton = new QRadioButton;

    saveAllDXClusterDataRadiobutton->setAutoExclusive(false);
    showHFRadiobutton->setAutoExclusive(false);
    showVHFRadiobutton->setAutoExclusive(false);
    showWARCRadiobutton->setAutoExclusive(false);
    showWorkedRadiobutton->setAutoExclusive(false);
    showConfirmedRadiobutton->setAutoExclusive(false);
    showANNRadiobutton->setAutoExclusive(false);
    showWWVRadiobutton->setAutoExclusive(false);
    showWCYRadiobutton->setAutoExclusive(false);

    showHFRadiobutton->setChecked(true);
    showVHFRadiobutton->setChecked(true);
    showWARCRadiobutton->setChecked(true);
    showWorkedRadiobutton->setChecked(true);
    showConfirmedRadiobutton->setChecked(true);
    showANNRadiobutton->setChecked(true);
    showWWVRadiobutton->setChecked(true);
    showWCYRadiobutton->setChecked(true);
    saveAllDXClusterDataRadiobutton->setChecked(false);


    addClusterButton->setText(tr("Add"));
    deleteClusterButton->setText(tr("Delete"));

    showHFRadiobutton->setText(tr("Show &HF spots"));
    showVHFRadiobutton->setText(tr("Show V/&UHF spots"));
    showWARCRadiobutton->setText(tr("Show W&ARC spots"));
    showWorkedRadiobutton->setText(tr("Show &worked spots"));
    showConfirmedRadiobutton->setText(tr("Show &confirmed spots"));
    showANNRadiobutton->setText(tr("Show ANN/&FULL messages"));
    showWWVRadiobutton->setText(tr("Show WW&V messages"));
    showWCYRadiobutton->setText(tr("Show WC&Y messages"));
    saveAllDXClusterDataRadiobutton->setText(tr("Save DX Cluster activity"));
    saveAllDXClusterDataRadiobutton->setToolTip(tr("Saves all the DX-Cluster activity to a file in the KLog folder"));


    QGroupBox *spotsGroupBox = new QGroupBox(tr("DX Spots"));

    QVBoxLayout *spotsVBoxLayout = new QVBoxLayout;
    spotsVBoxLayout->addWidget(showHFRadiobutton);
    spotsVBoxLayout->addWidget(showVHFRadiobutton);
    spotsVBoxLayout->addWidget(showWARCRadiobutton);
    spotsVBoxLayout->addWidget(showWorkedRadiobutton);
    spotsVBoxLayout->addWidget(showConfirmedRadiobutton);
    spotsVBoxLayout->addStretch(1);
    spotsGroupBox->setLayout(spotsVBoxLayout);

    QGroupBox *miscGroupBox = new QGroupBox(tr("Others"));

    QVBoxLayout *miscVBoxLayout = new QVBoxLayout;
    miscVBoxLayout->addWidget(saveAllDXClusterDataRadiobutton);
    miscVBoxLayout->addStretch(1);
    miscGroupBox->setLayout(miscVBoxLayout);

    QGroupBox *messagesGroupBox = new QGroupBox(tr("Messages"));

    QVBoxLayout *messagesVBoxLayout = new QVBoxLayout;
    messagesVBoxLayout->addWidget(showANNRadiobutton);
    messagesVBoxLayout->addWidget(showWWVRadiobutton);
    messagesVBoxLayout->addWidget(showWCYRadiobutton);
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

       //qDebug() << "SetupPageDxCluster::SetupPageDxCluster - END" << endl;
}

SetupPageDxCluster::~SetupPageDxCluster()
{
       //qDebug() << "SetupPageDxCluster::~SetupPageDxCluster" << endl;
}


void SetupPageDxCluster::createActions()
{
       //qDebug() << "SetupPageDxCluster::createActions" << endl;
    connect(addClusterButton, SIGNAL(clicked()), this, SLOT(slotAddButtonClicked()) );
    connect(deleteClusterButton, SIGNAL(clicked()), this, SLOT(slotDeleteButtonClicked()) );

}

void SetupPageDxCluster::slotAddButtonClicked()
{
       //qDebug() << "SetupPageDxCluster::slotAddButtonClicked" << endl;

    bool ok;
    ok = false;
    while (!ok)
    {
        QString text = QInputDialog::getText (this,
                           tr("KLog: Add a DXCluster server"),
                           tr("Add the address followed by the :port\nExample: dxfun.com:8000\nIf no port is specified, 41112 will be used by default:"),
                           QLineEdit::Normal, QString::null,
                           &ok);
          //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - SERVER: " << text << endl;
        if (ok && !text.isEmpty ())
        {
              //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 01"  << endl;
            if (checkIfValidDXCluster (text))
            {
                  //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 02"  << endl;
                if (checkIfNewDXCluster (text))
                {
                      //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 03"  << endl;
                    ok = true;
                    if ((text.contains (":")) == 0)
                    {
                          //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 04"  << endl;
                        text = text + ":41112";
                    }
                    dxclusterServersComboBox->insertItem (0, text);
                      //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 05"  << endl;
                }
                else
                {
                      //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 06"  << endl;
                    ok = false;
                }
            }
            else
            {
                  //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 07"  << endl;
                ok = false;
            }
        }
        else
        {
            // user entered nothing or pressed Cancel
              //qDebug() << "SetupPageDxCluster::slotAddButtonClicked - 08"  << endl;
            ok = true;
        }
    }
}

void SetupPageDxCluster::slotDeleteButtonClicked()
{
       //qDebug() << "SetupPageDxCluster::slotDeleteButtonClicked" << endl;
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
      //qDebug()  << "checkIfNewDXCluster: -" << tdxcluster << "-"<< endl;
    //int numberOfDXClusterServers = dxclusterServersComboBox->count ();

    if (dxclusterServersComboBox->findText(tdxcluster)<0)
    {
          //qDebug()  << "checkIfNewDXCluster: true" << endl;
        return true;
    }
    else
    {
          //qDebug()  << "checkIfNewDXCluster: false" << endl;
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
       //qDebug() << "SetupPageDxCluster::getDxclusterServersComboBox" << endl;
    QStringList servers;


    int numberOfDXClusterServers = dxclusterServersComboBox->count ();
    servers.clear();
    if(numberOfDXClusterServers>=1)
    {
        //stream << "DXClusterServerToUse=" << dxclusterServersComboBox->currentText () << endl;
        //servers << dxclusterServersComboBox->currentText ();
        for (int i = 0; i <= numberOfDXClusterServers - 1; i++)
          {
            dxclusterServersComboBox->setCurrentIndex (i);
            servers << dxclusterServersComboBox->currentText ();
            //stream << "DXClusterServerPort=" << dxclusterServersComboBox->currentText () << endl;
          }
    }

    return servers;
}

void SetupPageDxCluster::setDxclusterServersComboBox(const QStringList t)
{
       //qDebug() << "SetupPageDxCluster::setDxclusterServersComboBox" << endl;
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

QString SetupPageDxCluster::getSaveActivityRadiobutton()
{
    if (saveAllDXClusterDataRadiobutton->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

QString SetupPageDxCluster::getShowHFRadiobutton()
{

    if (showHFRadiobutton->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

QString SetupPageDxCluster::getShowVHFRadiobutton()
{

    if (showVHFRadiobutton->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

QString SetupPageDxCluster::getShowWARCRadiobutton()
{

    if (showWARCRadiobutton->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

QString SetupPageDxCluster::getShowWorkedRadiobutton()
{

    if (showWorkedRadiobutton->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

QString SetupPageDxCluster::getShowConfirmedRadiobutton()
{

    if (showConfirmedRadiobutton->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

QString SetupPageDxCluster::getShowANNRadiobutton()
{

    if (showANNRadiobutton->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

QString SetupPageDxCluster::getShowWWVRadiobutton()
{

    if (showWWVRadiobutton->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

QString SetupPageDxCluster::getShowWCYRadiobutton()
{

    if (showWCYRadiobutton->isChecked())
    {
        return "True";
    }
    else
    {
        return "False";
    }

}

void SetupPageDxCluster::setShowHFRadiobutton(const QString t)
{
    if ( (t.toUpper()) == "FALSE")
    {
        showHFRadiobutton->setChecked(false);
    }
    else
    {
        showHFRadiobutton->setChecked(true);
    }
}

void SetupPageDxCluster::setShowVHFRadiobutton(const QString t)
{
    if ( (t.toUpper()) == "FALSE")
    {
        showVHFRadiobutton->setChecked(false);
    }
    else
    {
       showVHFRadiobutton->setChecked(true);
    }
}

void SetupPageDxCluster::setShowWARCRadiobutton(const QString t)
{    
    if ( (t.toUpper()) == "FALSE")
    {
        showWARCRadiobutton->setChecked(false);
    }
    else
    {
        showWARCRadiobutton->setChecked(true);
    }
}

void SetupPageDxCluster::setSaveActivityRadiobutton(const QString t)
{
    if ( (t.toUpper()) == "FALSE")
    {
        saveAllDXClusterDataRadiobutton->setChecked(false);
    }
    else
    {
        saveAllDXClusterDataRadiobutton->setChecked(true);
    }
}

void SetupPageDxCluster::setShowWorkedRadiobutton(const QString t)
{
    if ( (t.toUpper()) == "FALSE")
    {
        showWorkedRadiobutton->setChecked(false);
    }
    else
    {
        showWorkedRadiobutton->setChecked(true);
    }
}

void SetupPageDxCluster::setShowConfirmedRadiobutton(const QString t)
{

    if ( (t.toUpper()) == "FALSE")
    {
        showConfirmedRadiobutton->setChecked(false);
    }
    else
    {
        showConfirmedRadiobutton->setChecked(true);
    }
}

void SetupPageDxCluster::setShowANNRadiobutton(const QString t)
{

    if ( (t.toUpper()) == "FALSE")
    {
        showANNRadiobutton->setChecked(false);
    }
    else
    {
        showANNRadiobutton->setChecked(true);
    }
}

void SetupPageDxCluster::setShowWWVRadiobutton(const QString t)
{
    if ( (t.toUpper()) == "FALSE")
    {
        showWWVRadiobutton->setChecked(false);
    }
    else
    {
        showWWVRadiobutton->setChecked(true);
    }
}

void SetupPageDxCluster::setShowWCYRadiobutton(const QString t)
{    
    if ( (t.toUpper()) == "FALSE")
    {
        showWCYRadiobutton->setChecked(false);
    }
    else
    {
        showWCYRadiobutton->setChecked(true);
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
        return QString::null;
    }
    //return QString::null;

}


void SetupPageDxCluster::setSelectedDxClusterServer(const QString t)
{
    dxclusterServersComboBox->setCurrentIndex(dxclusterServersComboBox->findText(t));
}


