#include "dxccstatuswidget.h"

DXCCStatusWidget::DXCCStatusWidget(QWidget *parent) : QWidget(parent)
{
    dxccView = new QTableView;
    dxccView->setContextMenuPolicy(Qt::CustomContextMenu);
    dxccView->setSortingEnabled(true);



}

