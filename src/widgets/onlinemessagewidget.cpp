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


}
