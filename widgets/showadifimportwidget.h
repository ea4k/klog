#ifndef SHOWADIFIMPORTPORTWIDGET_H
#define SHOWADIFIMPORTPORTWIDGET_H

#include <QtWidgets>
#include "dataproxy_sqlite.h"
#include "utilities.h"


class ShowAdifImportWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ShowAdifImportWidget(DataProxy_SQLite *dp, const QString &_parentFunction, QWidget *parent = nullptr);
    void addQSOToTheList(const QStringList _qso);
    void clear();


protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);

private slots:
    void slotOKPushButtonClicked();
   // void slotCancelPushButtonClicked();

signals:
 //void selection(QString _st, QDate _startD, QDate _endD);

private:
    void createUI();
    void fillTable();    
    void addQSO(const QStringList _qso);

    QList<QStringList> qsosList;
    DataProxy_SQLite *dataProxy;
    Utilities *util;
    QPushButton *okButton;//, *cancelButton;

    QTableWidget *tableWidget;
    QHeaderView *hv, *hh;

};


#endif
