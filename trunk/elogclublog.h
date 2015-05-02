#ifndef ELOGCLUBLOG_H
#define ELOGCLUBLOG_H
#include <QObject>
#include <QString>
#include <QProgressDialog>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>


class eLogClubLog : public QObject {
    Q_OBJECT
public:
    explicit eLogClubLog();
    ~eLogClubLog();

    void setCredentials(const QString _call, const QString _email, const QString _pass);
    int sendQSO(const QStringList _qso);

    //int download();

private:
    QString getClubLogAdif(const QStringList _q);
    QString call, email, pass, api;

    QNetworkAccessManager *manager;
    QNetworkReply* reply;

    int result;
    QString target;

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
