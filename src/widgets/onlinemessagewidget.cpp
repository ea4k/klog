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


int OnlineMessageWidget::showMessage(QNetworkReply::NetworkError _error, OnLineProvider _prov, OnlineErrorCode _onlineError, OnlineErrorReason _onlineReason, const QString &_msg)
{
    //enum OnLineProvider {ClubLog, LoTW, eQSL, QRZ, HamQTH};
    QString message = QString();
    QString detailedText = QString(tr("The server returned the following error: %1")).arg(translate(_error));
    QString title = QString();

    switch (_onlineReason)
    {
    case OnlineErrorReason::Other:
    break;
    case OnlineErrorReason::Auth:
    break;
    case OnlineErrorReason::DupeQSO:
    break;
    case OnlineErrorReason::WrongLogBook:
    break;
    }

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

    if ((title.length()<1) | (message.length()<1) | (detailedText.length()<1))
    {
        return -100;
    }

    QMessageBox msgBox;
    if (_onlineError == OnlineErrorCode::Ok)
    {
      msgBox.setIcon(QMessageBox::Warning);
    }
    else
    {
        msgBox.setIcon(QMessageBox::Information);
    }

    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.setDetailedText(detailedText);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    return msgBox.exec();


}


QString OnlineMessageWidget::translate(QNetworkReply::NetworkError _error)
{
    switch (_error) {
        case QNetworkReply::NoError:
            return QString::number(_error);
        break;
        case QNetworkReply::ConnectionRefusedError:
        break;
        case  QNetworkReply::RemoteHostClosedError:
        break;
        case QNetworkReply::HostNotFoundError:
        break;
    case QNetworkReply::TimeoutError:
        break;
    case QNetworkReply::OperationCanceledError:
        break;
    case QNetworkReply::SslHandshakeFailedError:
        break;
    case QNetworkReply::TemporaryNetworkFailureError:
        break;
    case QNetworkReply::NetworkSessionFailedError:
        break;
    case QNetworkReply::BackgroundRequestNotAllowedError:
        break;
    case QNetworkReply::TooManyRedirectsError:
        break;
    case QNetworkReply::InsecureRedirectError:
        break;
    case QNetworkReply::ProxyConnectionRefusedError:
        break;
    case QNetworkReply::ProxyConnectionClosedError:
        break;
    case QNetworkReply::ProxyNotFoundError:
        break;
    case QNetworkReply::ProxyTimeoutError:
        break;
    case QNetworkReply::ProxyAuthenticationRequiredError:
        break;
    case QNetworkReply::ContentAccessDenied:
        break;
    case QNetworkReply::ContentOperationNotPermittedError:
        break;
    case QNetworkReply::ContentNotFoundError:
        break;
    case QNetworkReply::AuthenticationRequiredError:
        break;
    case QNetworkReply::ContentReSendError:
        break;
    case QNetworkReply::ContentConflictError:
        break;
    case QNetworkReply::ContentGoneError:
        break;
    case QNetworkReply::InternalServerError:
        break;
    case QNetworkReply::OperationNotImplementedError:
        break;
    case QNetworkReply::ServiceUnavailableError:
        break;
    case QNetworkReply::ProtocolUnknownError:
        break;
    case QNetworkReply::ProtocolInvalidOperationError:
        break;
    case QNetworkReply::UnknownNetworkError:
        break;
    case QNetworkReply::UnknownProxyError:
        break;
    case QNetworkReply::UnknownContentError:
        break;
    case QNetworkReply::ProtocolFailure:
        break;
    case QNetworkReply::UnknownServerError:
        break;
    default:
        return QString(tr("Not identified"));
        break;
    }

    return QString::number(_error);
}
