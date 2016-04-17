
/***************************************************************************
                          mainwindowsattab.cpp  -  description
                             -------------------
    begin                : jan 2015
    copyright            : (C) 2015 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                                *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify           *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,                *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/
#include "mainwindowsattab.h"

MainWindowSatTab::MainWindowSatTab(QWidget *parent) :
    QWidget(parent)
{
    satNameLineEdit = new QLineEdit;
    satModeLineEdit = new QLineEdit;
    keepThisDataForNextQSORadiobutton = new QRadioButton;

    createUI();
}

MainWindowSatTab::~MainWindowSatTab(){}

void MainWindowSatTab::createUI()
{
    connect(satNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(satNameTextChanged() ) );
    connect(satModeLineEdit, SIGNAL(textChanged(QString)), this, SLOT(satModeTextChanged() ) );


    QLabel *keepLabel = new QLabel();
    keepLabel->setText(tr("Keep this data"));
    keepLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    keepLabel->setToolTip(tr("Data entered in this tab will be copied into the next QSO"));

    keepThisDataForNextQSORadiobutton->setToolTip(tr("Data entered in this tab will be copied into the next QSO"));
    satNameLineEdit->setToolTip(tr("Name of the Satellite (format like AO-51)"));
    satModeLineEdit->setToolTip(tr("Satellite mode used"));

    QLabel *satNameLabel = new QLabel();
    satNameLabel->setText(tr("Satellite"));
    satNameLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QLabel *satModeLabel = new QLabel();
    satModeLabel->setText(tr("Mode"));
    satModeLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);

    QGridLayout *tabLayout = new QGridLayout;
    tabLayout->addWidget(satNameLabel, 0, 0);
    tabLayout->addWidget(satNameLineEdit, 0, 1);
    tabLayout->addWidget(satModeLabel, 1, 0);
    tabLayout->addWidget(satModeLineEdit, 1, 1);
    tabLayout->addWidget(keepLabel, 3, 2);
    tabLayout->addWidget(keepThisDataForNextQSORadiobutton, 3, 3);

    setLayout(tabLayout);

}

void MainWindowSatTab::satNameTextChanged()
{
    //qDebug() << "MainWindowSatTab::satNameTextChanged: " << satNameLineEdit->text() << endl;
    satNameLineEdit->setText((satNameLineEdit->text()).toUpper());

    if ((satNameLineEdit->text()).length()>0)
    {
        emit setPropModeSat("SAT");
    }
    else if ((satModeLineEdit->text()).length()<1)
    {
        emit setPropModeSat("Not");
    }

}

void MainWindowSatTab::satModeTextChanged()
{
    //qDebug() << "MainWindowSatTab::satModeTextChanged: " << satModeLineEdit->text() << endl;
    satModeLineEdit->setText((satModeLineEdit->text()).toUpper());

    if ((satModeLineEdit->text()).length()>0)
    {
        emit setPropModeSat("SAT");
    }
    else if ((satNameLineEdit->text()).length()<1)
    {
        emit setPropModeSat("Not");
    }

}

QString MainWindowSatTab::getSatName()
{
    // Sat name must follow the format CC-NN to make it compatible with LOTW
    // C = Character
    // N = Number

    QString satName;
    satName = satNameLineEdit->text();
    //TODO: Check that the format is OK
    return satName;
}

void MainWindowSatTab::setSatName(const QString _t)
{
     //TODO: Check that the format is OK
    satNameLineEdit->setText(_t);
}

QString MainWindowSatTab::getSatMode()
{
    return satModeLineEdit->text();
}

void MainWindowSatTab::setSatMode(const QString _t)
{
    satModeLineEdit->setText(_t);
}

bool MainWindowSatTab::getRepeatThis()
{
    return keepThisDataForNextQSORadiobutton->isChecked();
}

void MainWindowSatTab::setRepeatThis(const bool _t)
{
    keepThisDataForNextQSORadiobutton->setChecked(_t);
}
void MainWindowSatTab::clear()
{
    satModeLineEdit->clear();
    satNameLineEdit->clear();
}
