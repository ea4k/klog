#ifndef ONLINEMESSAGEWIDGET_H
#define ONLINEMESSAGEWIDGET_H
/***************************************************************************
                          onlinemessagewidget.h  -  description
                             -------------------
    begin                : November 2020
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
#include <QObject>
#include <QWidget>
#include <QMessageBox>
#include <QNetworkReply>
#include "../utilities.h"

class OnlineMessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OnlineMessageWidget(QWidget *parent = nullptr);
    int showMessage(QNetworkReply::NetworkError _error, OnLineProvider _prov, OnlineErrorCode _onlineError, OnlineErrorReason _onlineReason, const QString &_msg);

signals:

public slots:

private:
    QString translate(QNetworkReply::NetworkError _error);

};

#endif // ONLINEMESSAGEWIDGET_H
