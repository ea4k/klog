#ifndef KLOG_INPUTWIDGETS_MAINWINDOWINPUTQSL_H
#define KLOG_INPUTWIDGETS_MAINWINDOWINPUTQSL_H
/***************************************************************************
                          mainwindowinputqsl.h  -  description
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
// This class implement the tab of the mainwindow that supports the QSL options
//

#include <QWidget>
#include <QtWidgets>
#include <QWidget>
#include "../dataproxy_sqlite.h"
#include "../utilities.h"

class MainWindowInputQSL : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindowInputQSL(DataProxy_SQLite *dp, QWidget *parent = nullptr);
    ~MainWindowInputQSL();

    QString getQSLRecStatus();
    QString getQSLSenStatus();
    QString getSentVia();
    QString getRecVia();
    QString getQSLVia();
    QString getQSLMsg();

    void setQSLRecStatus(const QString &_qs);
    void setQSLSenStatus(const QString &_qs);
    void setQSLRecVia(const QString &_qs);
    void setQSLSenVia(const QString &_qs);
    void setQSLVia(const QString &_qs, QColor qColor=Qt::black);
    void setQSLMsg(const QString &_qs);

    QDate getQSLRecDate();
    QDate getQSLSenDate();
    void setQSLRecDate(const QDate _qs);
    void setQSLSenDate(const QDate _qs);

    void clear();
    void qslViaClear();

signals:
    void returnPressed();

private slots:
    void slotQSLViaTextChanged();
    void slotQSLSentComboBoxChanged();
    void slotQSLRecvComboBoxChanged();
    void slotQSLViaLineEditReturnPressed();
/*
    void sloteQSLRecvComboBoxChanged();
    void sloteQSLSentComboBoxChanged();
    void slotLotwRecvComboBoxChanged();
    void slotLotwSentComboBoxChanged();
*/
private:
    void createUI();
    void setDefaultData();

    QComboBox *qslSentComboBox, *qslRecComboBox, *qslSentViaComboBox, *qslRecViaComboBox;
    QDateEdit *qslSentQDateEdit, *qslRecQDateEdit;
    QTextEdit *qslmsgTextEdit;
    QLineEdit *qslViaLineEdit;

    DataProxy_SQLite *dataProxy;
    Utilities *util;

    QStringList qslSentStatusList, qslRcvdStatusList, qslViaList;

    QPalette palette; // To color some widgets
};

#endif // MAINWINDOWINPUTQSL_H
