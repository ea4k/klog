#ifndef TIPSDIALOG_H
#define TIPSDIALOG_H


/***************************************************************************
                          tipsdialog.h  -  description
                             -------------------
    begin                : sept 2019
    copyright            : (C) 2019 by Jaime Robles
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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/

#include <QDialog>
#include <QLabel>
#include <QTextEdit>


#include <QTabWidget>
#include "klogdefinitions.h"



class TipsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TipsDialog(QWidget *parent = nullptr);
    ~TipsDialog();
    void setSeverity(const DebugLogLevel _sev);

signals:
    void debugLog (QString _func, QString _msg, DebugLogLevel _level);
    void fillInQSOSignal();
    void fillInDXCCSignal();
    void findQSL2QSOSignal();
    void fileExportToPrintSignal();
    //void fileExportForLoTWSignal();
    void fileOpenKLogFolderSignal();
    void toolSendPendingQSLSignal();
    void toolRecPendingQSLSignal();
    void toolRecRecPendingQSLSignal();
    void toolsUploadLoTWSignal();

private slots:
    void slotPrevButtonClicked();
    void slotNextButtonClicked();
    void slotLinkActivated(const QString &_link);

private:
    void setTip(const int _t);

    QLabel *tipTextQLabel;
    //QTextEdit *tipTextEdit;
    QWidget *tip;
    DebugLogLevel logSeverity;
    int tipId, tipMax;
    QString description;


};



#endif // TIPSDIALOG_H
