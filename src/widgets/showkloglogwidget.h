#ifndef SHOWKLOGLOGWIDGET_H
#define SHOWKLOGLOGWIDGET_H
/***************************************************************************
                          showkloglogwidget.h  -  description
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
#include <QObject>
#include <QWidget>
#include <QtWidgets>
#include <QListView>
#include <QStringListModel>
#include "../utilities.h"

// This widget will show the KLog log, the software log, to be able to debug
// or see how it is executing. Mainly for debug & development.

class ShowKLogLogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowKLogLogWidget(QWidget *parent = nullptr);
    void addLog(QString const &_func, QString const &_log, const DebugLogLevel _l);
signals:

public slots:

private:
    void createUI();
    QStringList list;
    QListView *logsView;
    QStringListModel *model;
    QComboBox *levelComboBox;

};

#endif // SHOWKLOGLOGWIDGET_H
