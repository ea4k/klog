#ifndef DXCCSTATUSWIDGET_H
#define DXCCSTATUSWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QString>
//#include <QTableView>
#include "awards.h"
#include "world.h"
#include "dataproxy.h"
#include "dataproxy_sqlite.h"


//TODO: Creating the widget to show the DXCC status

class DXCCStatusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DXCCStatusWidget(QWidget *parent = 0);
    ~DXCCStatusWidget();
    
    void update();    
    void setBands(const QStringList _ent, const bool _creating = false); // Receives the list of bandIDs
    void setCurrentLog(const int _logN);

signals:

public slots:
    //void slotSearchLineEditTextChanged();
    void slotRefreshButtonClicked();
private:
    void createUI();
    void setDefaultBands();
    void addEntity(QStringList const _ent); // DXCC id, bandid, bandid, ...
    //QStringList sortBandNamesBottonUp(const QStringList _qs);

    QTableWidget *dxccView;
    Awards *awards;
    World *world;
    DataProxy *dataProxy;

    QHeaderView *hv, *hh;
    //QLineEdit *searchLineEdit;
    QPushButton *refreshButton;
    //QRadioButton *showAllLogsButton;


    int numberOfColumns; // Columns will be number Of Bands + 2 (Id + Name)
    QStringList bandNames, validBands;
    int logNumber, tempLog; // log in use in the log / log to be used in the widget

};

#endif // DXCCSTATUSWIDGET_H
