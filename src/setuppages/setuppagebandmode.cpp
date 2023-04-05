/***************************************************************************
                          setuppagebandmode.cpp  -  description
                             -------------------
    begin                : nov 2011
    copyright            : (C) 2011 by Jaime Robles
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

#include "../setuppages/setuppagebandmode.h"

SetupPageBandMode::SetupPageBandMode(DataProxy_SQLite *dp, QWidget *parent) : QWidget(parent)
{
       //qDebug() << "SetupPageBandMode::SetupPageBandMode"  ;
    dataProxy = dp;

    bandsListWidget = new QListWidget;
    modesListWidget = new QListWidget;
    QHBoxLayout *layout = new QHBoxLayout;

    QVBoxLayout *bLayout = new QVBoxLayout;
    QVBoxLayout *mLayout = new QVBoxLayout;

    QLabel *bandsLabel = new QLabel(bandsListWidget);
    bandsLabel->setText(tr("Bands"));
    bandsLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *modesLabel = new QLabel(modesListWidget);
    modesLabel->setText(tr("Modes"));
    modesLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    addBands(dataProxy->getBands());
    addModes(dataProxy->getModes());

    bLayout->addWidget(bandsLabel);
    bLayout->addWidget(bandsListWidget);

    mLayout->addWidget(modesLabel);
    mLayout->addWidget(modesListWidget);

    layout->addLayout(bLayout);
    layout->addLayout(mLayout);
    //layout->addWidget(bandsListWidget);
    //layout->addWidget(modesListWidget);

    setLayout(layout);
       //qDebug() << "SetupPageBandMode::SetupPageBandMode - END"  ;
}

SetupPageBandMode::~SetupPageBandMode()
{}


void SetupPageBandMode::addBands(QStringList _b)
{
    bandsListWidget->addItems(_b);

    QListWidgetItem* item = 0;
    for(int i = 0; i < bandsListWidget->count(); ++i){
        item = bandsListWidget->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}


void SetupPageBandMode::addModes(QStringList _b)
{
    modesListWidget->addItems(_b);

    QListWidgetItem* item = 0;
    for(int i = 0; i < modesListWidget->count(); ++i){
        item = modesListWidget->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}


QStringList SetupPageBandMode::getBands()
{
       //qDebug() << "SetupPageBandMode::getBands";
  if ( (bandsListWidget->count()) < 1)
    {
        return QStringList();
    }
    QListWidgetItem *it;
    QStringList _bands;
    _bands.clear();
    for (int i = 0; i < bandsListWidget->count(); i++)
    {
        it = bandsListWidget->item(i);
        if (it->checkState() == Qt::Checked)
        {
            _bands.append (it->text());
        }
    }
    return _bands;
}

QStringList SetupPageBandMode::getModes()
{
       //qDebug() << "SetupPageBandMode::getModes";


    if ( (modesListWidget->count()) < 1)
    {
        return QStringList();
    }
    QListWidgetItem *it;
    QStringList _modes;
    _modes.clear ();
    for (int i = 0; i < modesListWidget->count(); i++)
    {
        it = modesListWidget->item(i);

        if (it->checkState() == Qt::Checked)
        {
            _modes.append (it->text());
        }
    }
    return _modes;
}


void SetupPageBandMode::setActiveBands(QStringList q)
{
       //qDebug() << "SetupPageBandMode::setActiveBands";

    if (q.isEmpty())
    {return;}

    //QString b;
    QListWidgetItem *it;

    if ( (bandsListWidget->count()) < 1)
    {
        return;
    }

    for (int i = 0; i < bandsListWidget->count(); i++)
    {
        it = bandsListWidget->item(i);
        it->setCheckState(Qt::Unchecked);

        for (int j=0;j<q.length();j++)
        {
            if (it->text() == q.at(j))
            {
                it->setCheckState(Qt::Checked);
            }
        }
    }
}

void SetupPageBandMode::setActiveModes(QStringList q)
{
       //qDebug() << "SetupPageBandMode::setActiveModes";

    if (q.isEmpty())
    {return;}

    //QString b;
    QListWidgetItem *it;

    if ( (modesListWidget->count()) < 1)
    {
        return;
    }

    for (int i = 0; i < modesListWidget->count(); i++)
    {
        it = modesListWidget->item(i);
        it->setCheckState(Qt::Unchecked);

        for (int j=0;j<q.length();j++)
        {
            if (it->text() == q.at(j))
            {
                it->setCheckState(Qt::Checked);
            }
        }
    }
}

void SetupPageBandMode::saveSettings()
{
    //qDebug() << Q_FUNC_INFO ;
    util = new Utilities(Q_FUNC_INFO);
    QSettings settings(util->getSetFile (), QSettings::IniFormat);
    settings.beginGroup ("BandMode");
    settings.setValue ("Bands", getBands ());
    settings.setValue ("Modes", getModes ());
    settings.endGroup ();
}
