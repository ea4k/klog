#ifndef MAINWINDOWINPUTEQSL_H
#define MAINWINDOWINPUTEQSL_H
/***************************************************************************
                          mainwindowinputeqsl.h  -  description
                             -------------------
    begin                : jun 2017
    copyright            : (C) 2017 by Jaime Robles
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
//
// This class implement the tab of the mainwindow that supports the eQSL options
//
#include <QWidget>
#include <QtWidgets>
#include <QWidget>
#include "../dataproxy_sqlite.h"
#include "../utilities.h"

class MainWindowInputEQSL : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindowInputEQSL(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    //~MainWindowInputEQSL();

    //void setData(const QString &_comment);
    //QString getComment();

    void clear();

    QString getQRZCOMStatus();
    QString getClubLogStatus();
    QString getEQSLRecStatus();
    QString getEQSLSenStatus();
    QString getLOTWRecStatus();
    QString getLOTWSenStatus();

    void setQRZCOMStatus(const QString &_qs);
    void setClubLogStatus(const QString &_qs);
    void setEQSLRecStatus(const QString &_qs);
    void setEQSLSenStatus(const QString &_qs);
    void setLOTWRecStatus(const QString &_qs);
    void setLOTWSenStatus(const QString &_qs);

    QDate getQRZCOMDate();
    QDate getClubLogDate();
    QDate getEQSLRecDate();
    QDate getEQSLSenDate();
    QDate getLOTWRecDate();
    QDate getLOTWSenDate();

    void setQRZCOMDate(const QDate _qs);
    void setClubLogDate(const QDate _qs);
    void setEQSLRecDate(const QDate _qs);
    void setEQSLSenDate(const QDate _qs);
    void setLOTWRecDate(const QDate _qs);
    void setLOTWSenDate(const QDate _qs);

    void setQueueSentByDefault(const bool _b);


signals:

private slots:
    void slotQRZCOMComboBoxChanged();
    void slotClubLogComboBoxChanged();
    void sloteQSLRecvComboBoxChanged();
    void sloteQSLSentComboBoxChanged();
    void slotLotwRecvComboBoxChanged();
    void slotLotwSentComboBoxChanged();

private:
    void createUI();
    void setDefaultData();

    QComboBox *eqslSentComboBox, *eqslRecComboBox, *lotwSentComboBox, *lotwRecComboBox, *clublogComboBox, *qrzcomComboBox;
    QDateEdit *eqslSentQDateEdit, *eqslRecQDateEdit, *lotwSentQDateEdit, *lotwRecQDateEdit, *clublogQDateEdit,  *qrzcomQDateEdit;


    DataProxy_SQLite *dataProxy;
    Utilities *util;

    QStringList qslSentStatusList, qslRcvdStatusList, clubLogStatusList;
    bool queueSentByDefault;

};

#endif // MAINWINDOWINPUTEQSL_H
