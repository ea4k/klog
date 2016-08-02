#ifndef DXCCSTATUSWIDGET_H
#define DXCCSTATUSWIDGET_H

#include <QWidget>
#include <QTableView>

//TODO: Creating the widget to show the DXCC status

class DXCCStatusWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DXCCStatusWidget(QWidget *parent = 0);

signals:

public slots:

private:
    QTableView *dxccView;
};

#endif // DXCCSTATUSWIDGET_H
