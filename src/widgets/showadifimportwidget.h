#ifndef KLOG_WIDGETS_SHOWADIFIMPORTPORTWIDGET_H
#define KLOG_WIDGETS_SHOWADIFIMPORTPORTWIDGET_H
/***************************************************************************
                          showadifimportwidget.h  -  description
                             -------------------
    begin                : July 2020
    copyright            : (C) 2020 by Jaime Robles
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
#include <QtWidgets>
#include "../dataproxy_sqlite.h"
#include "../utilities.h"


class ShowAdifImportWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ShowAdifImportWidget(DataProxy_SQLite *dp, const QString &_parentFunction, QWidget *parent = nullptr);
    ~ShowAdifImportWidget();
    void addQSOToTheList(const QStringList _qso);
    void clear();



protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);

private slots:
    void slotOKPushButtonClicked();
   // void slotCancelPushButtonClicked();

signals:
 //void selection(QString _st, QDate _startD, QDate _endD);

private:
    void createUI();
    void fillTable();
    void addQSO(const QStringList &_qso);

    QList<QStringList> qsosList;
    DataProxy_SQLite *dataProxy;
    Utilities *util;
    QPushButton *okButton;//, *cancelButton;

    QTableWidget *tableWidget;
    QHeaderView *hv, *hh;
};


#endif
