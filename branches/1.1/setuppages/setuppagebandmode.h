#ifndef SETUPPAGEBANDMODE_H
#define SETUPPAGEBANDMODE_H

#include <QtWidgets>
#include <QSqlQuery>
#include <QStringList>
#include <QListWidget>
#include "dataproxy_sqlite.h"


class SetupPageBandMode : public QWidget
{
    Q_OBJECT
public:
    SetupPageBandMode(DataProxy_SQLite *dp, QWidget *parent=nullptr);
    ~SetupPageBandMode();


    QString getBands(); // 10m, 12m, 15m
    QString getModes(); //ssb, CW

    void setActiveBands(QStringList q);
    void setActiveModes(QStringList q);

signals:

public slots:

private:
    void addBands(QStringList _b); // read the available bands from the DB
    void addModes(QStringList _b); // read the available modes from the DB

    QListWidget *bandsListWidget, *modesListWidget;

    DataProxy_SQLite *dataProxy;

};

#endif // SETUPPAGEBANDMODE_H
