#ifndef KLOG_LOGWINDOW_H
#define KLOG_LOGWINDOW_H

/***************************************************************************
                          logwindow.h  -  description
                             -------------------
    begin                : sept 2011
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
#include <QWidget>
#include <QTableView>
#include <QAction>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlRelationalDelegate>
#include <QDesktopServices>
#include <QTimer>
#include <QPersistentModelIndex>
#include "dataproxy_sqlite.h"
#include "logmodel.h"
#include "awards.h"
#include "utilities.h"

// The log grid's item delegate. Edits the Mode ADIF (modeid) and Mode (submode) columns
// with comboboxes restricted to the user's active modes, keeping the two columns in
// sync: picking a submode updates the parent mode to match, and picking a parent mode
// resets the submode to the row that stands for "just this mode, no specific submode".
// Also paints a visible border around the current cell: the log grid selects whole rows
// (needed for the bulk QSO actions), so without this the current cell -- the one arrow
// keys move and F2/click-again would edit -- has no visible indicator of its own.
class LogModeDelegate : public QSqlRelationalDelegate
{
    Q_OBJECT

public:
    explicit LogModeDelegate(DataProxy_SQLite *_dataProxy, QObject *parent = nullptr);
    void setActiveSubModes(const QStringList &_subModes);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    int modeIdColumn() const;
    int subModeColumn() const;
    QStringList activeParentModes() const;
    QStringList comboItemsFor(const QModelIndex &index, const QStringList &_baseItems) const;

    DataProxy_SQLite *dataProxy;
    QStringList activeSubModes;
    mutable int m_modeIdColumn = -1;
    mutable int m_subModeColumn = -1;
};

class LogWindow : public  QWidget
{
    Q_OBJECT
    friend class tst_LogWindow;

public:
    LogWindow(Awards *awards, QWidget *parent = nullptr);
    ~LogWindow();
    void createlogPanel(const int _currentLog);
    void clear();
    void refresh();
    void scrollToTop();
    void setCurrentLog(const int _currentLog);

    void qslSentViaBureau(const int _qsoId);    //Maybe this could be defined as private and call it with an action, if needed.
    void qslRecViaBureau(const int _qsoId);     //Maybe this could be defined as private and call it with an action, if needed.
    void qslRecViaDirect(const int _qsoId);

    bool isQSLReceived(const int _qsoId);
    bool isQSLSent(const int _qsoId);
    //void setProxyModel (const bool _p);
    void sortColumn(const int _c);
    void setColumns(const QStringList &_columns);
    void refreshColumns();
    void setActiveModes(const QStringList &_subModes);

signals:
    void actionQSODoubleClicked(const int _qsoid);
    void actionDeleteQSO(const int _qsoid);
    void updateAwards();
    void updateSearchText();   
    //void qsoFound(const QStringList _qs); // Each: QString with format: Fieldname:value
    void queryError(QString functionFailed, QString errorCodeS, QString nativeError, QString failedQuery); // To alert about any failed query execution
    //void clearError();
    void deleteTheseQSOs(QList<int> _qsos);
    void exportToADIFTheseQSOs(QList<int> _qsos);
    void uploadToQRZcomTheseQSOs(QList<int> _qsos);

private slots:

    void slotDoubleClickLog(const QModelIndex & index);
    void slotRighButtonFromLog(const QPoint& pos);
    void slotQSLSentViaBureauFromLog();
    void slotQSLSentViaDirectFromLog();
    void slotQSLRecViaDirectFromLog();
    void slotQSLRecViaBureauFromLog();
    void slotQsoDeleteFromLog();
    void slotQSOToEditFromLog();
    void slotQueryErrorManagement(QString functionFailed, QString errorCodeS, QString nativeError, QString failedQuery);
    void slotCheckQRZCom();
    void slotCheckDXHeatCom();

    void slotQSOsDeselectAll();
    void slotQSOsSelectAll();
    void slotQSOsDeleteFromLog();
    void slotQSOsExportFromLog();
    void slotQSOsUploadToLoTWFromLog();
    void slotQSOsUploadToClubLogFromLog();
    void slotQSOsUploadToEQSLFromLog();
    void slotMultipleQSLSentViaBureauFromLog();
    void slotMultipleQSLSentViaDirectFromLog();
    void slotMultipleQSLRecViaBureauFromLog();
    void slotMultipleQSLRecViaDirectFromLog();
    void slotQSOsQRZUploadFromLog();
    void slotOnSectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);
    void slotOnSectionResized(int logicalIndex, int oldSize, int newSize);
    void slotLogViewClicked(const QModelIndex &index);
    void slotEditPendingCell();

private:
    void createUI();
    void createActionsCommon();
    void createActions();
    void retoreColumsOrder();
    void saveColumnOrder();
    void saveColumnWidths();
    void restoreColumnWidths();
    void deleteQSO(const int _qsoID);
    void rightButtonFromLogMenu(const int trow);
    void rightButtonMultipleFromLogMenu();
    void showMenuRightButtonFromLogCreateActions();
    void showColumn(const QString &_columnName);

    void setDefaultData();
    void setColumnsOfLog(const QStringList &_columns);
    QStringList getOrderedVisibleHeaders() const;


    DataProxy_SQLite *dataProxy;
    LogModel *logModel;
    // qItemSelectionModel *selectionModel;
    Awards *awards;
    //DXCCStatusWidget *dxccStatusWidget;
    //eLogClubLog *elogClublog;

    QTableView *logView;
    QLabel *logLabel;
    LogModeDelegate *modeDelegate;

    // Disambiguates "click a cell already on the selected row" (start editing) from the
    // first half of a double-click (open the QSO edit dialog instead): the edit is
    // deferred by doubleClickInterval() and cancelled if a double-click follows.
    QTimer *m_editClickTimer;
    QPersistentModelIndex m_pendingEditIndex;
    int m_lastClickedRow;

    QAction *delQSOFromLogAct;
    QAction *qsoToEditFromLogAct;
    QAction *qslSentViaBureauFromLogAct;
    QAction *qslSentViaDirectFromLogAct;
    QAction *qslRecViaBureauFromLogAct;
    QAction *qslRecViaDirectFromLogAct;
    QAction *checkQRZCOMFromLogAct;
    QAction *checkDXHeatFromLogAct;

    //Multiple selection actions
    QAction *multipleDeselectAll;
    QAction *multipleSelectAll;
    QAction *multipleDelQSOsFromLogAct;
    QAction *multipleExportToADIFFromLogAct;
    QAction *multipleQueueForLoTWFromLogAct;
    QAction *multipleQueueForClubLogFromLogAct;
    QAction *multipleQueueForQRZCOMFromLogAct;
    QAction *multipleQueueForEQSLFromLogAct;
    QAction *multipleQslSentViaBureauFromLogAct;
    QAction *multipleQslSentViaDirectFromLogAct;
    QAction *multipleQslRecViaBureauFromLogAct;
    QAction *multipleQslRecViaDirectFromLogAct;
    // qAction *moveToAnotherLog; // MOves the selected QSOs to another log.

    int currentLog;
    bool m_blockWidthSave;

    Utilities *util;
    QStringList columns;
   //LogViewSortFilterProxyModel *proxyModel;
   //bool sortingThroughProxyModel;
};
/*
class ItemDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    ItemDelegate(QWidget *parent=nullptr){}

    QString displayText(const QVariant &value, const QLocale &locale) const;

    // qString displayText (const QVariant &value);
};
*/
#endif // LOGWINDOW_H
