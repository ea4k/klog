#ifndef DOWNLOADCTY_H
#define DOWNLOADCTY_H
#include <QObject>
#include <QString>
#include <QProgressDialog>
#include <QFile>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslError>
#include <QTimer>
#include <QUrl>
#include <QMessageBox>
#include "utilities.h"

class QSslError;

class DownLoadCTY : public QObject {
    Q_OBJECT
public:
    explicit DownLoadCTY(const QString _kontestDir, const QString _klogVersion);
    ~DownLoadCTY();
    int download();

private:
    Utilities *util;

    //void setTarget(const QString& t);
    bool saveToDisk(const QString &filename, QIODevice *data);
    QString saveFileName(const QUrl &url);

    QNetworkAccessManager *manager;
    QNetworkRequest *request;


    //QString target;
    int result; // enum QNetworkReply::NetworkError
    QString kontestDir;
    QString urld;
    QUrl *url;

private slots:
    void slotDownloadFinished(QNetworkReply* reply);
    void slotDownloadProgress(qint64 received, qint64 total);
    void slotErrorManagement(QNetworkReply::NetworkError networkError);



signals:
    void actionReturnDownload(const int _i);
    void done();
    void actionShowProgres(qint64 received, qint64 total);
    void actionError(const int _i);


};
#endif // DOWNLOADCTY_H
