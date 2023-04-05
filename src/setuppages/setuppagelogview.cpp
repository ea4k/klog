/***************************************************************************
                          setuppagelogview.cpp  -  description
                             -------------------
    begin                : ago 2021
    copyright            : (C) 2021 by Jaime Robles
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

#include "setuppagelogview.h"

SetupPageLogView::SetupPageLogView(DataProxy_SQLite *dp, QWidget *parent) : QWidget(parent)
{
       //qDebug() << "SetupPageLogView::SetupPageLogView"  ;
    dataProxy = dp;

    fieldsListWidget = new QListWidget;

    QHBoxLayout *layout = new QHBoxLayout;
    QVBoxLayout *fLayout = new QVBoxLayout;


    QLabel *fieldsLabel = new QLabel(fieldsListWidget);
    fieldsLabel->setText(tr("Fields"));
    fieldsLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    addFields(dataProxy->getFields());


    fLayout->addWidget(fieldsLabel);
    fLayout->addWidget(fieldsListWidget);

    layout->addLayout(fLayout);

    setLayout(layout);
       //qDebug() << "SetupPageLogView::SetupPageLogView - END"  ;
}

SetupPageLogView::~SetupPageLogView()
{}

void SetupPageLogView::init()
{
    if (fieldsListWidget->count ()<1)
    {
        QStringList aux;
        aux.clear ();
        aux << "qso_date" << "call" << "rst_sent" << "rst_rcvd" << "bandid" << "modeid" << "comment";
        aux.removeDuplicates();
        setActiveFields(aux);
    }
}

void SetupPageLogView::addFields(QStringList _b)
{
    fieldsListWidget->addItems(_b);

    QListWidgetItem* item = 0;
    for(int i = 0; i < fieldsListWidget->count(); ++i){
        item = fieldsListWidget->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}

QStringList SetupPageLogView::getActiveFields()
{
    //qDebug() << Q_FUNC_INFO;
    if ( (fieldsListWidget->count()) < 1)
    {
        return QStringList();
    }
     QListWidgetItem *it;
     QString b = QString();
    for (int i = 0; i < fieldsListWidget->count(); i++)
    {
        it = fieldsListWidget->item(i);
        if (it->checkState() == Qt::Checked)
        {
            b = b + it->text();
            b = b + ", ";
        }
    }
    if (b.size()>=2)
    {
        b.chop(2);
    }
    //qDebug() << Q_FUNC_INFO << " : " << b;
    return b.split(", ", Qt::SkipEmptyParts);
}

void SetupPageLogView::setActiveFields(QStringList q)
{
   //qDebug() << Q_FUNC_INFO << " - Start";

    if (q.isEmpty())
    {return;}

    QListWidgetItem *it;

    if ( (fieldsListWidget->count()) < 1)
    {
        //qDebug() << Q_FUNC_INFO << " - END-1";
        return;
    }

    for (int i = 0; i < fieldsListWidget->count(); i++)
    {
        it = fieldsListWidget->item(i);
        it->setCheckState(Qt::Unchecked);

        for (int j=0;j<q.length();j++)
        {
            if (it->text() == q.at(j))
            {
                //qDebug() << Q_FUNC_INFO << " - Checking: " << q.at(j);
                it->setCheckState(Qt::Checked);
            }
        }
    }
    //qDebug() << Q_FUNC_INFO << " - END";
}

void SetupPageLogView::saveSettings()
{
    //qDebug() << Q_FUNC_INFO ;
    Utilities util(Q_FUNC_INFO);
    QSettings settings(util.getSetFile (), QSettings::IniFormat);
    //settings.beginGroup ("LogView");
    settings.setValue ("LogViewFields", getActiveFields ());
    //settings.endGroup ();
}

void SetupPageLogView::loadSettings()
{

    //qDebug() << Q_FUNC_INFO << " - Start";
    Utilities util(Q_FUNC_INFO);
    QSettings settings(util.getSetFile (), QSettings::IniFormat);
    QStringList aux;
    aux.clear();
    aux << dataProxy->filterValidFields(settings.value("LogViewFields").toStringList ());
    aux.removeDuplicates();
    setActiveFields(aux);
    //qDebug() << Q_FUNC_INFO << " - END";
}
