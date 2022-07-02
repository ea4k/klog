#ifndef SETUPPAGEDXCLUSTER_H
#define SETUPPAGEDXCLUSTER_H
/***************************************************************************
                          setuppagedxcluster.h  -  description
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

#include <QtWidgets>
#include "../utilities.h"

class SetupPageDxCluster : public QWidget {
    Q_OBJECT

public:
    SetupPageDxCluster(QWidget *parent=nullptr);
    ~SetupPageDxCluster();

    QStringList getDxclusterServersComboBox();
    void setDxclusterServersComboBox(const QStringList t);
    QString getSelectedDxClusterServer();
    void setSelectedDxClusterServer(const QString t);

    QString getShowHFQCheckbox();
    QString getShowVHFQCheckbox();
    QString getShowWARCQCheckbox();
    QString getShowWorkedQCheckbox();
    QString getShowConfirmedQCheckbox();
    QString getShowANNQCheckbox();
    QString getShowWWVQCheckbox();
    QString getShowWCYQCheckbox();
    QString getSaveActivityQCheckbox();
    QString getSendSpotsToMap();

    void setSendSpotstoMap(const QString t);
    void setShowHFQCheckbox(const QString t);
    void setShowVHFQCheckbox(const QString t);
    void setShowWARCQCheckbox(const QString t);
    void setShowWorkedQCheckbox(const QString t);
    void setShowConfirmedQCheckbox(const QString t);
    void setShowANNQCheckbox(const QString t);
    void setShowWWVQCheckbox(const QString t);
    void setShowWCYQCheckbox(const QString t);
    void setSaveActivityQCheckbox(const QString t);

private slots:
    void slotAddButtonClicked();
    void slotDeleteButtonClicked();

private:

    void createActions();
    bool checkIfValidDXCluster (const QString &tdxcluster);
    bool checkIfNewDXCluster (const QString &tdxcluster);


    QComboBox *dxclusterServersComboBox;
    QPushButton *addClusterButton;
    QPushButton *deleteClusterButton;

    QCheckBox *showHFQCheckbox;
    QCheckBox *showVHFQCheckbox;
    QCheckBox *showWARCQCheckbox;
    QCheckBox *showWorkedQCheckbox;
    QCheckBox *showConfirmedQCheckbox;
    QCheckBox *showANNQCheckbox;
    QCheckBox *showWWVQCheckbox;
    QCheckBox *showWCYQCheckbox;
    QCheckBox *sendSpotsToMapCheckbox;

    QCheckBox *saveAllDXClusterDataQCheckbox;

    Utilities *util;
    //QStringList dxClusterServers;

};

#endif // SETUPPAGEDXCLUSTER_H

