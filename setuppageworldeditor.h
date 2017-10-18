#ifndef SETUPPAGEWORLDEDITOR_H
#define SETUPPAGEWORLDEDITOR_H

/***************************************************************************
                          setuppageworldeditor.h  -  description
                             -------------------
    begin                : jun 2012
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
 *    along with KLog.  If not, see <http://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/

//#include <QtWidgets>
#include <QtWidgets>
#include <QtSql>
#include <QDialog>
#include "world.h"
#include "dataproxy.h"
#include "setupentitydialog.h"
#include "utilities.h"

enum
{
    WORLD_DXCCid = 0,
    WORLD_Nameid = 1,
    WORLD_MainPrefix = 2,
    WORLD_CQZ = 3,
    WORLD_ITUZ = 4,
    WORLD_Cont = 5
};

class SetupPageWorldEditor : public QWidget {
    Q_OBJECT

public:    
    SetupPageWorldEditor(DataProxy *dp, QWidget *parent=0);
    ~SetupPageWorldEditor();

private slots:
    void slotAddButtonClicked();
    void slotDelButtonClicked();
    void slotEditButtonClicked();
    void slotDoubleClickEntity( const QModelIndex & index);
    void slotAnalyzeEntityAddedSignal(const QStringList _qs);

    void slotImportWorldButtonClicked();

private:

    World *world;
    Utilities *util;
    DataProxy *dataProxy;

    void createWorldPanel();
    void createWorldModel();
    void createActions();
    bool isWorldEmpty();

    QSqlRelationalTableModel *worldModel;
    QWidget *worldPanel;    
    QTableView *worldView;
    QTreeWidget *searchResultsTreeWidget;


    QPushButton *addEntityPushButton, *delEntityPushButton, *editEntityPushButton, *exportWorldPushButton, *loadWorldPushButton;

    SetupEntityDialog *setupEntityDialog;
};


#endif // SETUPPAGEWORLDEDITOR_H
