#ifndef SOFTWAREUPDATE_H
#define SOFTWAREUPDATE_H

#include <QCoreApplication>
#include <QMessageBox>
#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
//#include <QFile>
//#include <QDirIterator>
#include <QList>
#include <QRegularExpression>
#include <QDebug>
#include "utilities.h"
#include "softwareupdatedialog.h"


class QSslError;

class SoftwareUpdate: public QObject {
    Q_OBJECT

public:
    SoftwareUpdate(const QString _klogVersion);
    ~SoftwareUpdate();

    void addCall(const QString _call);

    void needToUpdate();

private slots:
    void slotReadyRead();
    void slotError(int _p);
    //void slotSslErrors(QList _p);
    //void replyFinished(QNetworkReply *data);

    void slotDownloadFinished(QNetworkReply *reply);

signals:
    void updateNeededSignal(const bool _q); // Will be TRUE if updated if needed and FALSE if we already have the latest version

private:
    bool checkUpdates(QIODevice *data);
    void updateNeeded(QString _newVer);
    void setHeader();

    Utilities *util;

    QString klogVersion, latestVersion, callsign;
    QString urld;
    QUrl *url;
    
    QNetworkAccessManager *manager;
    QNetworkRequest request;

    SoftwareUpdateDialog *updateDialog;

    int result;
    bool toUpdate;

};

#endif // SOFTWAREUPDATE_H



