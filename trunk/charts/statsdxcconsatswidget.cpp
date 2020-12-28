#include "statsdxcconsatswidget.h"



StatsDXCCOnSatsWidget::StatsDXCCOnSatsWidget(DataProxy_SQLite *dp, QWidget *parent)
{
     //qDebug() << "StatsDxccOnSatsWidget::StatsDxccOnSatsWidget" << endl;

    dataProxy = dp;
    util = new Utilities;
    confirmedOnlyCheckBox = new QCheckBox;
    onlyLEOSatCheckBox = new QCheckBox;
    log = -1;
    numberLabel = new QLabel;


    //chart = new QChart();
    //chartView = new QChartView(chart);
    //chart->setTheme(QChart::ChartThemeQt);
    //chart->setTheme(QChart::ChartThemeBlueCerulean);

    createUI();
    //prepareChart();
}

void StatsDXCCOnSatsWidget::createUI()
{

    confirmedOnlyCheckBox->setText(tr("Show confirmed only"));
    confirmedOnlyCheckBox->setChecked(false);
    onlyLEOSatCheckBox->setText(tr("Only LEO sats", "LEO means Low Earth Orbiting and it is a well known word for hams. Do not translate if not sure."));
    onlyLEOSatCheckBox->setChecked(false);

    tableWidget = new QTableWidget(this);
    tableWidget->setRowCount(0);
    tableWidget->setColumnCount(7);
    tableWidget->resizeRowsToContents();
    tableWidget->sortByColumn(2);
    tableWidget->horizontalHeader()->setStretchLastSection(true);

    QLabel *textLabel = new QLabel;
    textLabel->setText(tr("Number"));
    textLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    numberLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    numberLabel->setText(QString::number(0));

    QHBoxLayout *labelLayout = new QHBoxLayout;
    labelLayout->addWidget(textLabel);
    labelLayout->addWidget(numberLabel);

    QHBoxLayout *checksLayout = new QHBoxLayout;
    checksLayout->addWidget(confirmedOnlyCheckBox);
    checksLayout->addWidget(onlyLEOSatCheckBox);
    checksLayout->addLayout(labelLayout);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(checksLayout);
    layout->addWidget(tableWidget);
    setLayout(layout);

    connect(confirmedOnlyCheckBox, SIGNAL(clicked()), this, SLOT(slotConfirmedClicked() ) );
    connect(onlyLEOSatCheckBox, SIGNAL(clicked()), this, SLOT(slotConfirmedClicked() ) );

}

void StatsDXCCOnSatsWidget::prepareChart(const int _log)
{
    //qDebug() << Q_FUNC_INFO << "Log = " << QString::number(_log) << endl;

    while(tableWidget->rowCount()>0)
    {
        tableWidget->removeRow(tableWidget->rowCount()-1);
    }

    log = _log;
    QList<QSO*> _qsos;
    _qsos.clear();
    _qsos << dataProxy->getSatDXCCStats(log);
    tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Call")));
    tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Date")));
    tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Band")));
    tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Mode")));
    tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem(tr("DXCC")));
    tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem(tr("Satellite")));
    tableWidget->setHorizontalHeaderItem(6, new QTableWidgetItem(tr("Confirmed")));
    tableWidget->setStyleSheet("QHeaderView::section { background-color:cornflowerblue }");
    //qDebug() << "StatsDxccOnSatsWidget::prepareChart: QSOs: " << QString::number(_qsos.length()) << endl;

    int number = 0;
    QList<int> entities;
    entities.clear();

    if (_qsos.length()>0)
    {
        for (int i = 0; i<_qsos.length(); i++)
        {
            bool confirmed = false;
            bool printThisOne = true;
            QString satName = _qsos.at(i)->getSatName();

            QString qslStatus = QString();
            if (_qsos.at(i)->getLoTWQSL_RCVD() == "Y")
            {
               qslStatus = "LoTW";
               confirmed = true;
            }
            else if (_qsos.at(i)->getQSL_RCVD() == "Y")
            {
               qslStatus = "QSL";
               confirmed = true;
            }
            else
            {
                qslStatus = QString(tr("No"));
                confirmed = false;
            }

            if (confirmedOnlyCheckBox->isChecked() && !confirmed)
            {
                printThisOne = false;
            }
            if ((onlyLEOSatCheckBox->isChecked()) && (satName == "QO-100") )
            {
                printThisOne = false;
            }
            if (entities.contains(_qsos.at(i)->getDXCC()))
            {
                printThisOne = false;
            }

            if (printThisOne)
            {
                //qDebug() << "StatsDxccOnSatsWidget::prepareChart: QSOs: printThisOne: " << (_qsos.at(i)->getCall())  << endl;
                entities.append(_qsos.at(i)->getDXCC());
                number++;
                tableWidget->insertRow(tableWidget->rowCount());
                tableWidget->setItem(tableWidget->rowCount()-1, 0, new QTableWidgetItem((_qsos.at(i)->getCall())) );
                tableWidget->setItem(tableWidget->rowCount()-1, 1, new QTableWidgetItem( util->getDateSQLiteStringFromDate(_qsos.at(i)->getDate()) ) );
                tableWidget->setItem(tableWidget->rowCount()-1, 2, new QTableWidgetItem((_qsos.at(i)->getBand())) );
                tableWidget->setItem(tableWidget->rowCount()-1, 3, new QTableWidgetItem((_qsos.at(i)->getMode())) );
                tableWidget->setItem(tableWidget->rowCount()-1, 4, new QTableWidgetItem((dataProxy->getEntityNameFromId(_qsos.at(i)->getDXCC()))) );
                tableWidget->setItem(tableWidget->rowCount()-1, 5, new QTableWidgetItem(satName));
                tableWidget->setItem(tableWidget->rowCount()-1, 6, new QTableWidgetItem(qslStatus) );
            }

        }
        numberLabel->setText(QString::number(number));
    }
}

void StatsDXCCOnSatsWidget::slotConfirmedClicked()
{
    prepareChart(log);
}
