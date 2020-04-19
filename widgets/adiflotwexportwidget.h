#ifndef ADIFLOTWEXPORTWIDGET_H
#define ADIFLOTWEXPORTWIDGET_H

#include <QtWidgets>
#include "dataproxy_sqlite.h"

enum ExportMode {ModeLotW, ModeADIF};

class AdifLoTWExportWidget : public QWidget
{
    Q_OBJECT


public:
    explicit AdifLoTWExportWidget(DataProxy_SQLite *dp, const QString &_parentFunction, QWidget *parent = nullptr);
    void setExportMode(const ExportMode _EMode);

protected:
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);

private slots:
    void slotOKPushButtonClicked();
    void slotCancelPushButtonClicked();
    void slotStationCallsignChanged();
    void slotDateChanged();
signals:
    void selection(QString _st, QDate _startD, QDate _endD);

private:
    void createUI();
    void fillTable();
    void setTopLabel();
    void addQSO(const int _qsoID);
    void fillStationCallsignComboBox();


    DataProxy_SQLite *dataProxy;
    QComboBox *stationCallsignComboBox;
    QDateEdit *startDate, *endDate;
    QLabel *topLabel;

    QPushButton *okButton, *cancelButton;
    ExportMode selectedEMode;

    QTableWidget *tableWidget;
    QHeaderView *hv, *hh;

};


#endif
