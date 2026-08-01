#ifndef KLOG_CLUSTER_DXCLUSTERASSISTANT_H
#define KLOG_CLUSTER_DXCLUSTERASSISTANT_H
/***************************************************************************
    dxclusterassistant.h  -  description
       -------------------
begin                : mar 2024
copyright            : (C) 2024 by Jaime Robles
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

// The DX Assistant widget: a floating independent window that shows the
// live, prioritised list of DXCluster spots scored by DXAssistantEngine
// against the user's own log. Spots the user cannot "improve" (already
// confirmed in band+mode) never reach this widget — the engine discards
// them beforehand.

#include <QtWidgets>
#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QSet>
#include "dxspot.h"
#include "../klogdefinitions.h"

class Awards;
class World;
class DataProxy_SQLite;
class DXAssistantEngine;

// Table model backed by the scored spot list.
class DXAssistantSpotModel : public QAbstractTableModel
{
    Q_OBJECT
    friend class tst_DXClusterAssistant;

public:
    enum Column
    {
        ColPriority = 0,   // 1 = most valuable spot; recomputed on every change
        ColScore,
        ColDXCall,
        ColCountry,
        ColFrequency,
        ColMode,
        ColStatus,
        ColSpotter,
        ColAge,
        ColMWRank,
        ColumnCount
    };

    // SortRole carries the raw sortable value; CallRole the DX callsign
    // (used by the proxy filter to hide spots); BandIdRole and
    // SpotterContinentRole feed the band and continent view filters.
    static constexpr int SortRole = Qt::UserRole;
    static constexpr int CallRole = Qt::UserRole + 1;
    static constexpr int BandIdRole = Qt::UserRole + 2;
    static constexpr int SpotterContinentRole = Qt::UserRole + 3;

    explicit DXAssistantSpotModel(World *_world, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    void setColors(const QColor &_newOne, const QColor &_needed, const QColor &_worked);

    int indexOf(const QString &_call, int _bandId) const;
    DXSpot spotAt(int _row) const;
    void addSpot(const DXSpot &_spot);
    void replaceSpot(int _row, const DXSpot &_spot);
    void removeSpotAt(int _row);
    int spotCount() const;
    void clearSpots();

    void refreshAges();                 // Update the Age column (called every minute)
    void removeOlderThan(int _minutes); // Drop spots past the TTL
    int worstSpotRow() const;           // Lowest score (tiebreak included); -1 if empty

private:
    QSOStatus effectiveStatus(const DXSpot &_spot) const;
    QString statusText(const DXSpot &_spot) const;
    QVariant sortValue(const DXSpot &_spot, int _column) const;
    int priorityOfSpot(const DXSpot &_spot) const;   // 1-based; ties share the number
    void refreshPriorities();

    QList<DXSpot> spots;
    World *world;
    QColor newOneColor, neededColor, workedColor;
};

// Proxy: hides per-session hidden calls and breaks score ties with the
// ClubLog Most Wanted rank (lower rank first; unranked entities last).
class DXAssistantProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    friend class tst_DXClusterAssistant;

public:
    explicit DXAssistantProxyModel(QObject *parent = nullptr);
    void setHiddenCalls(const QSet<QString> *_calls);
    void setDisabledBands(const QSet<int> *_bands);
    void setOnlyContinent(bool _enabled, const QString &_continent);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const override;

private:
    int tieBreakValue(const QModelIndex &index) const;
    const QSet<QString> *hiddenCalls;
    const QSet<int> *disabledBands;
    bool onlyMyContinent;
    QString userContinent;
};

class DXClusterAssistant : public QWidget
{
    Q_OBJECT
    friend class tst_DXClusterAssistant;

public:
    static constexpr int SPOT_TTL_MINUTES = 30;   // Future: loaded from QSettings

    explicit DXClusterAssistant(Awards *_awards, World *_world,
                                DataProxy_SQLite *_dataProxy,
                                const QString &_parentFunction,
                                QWidget *parent = nullptr);
    ~DXClusterAssistant();
    bool init();

    // The engine is owned by MainWindow and injected after construction;
    // recalculateAll() and the duplicate-spot rule are no-ops without it.
    void setEngine(DXAssistantEngine *_engine);
    void setColors(const QColor &_newOne, const QColor &_needed, const QColor &_worked);

    static constexpr int MAX_SPOTS_DEFAULT = 25;   // Cap on managed spots

    void addOrUpdateSpot(const DXSpot &_spot);
    // Called for EVERY spot arriving from the DXCluster, including the ones
    // the engine discards (already confirmed, unresolvable, over the cap):
    // feeds the raw activity tally behind "Most active band".
    void registerBandActivity(DXSpot _spot);
    void recalculateAll();      // Called after QSO logged or Most Wanted updated
    void setTTL(int _minutes);
    void setMaxSpots(int _max);
    void clearHiddenSpots();
    void updateBandSummary();   // Refresh "Most active band" / "Band to be"

signals:
    void spotSendToUI(const DXSpot &_spot);    // Fill the main QSO entry form
    void spotLogDirectly(const DXSpot &_spot); // Add QSO straight to the log

private slots:
    void slotTimerTick();
    void slotDoubleClicked(const QModelIndex &_index);
    void slotContextMenu(const QPoint &_pos);
    void slotHeaderContextMenu(const QPoint &_pos);
    void slotClearHiddenClicked();

private:
    bool createUI();
    void hideSpotCall(const QString &_call);
    bool spotForProxyIndex(const QModelIndex &_index, DXSpot &_spot) const;
    void updateClearHiddenButton();
    void applyViewFilters();          // Push filter state to the proxy and refresh the summary
    void enforceMaxSpots();           // Evict the lowest-value spots over the cap
    void pruneBandActivity();         // Drop raw-activity entries past the age limit
    bool spotIsShown(DXSpot _spot) const;   // Same rules the proxy filter applies

    Awards *awards;
    World *world;
    DataProxy_SQLite *dataProxy;
    DXAssistantEngine *engine;

    DXAssistantSpotModel *model;
    DXAssistantProxyModel *proxy;
    QTableView *tableView;
    QPushButton *clearHiddenButton;
    QLabel *mostActiveBandLabel;   // Band with most spots: raw cluster activity (#860)
    QLabel *bandToBeLabel;         // Band with highest cumulative score (#796)
    QTimer *ttlTimer;

    QSet<QString> hiddenCalls;   // Per-session only; never persisted to disk
    QSet<int> disabledBands;     // Bands the user filtered out of the view
    QHash<int, QList<QDateTime>> bandActivity;   // bandId -> raw spot arrival times
    bool onlyMyContinentSpotters;
    int ttlMinutes;
    int maxSpots;
};

#endif // KLOG_CLUSTER_DXCLUSTERASSISTANT_H
