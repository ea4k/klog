#ifndef SETUPPAGEINTERFACESWINDOWS_H
#define SETUPPAGEINTERFACESWINDOWS_H
/***************************************************************************
                          setuppageinterfaceswindows.h  -  description
                             -------------------
    begin                : feb 2020
    copyright            : (C) 2020 by Jaime Robles
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

#include <QObject>
#include <QtWidgets>


class SetupPageInterfacesWindows : public QWidget
{
    Q_OBJECT
public:
    SetupPageInterfacesWindows(QWidget *parent = nullptr);
    ~SetupPageInterfacesWindows();

    void setSendToPSTRotator(const QString &_t);
    void setPSTRotatorUDPServerPort(const QString &_t);
    void setPSTRotatorUDPServer(const QString &_t);

    QString getSendToPSTRotator();
    QString getPSTRotatorUDPServerPort();
    QString getPSTRotatorUDPServer();


private:
    void createUI();
    void createActions();

    QCheckBox *sendToPSTRotatorCheckBox;
    QSpinBox *PSTRotatorUDPServerPortSpinBox;
    QLineEdit *PSTRotatorServerLineEdit;
    int PSTRotatorDefaultport;
    QString PSTRotatorServer;

    
signals:

public slots:
    //void slotLogFromWSJTCheckBoxClicked();
    //void slotUDPServerCheckBoxClicked();
    void slotSendToPSTRotatorCheckBoxClicked();
    
};

#endif // SetupPageInterfacesWindows_H
