#ifndef FILEAWARDMANAGER_H
#define FILEAWARDMANAGER_H

#include <QObject>
#include <QFileDialog>
#include <QFile>
#include "utilities.h"
#include "dataproxy_sqlite.h"
#include "world.h"


class FileAwardManager : public QWidget
{
    Q_OBJECT
public:
    explicit FileAwardManager(DataProxy_SQLite *dp, const QString &_parentFunction);
    bool importNewAwardFile();

signals:

public slots:

private:
    void showError(const QString _header, const QString _msg, const QString _detailedMsg);
    DataProxy_SQLite *dataProxy;
    Utilities *util;
    World *world;

};

#endif // FILEAWARDMANAGER_H
