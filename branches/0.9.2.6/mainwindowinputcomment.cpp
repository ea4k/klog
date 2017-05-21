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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.          *
 *                                                                           *
 *****************************************************************************/

#include "mainwindowinputcomment.h"


MainWindowInputComment::MainWindowInputComment(QWidget *parent) :
    QWidget(parent)
{
    commentLineEdit = new QLineEdit();
    comment.clear();
    createUI();
}


MainWindowInputComment::~MainWindowInputComment(){}

void MainWindowInputComment::createUI()
{

    commentLineEdit->setToolTip(tr("Add a comment for this QSO"));

    QVBoxLayout *tabLayout = new QVBoxLayout;
    tabLayout->addWidget(commentLineEdit);

    setLayout(tabLayout);
}




void MainWindowInputComment::setData(const QString _comment)
{
    comment = _comment;
    commentLineEdit->setText(comment);
}

QString MainWindowInputComment::getComment()
{
    return commentLineEdit->text();
}

void MainWindowInputComment::clear()
{
    comment.clear();
    commentLineEdit->clear();
}


