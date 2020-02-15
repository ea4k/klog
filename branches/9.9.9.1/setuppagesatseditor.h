#ifndef SETUPPAGESATEDITOR_H
#define SETUPPAGESATEDITOR_H

/***************************************************************************
                          setuppagesateditor.h  -  description
                             -------------------
    begin                : oct 2018
    copyright            : (C) 2018 by Jaime Robles
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

#include <QtWidgets>
#include <QtSql>
#include <QDialog>
#include "dataproxy.h"
//#include "utilities.h"

enum
{
    WORLD_DXCCid = 0,
    WORLD_Nameid = 1,
    WORLD_MainPrefix = 2,
    WORLD_CQZ = 3,
    WORLD_ITUZ = 4,
    WORLD_Cont = 5
};

class SetupPageSatEditor : public QWidget {
    Q_OBJECT

public:    
    SetupPageSatEditor(DataProxy *dp, QWidget *parent=0);
    ~SetupPageSatEditor();

private slots:
    void slotAddButtonClicked();
    void slotDelButtonClicked();
    void slotEditButtonClicked();
    void slotDoubleClickEntity( const QModelIndex & index);
    //void slotAnalyzeEntityAddedSignal(const QStringList _qs);

    void slotImportSatsButtonClicked();

private:

    //World *world;
    Utilities *util;
    DataProxy *dataProxy;

    void createSatsPanel();
    void createSatsModel();
    void createActions();
    bool isSatsListEmpty();

    QSqlRelationalTableModel *satsModel;
    QWidget *satsPanel;    
    QTableView *satsView;
    QTreeWidget *searchResultsTreeWidget;


    QPushButton *addSatPushButton, *delSatPushButton, *editSatPushButton, *exportSatsPushButton, *loadSatsPushButton;

    //SetupEntityDialog *setupEntityDialog;
};


#endif // SetupPageSatEditor_H
