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
    int deleteQSO(const QStringList _qso);

    //int download();

private:
    QString getClubLogAdif(const QStringList _q);
    int sendData(const QString _q);                     //  Sends the data (http post) to ClubLog
    QString prepareToTranslate(const QString _m);       //  Get the message and put it in a tr to be able to translate it

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
    void showMessage(const QString _t);


};
#endif // DOWNLOADCTY_H
