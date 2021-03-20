#ifndef SETUPENTITYDIALOG_H
#define SETUPENTITYDIALOG_H
/***************************************************************************
                          SetupEntityDialog.h  -  description
                             -------------------
    begin                : sept 2012
    copyright            : (C) 2012 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                             *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify        *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,             *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/

//#include <QtWidgets>
#include <QtWidgets>
#include <QDialog>
#include <QPalette>


class SetupEntityDialog : public QDialog
{
    Q_OBJECT

public:
    SetupEntityDialog();
    ~SetupEntityDialog();

public slots:

private slots:

    void slotOkButtonClicked();
    void slotCancelButtonClicked();

    void slotCheckEntity();
    void slotCheckMainprefix();
    void slotCheckCQz();
    void slotCheckITUz();
    void slotCheckContinent();
    void slotCheckLatitude();
    void slotCheckLongitude();
    void slotCheckUTC();
    void slotCheckARRLid();
    void slotCheckDeleted();
    void slotCheckDeletedDate();
    void slotCheckPrefixes();


signals:
    void entityAdded(const QStringList _qs); // entity

private:
    QString checkEntity();
    QString checkMainprefix();
    QString checkCQz();
    QString checkITUz();
    QString checkContinent();
    QString checkLatitude();
    QString checkLongitude();
    QString checkUTC();
    QString checkARRLid();
    QString checkDeleted();
    QString checkDeletedDate();
    QString checkPrefixes();

    bool entityBool, mainPrefixBool, cqBool, ituBool, contBool;
    bool latBool, lonBool, utcBool, arrlidBool, delBool, delDateBool, prefBool;

    QLineEdit *entityLineEdit, *cqLineEdit, *ituLineEdit;
    QLineEdit *contLineEdit, *latLineEdit, *lonLineEdit;
    QLineEdit *utcLineEdit, *mprefLineEdit, *arrlidLineEdit;
    QLineEdit *deletedLineEdit, *prefLineEdit;
    QDateEdit *delQDateEdit;
    QRadioButton *delRbutton;

    QStringList entityData;
    //QPalette *paletteOrig, *paletteWrong;
    //QColor color;
    QPalette pal, palw;

};


#endif // SetupEntityDialog_H
