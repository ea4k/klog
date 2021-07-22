/***************************************************************************
                          onlinemessagewidget.cpp  -  description
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
#include "onlinemessagewidget.h"

OnlineMessageWidget::OnlineMessageWidget(QWidget *parent) : QWidget(parent)
{

}


int OnlineMessageWidget::showMessage(QNetworkReply::NetworkError _error, OnLineProvider _prov, OnlineErrorCode _onlineError, OnlineErrorReason _onlineReason, const QString _msg)
{
    //enum OnLineProvider {ClubLog, LoTW, eQSL, QRZ, HamQTH};
    QString message = QString();
    QString detailedText = QString();
    QString title = QString();

    switch (_prov)
    {
    case ClubLog:
    {

    }
        break;
    case LoTW:
    {}
        break;
    case QRZ:
    {}
        break;
    case eQSL:
    {}
        break;
    }

    if ((title.length()<1) | (message.length()<1) | detailedText.length()<1)
    {
        return -100;
    }

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.setDetailedText(detailedText);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    return msgBox.exec();


}


QString OnlineMessageWidget::translate(const QString _msg)
{
    return _msg;
}
