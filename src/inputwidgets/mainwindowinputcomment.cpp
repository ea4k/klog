/***************************************************************************
                          mainwindowinputcomment.cpp  -  description
                             -------------------
    begin                : Ago 2016
    copyright            : (C) 2016 by Jaime Robles
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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/

#include "mainwindowinputcomment.h"


MainWindowInputComment::MainWindowInputComment(QWidget *parent) :
    QWidget(parent)
{
       //qDebug() << "MainWindowInputComment::MainWindowInputComment"  ;
    commentLineEdit = new QLineEdit();
    keepThisDataForNextQSOQCheckbox = new QCheckBox;
    comment.clear();
    createUI();
       //qDebug() << "MainWindowInputComment::MainWindowInputComment - END"  ;
}


MainWindowInputComment::~MainWindowInputComment(){}

void MainWindowInputComment::createUI()
{

    QLabel *commentLabel = new QLabel(tr("Comment"));
    commentLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    commentLineEdit->setToolTip(tr("Add a comment for this QSO."));

    QLabel *keepLabel = new QLabel();
    keepLabel->setText(tr("Keep this data"));
    keepLabel->setAlignment(Qt::AlignVCenter| Qt::AlignRight);
    keepLabel->setToolTip(tr("Data entered in this tab will be copied into the next QSO."));

    keepThisDataForNextQSOQCheckbox->setToolTip(tr("Data entered in this tab will be copied into the next QSO."));

    QGridLayout *tabLayout = new QGridLayout;
    //QVBoxLayout *tabLayout = new QVBoxLayout;
    //tabLayout->addWidget(commentLineEdit);
    tabLayout->addWidget(commentLabel, 0, 0);
    tabLayout->addWidget(commentLineEdit, 0, 1);
    tabLayout->addWidget(keepLabel, 2, 1);
    tabLayout->addWidget(keepThisDataForNextQSOQCheckbox, 2, 2);
    //tabLayout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(tabLayout);
}

void MainWindowInputComment::setData(const QString &_comment)
{
    comment = _comment;
    commentLineEdit->setText(comment);
}

QString MainWindowInputComment::getComment()
{
    return commentLineEdit->text();
}

void MainWindowInputComment::clear(bool _full)
{
    if ((!keepThisDataForNextQSOQCheckbox->isChecked()) || _full)
    {
        comment.clear();
        commentLineEdit->clear();
    }
    if (_full)
    {
        keepThisDataForNextQSOQCheckbox->setChecked (false);
    }
}

void MainWindowInputComment::setKeep(const bool _b)
{
    keepThisDataForNextQSOQCheckbox->setChecked (_b);
}

bool MainWindowInputComment::getKeep()
{
    return keepThisDataForNextQSOQCheckbox->isChecked ();
}
