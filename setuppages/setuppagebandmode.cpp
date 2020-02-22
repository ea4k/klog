#include "setuppages/setuppagebandmode.h"

SetupPageBandMode::SetupPageBandMode(DataProxy_SQLite *dp, QWidget *parent) : QWidget(parent)
{
     //qDebug() << "SetupPageBandMode::SetupPageBandMode"   << endl;
    dataProxy = dp;

    bandsListWidget = new QListWidget;
    modesListWidget = new QListWidget;
    QHBoxLayout *layout = new QHBoxLayout;

    QVBoxLayout *bLayout = new QVBoxLayout;
    QVBoxLayout *mLayout = new QVBoxLayout;

    QLabel *bandsLabel = new QLabel(bandsListWidget);
    bandsLabel->setText(tr("Bands"));
    bandsLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    QLabel *modesLabel = new QLabel(modesListWidget);
    modesLabel->setText(tr("Modes"));
    modesLabel->setAlignment(Qt::AlignVCenter| Qt::AlignCenter);

    addBands(dataProxy->getBands());
    addModes(dataProxy->getModes());

    bLayout->addWidget(bandsLabel);
    bLayout->addWidget(bandsListWidget);

    mLayout->addWidget(modesLabel);
    mLayout->addWidget(modesListWidget);

    layout->addLayout(bLayout);
    layout->addLayout(mLayout);
    //layout->addWidget(bandsListWidget);
    //layout->addWidget(modesListWidget);

    setLayout(layout);
     //qDebug() << "SetupPageBandMode::SetupPageBandMode - END"   << endl;
}

SetupPageBandMode::~SetupPageBandMode()
{}


void SetupPageBandMode::addBands(QStringList _b)
{
    bandsListWidget->addItems(_b);

    QListWidgetItem* item = 0;
    for(int i = 0; i < bandsListWidget->count(); ++i){
        item = bandsListWidget->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}


void SetupPageBandMode::addModes(QStringList _b)
{
    modesListWidget->addItems(_b);

    QListWidgetItem* item = 0;
    for(int i = 0; i < modesListWidget->count(); ++i){
        item = modesListWidget->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }
}


QString SetupPageBandMode::getBands()
{
     //qDebug() << "SetupPageBandMode::getBands" << endl;

    QString b;
    QListWidgetItem *it;

    if ( (bandsListWidget->count()) < 1)
    {
        return "";
    }
    for (int i = 0; i < bandsListWidget->count(); i++)
    {
        it = bandsListWidget->item(i);
        if (it->checkState() == Qt::Checked)
        {
            b = b + it->text();
            b = b + ", ";
        }
    }
    if (b.size()<2)
    {
    }else
    {
        b.chop(2);
    }
    return b;
}

QString SetupPageBandMode::getModes()
{
     //qDebug() << "SetupPageBandMode::getModes" << endl;

    QString b;
    QListWidgetItem *it;

    if ( (modesListWidget->count()) < 1)
    {
        return "";
    }
    for (int i = 0; i < modesListWidget->count(); i++)
    {
        it = modesListWidget->item(i);

        if (it->checkState() == Qt::Checked)
        {
            b = b + it->text();
            b = b + ", ";
        }
    }

    if (b.size()<2)
    {
    }else
    {
        b.chop(2);
    }
     //qDebug() << "SetupPageBandMode::getModes: " << b << endl;
    return b;
}


void SetupPageBandMode::setActiveBands(QStringList q)
{
     //qDebug() << "SetupPageBandMode::setActiveBands" << endl;

    if (q.isEmpty())
    {return;}

    QString b;
    QListWidgetItem *it;

    if ( (bandsListWidget->count()) < 1)
    {
        return;
    }

    for (int i = 0; i < bandsListWidget->count(); i++)
    {
        it = bandsListWidget->item(i);
        it->setCheckState(Qt::Unchecked);

        for (int j=0;j<q.length();j++)
        {
            if (it->text() == q.at(j))
            {
                it->setCheckState(Qt::Checked);
            }
        }
    }
}

void SetupPageBandMode::setActiveModes(QStringList q)
{
     //qDebug() << "SetupPageBandMode::setActiveModes" << endl;

    if (q.isEmpty())
    {return;}

    QString b;
    QListWidgetItem *it;

    if ( (modesListWidget->count()) < 1)
    {
        return;
    }

    for (int i = 0; i < modesListWidget->count(); i++)
    {
        it = modesListWidget->item(i);
        it->setCheckState(Qt::Unchecked);

        for (int j=0;j<q.length();j++)
        {
            if (it->text() == q.at(j))
            {
                it->setCheckState(Qt::Checked);
            }
        }
    }
}
