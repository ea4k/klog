/***************************************************************************
    bandrecommender.cpp  -  description
       -------------------
begin                : jan 2026
copyright            : (C) 2026 by Jaime Robles
email                : jaime@robles.es
***************************************************************************/

/****************************************************************************
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
*    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
*                                                                           *
*****************************************************************************/

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

void BandRecommender::setButton1(const QString _band)
{
    button1->setText(_band);
}

void BandRecommender::setButton2(const QString _band)
{
    button2->setText(_band);
}

void BandRecommender::setButton3(const QString _band)
{
    button3->setText(_band);
}

void BandRecommender::setButton4(const QString _band)
{
    button4->setText(_band);
}

void BandRecommender::setButton5(const QString _band)
{
    button5->setText(_band);
}
