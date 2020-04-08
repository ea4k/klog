#ifndef MAINWINDOWINPUTCOMMENT_H
#define MAINWINDOWINPUTCOMMENT_H
/***************************************************************************
                          mainwindowminputcomment.h  -  description
                             -------------------
    begin                : ago 2016
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
//
// This class implement the tab of the mainwindow that supports the Comment
//
#include <QWidget>
#include <QtWidgets>

class MainWindowInputComment : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindowInputComment(QWidget *parent = nullptr);
    ~MainWindowInputComment();
    void createUI();
    void setData(const QString &_comment);
    QString getComment();

    void clear();

private:
    QString comment;
    QLineEdit *commentLineEdit;

};

#endif // MAINWINDOWINPUTCOMMENT_H
