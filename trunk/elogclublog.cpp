#include "elogclublog.h"

eLogClubLog::eLogClubLog()
{
/*
 QNetworkRequest request;
 request.setUrl(QUrl("http://qt.nokia.com"));
 request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");

 QNetworkReply *reply = manager->get(request);
 connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
 connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
         this, SLOT(slotError(QNetworkReply::NetworkError)));
 connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
         this, SLOT(slotSslErrors(QList<QSslError>)));

*/

}

//realtimeURL: https://secure.clublog.org/realtime.php
/*
  Form variables
•email: A registered email address in Club Log
•password: The password to authenticate the email login
•callsign: The callsign into which the logs should be uploaded (must belong to the announced email address).
•adif: A text field containing exactly one ADIF record, ending in <EOR>. Any additional records are ignored.
•api: An API key to access this interface (protecting it from abuse), which you can obtain by emailing the helpdesk.


*/

int eLogClubLog::sendQSO(const QString _qso)
{
    return -1;
}

QString eLogClubLog::getCLubLogAdif(const QStringList _q)
{
    // _qso must include 16 ordered fields than can be empty or contain data. This function builds the ADIF QSO
/* http://clublog.freshdesk.com/support/solutions/articles/53202-which-adif-fields-does-club-log-use-
ClubLog only accepts the following ADIF fields:
  •QSO_DATE
  •TIME_ON
  •QSLRDATE
  •QSLSDATE
  •CALL
  •OPERATOR
  •MODE
  •BAND
  •BAND_RX
  •FREQ
  •QSL_RCVD
  •LOTW_QSL_RCVD
  •QSL_SENT
  •DXCC
  •PROP_MODE
  •CREDIT_GRANTED
*/

    /*
    if (_qso.size()!=16)
    {
        return "";
    }

    QStringList _q;
    _q.clear();
    _q << _qso;
*/
  //  if (_q.at(0)).length()>



    return "";
}
