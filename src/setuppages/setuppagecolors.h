#ifndef SETUPPAGECOLORS_H
#define SETUPPAGECOLORS_H
/***************************************************************************
                          setuppagecolors.h  -  description
                             -------------------
    begin                : nov 2011
    copyright            : (C) 2011 by Jaime Robles
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

//#include <QtWidgets>
//#include <QPalette>
#include <QtWidgets>
#include "utilities.h"

class SetupPageColors : public QWidget {
    Q_OBJECT

public:
    SetupPageColors(QWidget *parent=0);
    ~SetupPageColors();

    QString getNewOneColor();
    QString getNeededColor();
    QString getWorkedColor();
    QString getConfirmedColor();
    QString getDefaultColor();
    QString getDarkMode();

    void setNewOneColor(const QString c);
    void setNeededColor(const QString c);
    void setWorkedColor(const QString c);
    void setConfirmedColor(const QString c);
    void setDefaultColor(const QString c);
    void setDarkMode(const QString &_c);



private slots:
    void slotNewOneColorButtonClicked();
    void slotConfirmedColorButtonClicked();
    void slotWorkedColorButtonClicked();
    void slotNeededColorButtonClicked();
    void slotDefaultColorButtonClicked();

    void slotWSJTXButtonClicked();
    void slotKLogButtonClicked();
    void slotSetDarkMode();


private:
    bool darkMode;
    Utilities *util;
    void setDefaultColors();
    void setWSJTXColors();
    QColor giveColor (QColor c);

    QPalette palette;

    QPushButton *newOneColorButton;     // In ANY band
    QPushButton *neededColorButton;     // In this band
    QPushButton *workedColorButton;     // In this band
    QPushButton *confirmedColorButton;  // In this band
    QPushButton *defaultColorButton;  // In this band
    QPushButton *wsjtxColorButton;  // In this band
    QPushButton *klogColorButton;  // In this band
    QPushButton *darkModeButton;

    QColor color;
/*
        0 - New one.
        1 - Worked but not confirmed: New one in this band.
        2 - Worked but not confirmed: Worked in this band.
        3 - Confirmed: New one in this band.
        4 - Confirmed: Worked in this band.
        5 - Confirmed: Confirmed in this band.
*/

};

#endif // SETUPPAGECOLORS_H
