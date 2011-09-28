/****************************f***********************************************
                          klognetwork.h  -  Network class of KLog
                          
  Manages the download of the CTY.DAT file. Most of the code comes from the QT examples.
                             -------------------
    begin                : mar  30 20:00:00 CET 2010
    copyright            : (C) 2010 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/******************************************************************************
 *                                                                            *
 *   This program is free software; you can redistribute and/or modify     *
*  it under the terms of the GNU General Public License as published by     *
*  the Free Software Foundation; either version 2 of the License, or        *
*  (at your option) any later version.                                      *
 *                                                                            *
*  This program is distributed in the hope that it will be useful,          *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
*  GNU General Public License for more details.                             *
 *                                                                            *
*  You should have received a copy of the GNU General Public License        *
*  along with this program; if not, write to the Free Software              *
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA*
 *****************************************************************************/

#ifndef KLOG_NET_H
#define KLOG_NET_H

#include <QDialog>
#include <KDE/KLocale>

class QDialogButtonBox;
class QFile;
class QHttp;
class QHttpResponseHeader;
class QLabel;
class QLineEdit;
class QProgressDialog;
class QPushButton;
class QSslError;
class QAuthenticator;

class KLogNetwork : public QDialog {
    Q_OBJECT

public:
    KLogNetwork(QWidget *parent = 0);

private slots:
    void downloadFile();
    void cancelDownload();
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QHttpResponseHeader &responseHeader);
    void updateDataReadProgress(int bytesRead, int totalBytes);
    //void enableDownloadButton();
    void slotAuthenticationRequired(const QString &, quint16, QAuthenticator *);
#ifndef QT_NO_OPENSSL
    void sslErrors(const QList<QSslError> &errors);
#endif

private:
    QLabel *statusLabel;
    QLabel *urlLabel, *statusLabel2;
    QLineEdit *urlLineEdit;
    QProgressDialog *progressDialog;
    QPushButton *downloadButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;

    QHttp *http;
    QFile *file;
    int httpGetId;
    bool httpRequestAborted;
};

#endif
