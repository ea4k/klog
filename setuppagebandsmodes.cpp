/***************************************************************************
                          setuppagebandsmodes.cpp  -  description
                             -------------------
    begin                : sept 2011
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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/

//#include <//qDebug>

#include "setuppagebandsmodes.h"


SetupPageBandsModes::SetupPageBandsModes(QWidget *parent) : QWidget(parent){
   //qDebug() << "SetupPageBandsModes::SetupPageBandsModes" << endl;
   // bandsWidget = new QWidget;

    dataProxy = new DataProxy_SQLite();
   //qDebug() << "SetupPageBandsModes::SetupPageBandsModes -1" << endl;

    bandsNotActiveListWidget = new QListWidget(this);
    bandsActiveListWidget = new QListWidget(this);
    modesNotActiveListWidget = new QListWidget(this);

    modesActiveListWidget = new QListWidget(this);
    bandsNotActiveListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    bandsActiveListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    modesNotActiveListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    modesActiveListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    QGroupBox *bandsGroupBox = new QGroupBox(tr("Bands"));
    QGroupBox *modesGroupBox = new QGroupBox(tr("Modes"));

    QHBoxLayout *bandsLayout = new QHBoxLayout;
    bandsLayout->addWidget(bandsNotActiveListWidget);
    bandsLayout->addWidget(bandsActiveListWidget);
    bandsLayout->addStretch(1);
    bandsGroupBox->setLayout(bandsLayout);

    QHBoxLayout *modesLayout = new QHBoxLayout;
    modesLayout->addWidget(modesNotActiveListWidget);
    modesLayout->addWidget(modesActiveListWidget);
    modesLayout->addStretch(1);
    modesGroupBox->setLayout(modesLayout);

    bandSelPushButton = new QPushButton(tr("&Add Band"), this);;
    bandUnSelPushButton  = new QPushButton(tr("&Remove Band"), this);
    modeSelPushButton  = new QPushButton(tr("Add &Mode"), this);
    modeUnSelPushButton = new QPushButton(tr("Remove M&ode"), this);;

    QVBoxLayout *bandsButtonsLayout = new QVBoxLayout;
    bandsButtonsLayout->addWidget(bandSelPushButton);
    bandsButtonsLayout->addWidget(bandUnSelPushButton);

    QVBoxLayout *modesButtonsLayout = new QVBoxLayout;
    modesButtonsLayout->addWidget(modeSelPushButton);
    modesButtonsLayout->addWidget(modeUnSelPushButton);

    QHBoxLayout *bandsModesWidgetLayout = new QHBoxLayout;
    bandsModesWidgetLayout->addWidget(bandsGroupBox);
    bandsModesWidgetLayout->addLayout(bandsButtonsLayout);
    bandsModesWidgetLayout->addWidget(modesGroupBox);
    bandsModesWidgetLayout->addLayout(modesButtonsLayout);
   //qDebug() << "SetupPageBandsModes::SetupPageBandsModes -2" << endl;
    bands.clear();
   //qDebug() << "SetupPageBandsModes::SetupPageBandsModes -2.1" << endl;
    bands << dataProxy->getBands();
   //qDebug() << "SetupPageBandsModes::SetupPageBandsModes -3" << endl;
    modes.clear();
   //qDebug() << "SetupPageBandsModes::SetupPageBandsModes -3.1" << endl;
    modes << dataProxy->getModes();
   //qDebug() << "SetupPageBandsModes::SetupPageBandsModes -4" << endl;
    bandsNotActiveListWidget->addItems(bands);
   //qDebug() << "SetupPageBandsModes::SetupPageBandsModes -5" << endl;
    modesNotActiveListWidget->addItems(modes);
   //qDebug() << "SetupPageBandsModes::SetupPageBandsModes -6" << endl;
    setLayout(bandsModesWidgetLayout);

    connect(bandsNotActiveListWidget, SIGNAL(itemDoubleClicked (  QListWidgetItem *  )), this, SLOT(slotBandActiveItemDoubleClicked ( QListWidgetItem * ) ) );
    connect(bandsActiveListWidget, SIGNAL(itemDoubleClicked (  QListWidgetItem *  )), this, SLOT(slotBandNotActiveItemDoubleClicked ( QListWidgetItem * ) ) );
    connect(bandSelPushButton, SIGNAL(clicked ( )), this, SLOT(slotBandSelButtonClicked ( ) ) );
    connect(bandUnSelPushButton, SIGNAL(clicked ( )), this, SLOT(slotBandUnSelButtonClicked ( ) ) );
    connect(modeSelPushButton, SIGNAL(clicked ( )), this, SLOT(slotModeSelButtonClicked ( ) ) );
    connect(modeUnSelPushButton, SIGNAL(clicked ( )), this, SLOT(slotModeUnSelButtonClicked ( ) ) );
    //qDebug() << "SetupPageBandsModes::SetupPageBandsModes - END" << endl;
    //connect(logView, SIGNAL(doubleClicked ( const QModelIndex& ) ), this, SLOT(slotDoubleClickLog( const QModelIndex& ) ) );
}

SetupPageBandsModes::~SetupPageBandsModes(){
    //qDebug() << "SetupPageBandsModes::~SetupPageBandsModes" << endl;
}

void SetupPageBandsModes::createActions(){
//void	itemDoubleClicked ( QListWidgetItem * item )


}

void SetupPageBandsModes::slotBandActiveItemDoubleClicked ( QListWidgetItem * item ){
    //qDebug() << "SetupPageBandsModes::slotBandActiveItemDoubleClicked" << item->text() << endl;

    bandsActiveListWidget->addItem(item->text());
    //bandsActiveListWidget->removeItemWidget(item->cur);

}

void SetupPageBandsModes::slotBandNotActiveItemDoubleClicked ( QListWidgetItem * item ){
    //qDebug() << "SetupPageBandsModes::slotBandNotActiveItemDoubleClicked" << item->text() << endl;

    //bandsNotActiveListWidget->addItem(itemitem->text());
    //bandsNotActiveListWidget->removeItemWidget(item);

}

void SetupPageBandsModes::slotBandSelButtonClicked(){
    //qDebug() << "SetupPageBandsModes::slotBandSelButtonClicked" << endl;

    QListWidgetItem *it;

    for (int i = 0; i < bandsNotActiveListWidget->count(); i++)
    {
        it = bandsNotActiveListWidget->item(i);
        if( (*it).isSelected() )
        {
            bandsActiveListWidget->addItem((it)->text());
            bandsNotActiveListWidget->takeItem(i);
            i--;
        }
        else
        {
        }
    }

};

void SetupPageBandsModes::slotBandUnSelButtonClicked(){
    //qDebug() << "SetupPageBandsModes::slotBandUnSelButtonClicked" << endl;

    QListWidgetItem *it;

    for (int i = 0; i < bandsActiveListWidget->count(); i++)
    {
        it = bandsActiveListWidget->item(i);
        if( (*it).isSelected() )
        {
            bandsNotActiveListWidget->addItem((it)->text());
            bandsActiveListWidget->takeItem(i);
            i--;
        }
        else
        {
        }
    }

};

void SetupPageBandsModes::slotModeSelButtonClicked(){
    //qDebug() << "SetupPageBandsModes::slotModeSelButtonClicked" << endl;

    QListWidgetItem *it;

    for (int i = 0; i < modesNotActiveListWidget->count(); i++)
    {
        it = modesNotActiveListWidget->item(i);
        if( (*it).isSelected() )
        {
            modesActiveListWidget->addItem((it)->text());
            modesNotActiveListWidget->takeItem(i);
            i--;
        }
        else
        {
        }
    }


};

void SetupPageBandsModes::slotModeUnSelButtonClicked(){
    //qDebug() << "SetupPageBandsModes::slotModeUnSelButtonClicked" << endl;

    QListWidgetItem *it;

    for (int i = 0; i < modesActiveListWidget->count(); i++)
    {
        it = modesActiveListWidget->item(i);
        if( (*it).isSelected() )
        {
            modesNotActiveListWidget->addItem((it)->text());
            modesActiveListWidget->takeItem(i);
            i--;
        }
        else
        {
        }
    }

};

QString SetupPageBandsModes::getBands()
{
    //qDebug() << "SetupPageBandsModes::getBands" << endl;

    QString b;
    QListWidgetItem *it;

    if ( (bandsActiveListWidget->count()) < 1)
    {
        return "";
    }

    for (int i = 0; i < bandsActiveListWidget->count(); i++)
    {
        it = bandsActiveListWidget->item(i);
        b = b + it->text();
        b = b + ", ";

    }

    if (b.size()<2)
    {
    }else
    {
        b.chop(2);
    }
    //qDebug() << "SetupPageBandsModes::getBands: " << b << endl;
    return b;
}

QString SetupPageBandsModes::getModes()
{
    //qDebug() << "SetupPageBandsModes::getModes" << endl;

    QString b;
    QListWidgetItem *it;

    if ( (modesActiveListWidget->count()) < 1)
    {
        return "";
    }

    for (int i = 0; i < modesActiveListWidget->count(); i++)
    {
        it = modesActiveListWidget->item(i);
        b = b + it->text();
        b = b + ", ";

    }

    if (b.size()<2)
    {
    }else
    {
        b.chop(2);
    }
    //qDebug() << "SetupPageBandsModes::getModes: " << b << endl;
    return b;
}

void SetupPageBandsModes::setActiveBands(QStringList q)
{
    QListWidgetItem *it, *itn;

    bandsActiveListWidget->addItems(q);

    for (int i = 0; i < bandsActiveListWidget->count(); i++)
    {
        it = bandsActiveListWidget->item(i);

        for (int j = 0; j < bandsNotActiveListWidget->count(); j++)
        {
            itn = bandsNotActiveListWidget->item(j);
            if ( (itn->text()) == (it->text()) )
            {
                bandsNotActiveListWidget->takeItem(j);
            }
        }

    }

}

void SetupPageBandsModes::setActiveModes(QStringList q)
{
    QListWidgetItem *it, *itn;

    modesActiveListWidget->addItems(q);

    for (int i = 0; i < modesActiveListWidget->count(); i++)
    {
        it = modesActiveListWidget->item(i);

        for (int j = 0; j < modesNotActiveListWidget->count(); j++)
        {
            itn = modesNotActiveListWidget->item(j);
            if ( (itn->text()) == (it->text()) )
            {
                modesNotActiveListWidget->takeItem(j);
            }
        }

    }

}

