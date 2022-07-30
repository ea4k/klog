/***************************************************************************
                          showkloglogwidget.cpp  -  description
                             -------------------
    begin                : jan 2021
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

#include "showkloglogwidget.h"

ShowKLogLogWidget::ShowKLogLogWidget(QWidget *parent) : QWidget(parent)
{
    levelComboBox = new QComboBox;
    model = new QStringListModel();
    //QStringList list;
    list.clear();

    model->setStringList(list);
    logsView = new QListView;
    logsView->setModel(model);
    createUI();

}

void ShowKLogLogWidget::createUI()
{
    QStringList levels;
    levels.clear();
    //TODO: Link the Level tags to the utilities.h definition
    levels << "1-Info" << "2-Debug";
    levelComboBox->clear();
    levelComboBox->addItems(levels);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(levelComboBox);
    layout->addWidget(logsView);
    setLayout(layout);
}


void ShowKLogLogWidget::add(const QString &_func, QString const &_log, const DebugLogLevel _l)
{
    if (logLevel >_l)
    {
        return;
    }
    QString msg;
    Utilities util;
    msg = QString("%1 %2 - %3 - %4").arg(QDateTime::currentDateTime().toString("yyyy-MM-ddThh:mm:ss")).arg(util.debugLevelToString (_l)).arg(_func).arg(_log);

    qDebug() << "Debugging: " << msg;

    if(model->insertRow(0)) {
        QModelIndex index = model->index(0, 0);
        model->setData(index, msg);
    }
}

void ShowKLogLogWidget::setLogLevel(const DebugLogLevel _l)
{
    logLevel = _l;
}
