#ifndef KLOG_ABOUTDIALOG_H
#define KLOG_ABOUTDIALOG_H

/***************************************************************************
                          aboutdialog.h  -  description
                             -------------------
    begin                : feb 2017
    copyright            : (C) 2017 by Jaime Robles
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
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.         *
 *                                                                           *
 *****************************************************************************/

class QEvent;

#include <QDialog>
#include <QTabWidget>


class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AboutDialog(const QString &tversion, QWidget *parent = nullptr);
    ~AboutDialog();

    bool event(QEvent *event);

private:
    QTabWidget *tabw;
    QWidget *tab1, *tab2, *tab3, *tab4;

};



#endif // ABOUTDIALOG_H
