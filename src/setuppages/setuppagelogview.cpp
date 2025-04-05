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
   //qDebug() << Q_FUNC_INFO ;
    addFields(dataProxy->getFields());
    if (fieldsListWidget->count ()<1)
    {
       //qDebug() << Q_FUNC_INFO << " - No fields in the widget, populating with default ones";
        QStringList aux = {"qso_date", "call", "rst_sent", "rst_rcvd", "bandid", "modeid", "comment"};
        setActiveFields(aux);
    }
}

void SetupPageLogView::addFields(QStringList _b)
{
   //qDebug() << Q_FUNC_INFO << " - fields: " << _b.count();
    fieldsListWidget->clear();
    fieldsListWidget->addItems(_b);

    for(int i = 0; i < fieldsListWidget->count(); ++i)
    {
        QListWidgetItem* item = fieldsListWidget->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}

QStringList SetupPageLogView::getActiveFields()
{
    //qDebug() << Q_FUNC_INFO;
    QStringList activeFields;

    for (int i = 0; i < fieldsListWidget->count(); ++i)
    {
        QListWidgetItem* item = fieldsListWidget->item(i);
        if (item->checkState() == Qt::Checked)
        {
            activeFields.append(item->text());
        }
    }

    return activeFields;
}

void SetupPageLogView::setActiveFields(QStringList q)
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    if (q.isEmpty())
    {
       //qDebug() << Q_FUNC_INFO << " - Empty list - END-0";
        return;
    }

    QListWidgetItem *it;
    if ( (fieldsListWidget->count()) < 1)
    {
       //qDebug() << Q_FUNC_INFO << " No fields in the widget to select - END-1";
        return;
    }

    for (int i = 0; i < fieldsListWidget->count(); i++)
    {
        it = fieldsListWidget->item(i);
        it->setCheckState(Qt::Unchecked);

        for (int j=0;j<q.length();j++)
        {
           //qDebug() << Q_FUNC_INFO << " - Checking: " << q.at(j);
            if (it->text() == q.at(j))
            {
               //qDebug() << Q_FUNC_INFO << " - Adding: " << q.at(j);
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
    QSettings settings(util.getCfgFile (), QSettings::IniFormat);
    //settings.beginGroup ("LogView");
    settings.setValue ("LogViewFields", getActiveFields ());
    //settings.endGroup ();
}

void SetupPageLogView::loadSettings()
{
   //qDebug() << Q_FUNC_INFO << " - Start";
    Utilities util(Q_FUNC_INFO);
    QSettings settings(util.getCfgFile(), QSettings::IniFormat);
    QStringList aux = dataProxy->filterValidFields(settings.value("LogViewFields").toStringList());
    setActiveFields(aux);
   //qDebug() << Q_FUNC_INFO << " - END";
}
