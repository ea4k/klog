#ifndef BANDRECOMMENDER_H
#define BANDRECOMMENDER_H
/***************************************************************************
    bandrecommender.h  -  description
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
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class BandRecommender : public QWidget
{
    Q_OBJECT
public:
    explicit BandRecommender(QWidget *parent = nullptr);
    void createUI();
    void setButton1(const QString _band);
    void setButton2(const QString _band);
    void setButton3(const QString _band);
    void setButton4(const QString _band);
    void setButton5(const QString _band);

signals:

private:
    QPushButton *button1, *button2, *button3, *button4, *button5;
};

#endif // BANDRECOMMENDER_H
