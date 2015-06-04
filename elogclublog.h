#ifndef ELOGCLUBLOG_H
#define ELOGCLUBLOG_H
#include <QObject>
#include <QFile>
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
    int sendQSO(QStringList _qso);
    int deleteQSO(QStringList _qso);
    int modifyQSO (QStringList _oldQSO, QStringList _newQSO);

    //int sendLogFile(const QFile _f);

    //int download();

private:
    QString getClubLogAdif(const QStringList _q);
    int sendData(const QString _q);                     //  Sends the data (http post) to ClubLog
    QString prepareToTranslate(const QString _m);       //  Get the message and put it in a tr to be able to translate it

    QString call, email, pass, api;

    QNetworkAccessManager *manager;
    QNetworkReply* reply;
    int currentQSO;
    int result;
    QString target;

private slots:
    void slotQsoUploadFinished(QNetworkReply* data);
    void slotFileUploadFinished(QNetworkReply* data);
    void downloadProgress(qint64 received, qint64 total);
    void slotErrorManagement(QNetworkReply::NetworkError networkError);


signals:
    void actionReturnDownload(const int _i, const int _qsoId);
    void done();
    void actionShowProgres(qint64 received, qint64 total);
    void actionError(const int _i);
    void showMessage(const QString _t);


};
#endif // DOWNLOADCTY_H
