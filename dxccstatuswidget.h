#ifndef DXCCSTATUSWIDGET_H
#define DXCCSTATUSWIDGET_H

#include <QWidget>
#include <QtWidgets>
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
    void setBands(QStringList const _ent); // Receives the list of bandIDs

signals:

public slots:
    void slotSearchLineEditTextChanged();
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
    QLineEdit *searchLineEdit;
    QPushButton *refreshButton;

    int numberOfColumns; // Columns will be number Of Bands + 2 (Id + Name)
    QStringList bandNames, validBands;
};

#endif // DXCCSTATUSWIDGET_H
