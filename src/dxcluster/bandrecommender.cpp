#include "bandrecommender.h"

BandRecommender::BandRecommender(QWidget *parent)
    : QWidget{parent}
{
    button1 = new QPushButton;
    button2 = new QPushButton;
    button3 = new QPushButton;
    button4 = new QPushButton;
    button5 = new QPushButton;
}

void BandRecommender::createUI()
{
    button1->setText(tr("1-Prio"));
    button2->setText(tr("2-Prio"));
    button3->setText(tr("3-Prio"));
    button4->setText(tr("4-Prio"));
    button5->setText(tr("5-Prio"));

    //QGroupBox *groupBox = new QGroupBox;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(button1);
    layout->addWidget(button2);
    layout->addWidget(button3);
    layout->addWidget(button4);
    layout->addWidget(button5);
    setLayout(layout);
}
