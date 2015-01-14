#ifndef DOWNLOADCTY_H
#define DOWNLOADCTY_H
#include <QObject>
#include <QString>
#include <QProgressDialog>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>


class DownLoadCTY : public QObject {
    Q_OBJECT
public:
    explicit DownLoadCTY(const QString _kontestDir);
    ~DownLoadCTY();
    int download();

private:

    void setTarget(const QString& t);

    QNetworkAccessManager manager;
    QNetworkReply* reply;

    QString target;
    int result; // enum QNetworkReply::NetworkError
    QString kontestDir;

private slots:
    void downloadFinished(QNetworkReply* data);
    void downloadProgress(qint64 received, qint64 total);
    void slotErrorManagement(QNetworkReply::NetworkError networkError);


signals:
    void actionReturnDownload(const int _i);
    void done();
    void actionShowProgres(qint64 received, qint64 total);
    void actionError(const int _i);


};
#endif // DOWNLOADCTY_H
