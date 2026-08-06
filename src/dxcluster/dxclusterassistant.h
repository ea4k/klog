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

// The DX Assistant widget: an embeddable panel that shows the live,
// prioritised list of DXCluster spots scored by DXAssistantEngine against
// the user's own log. Spots the user cannot "improve" (already confirmed
// in band+mode) never reach this widget — the engine discards them
// beforehand.
// MainWindow hosts it as a tab of the upper-right tab widget; the class
// sets no window flags of its own, so it can be embedded in any layout.

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
        ColSource,         // DXCluster, WSJT-X, ...
        ColAge,
        ColMWRank,
        ColumnCount
    };

    // SortRole carries the raw sortable value; every other role feeds one of
    // the view filters the proxy applies (see DXAssistantProxyModel).
    static constexpr int SortRole = Qt::UserRole;
    static constexpr int CallRole = Qt::UserRole + 1;
    static constexpr int BandIdRole = Qt::UserRole + 2;
    static constexpr int SpotterContinentRole = Qt::UserRole + 3;
    static constexpr int SpotterDXCCRole = Qt::UserRole + 4;
    static constexpr int StatusRole = Qt::UserRole + 5;   // effectiveStatus()
    static constexpr int DXCCRole = Qt::UserRole + 6;
    static constexpr int SourceRole = Qt::UserRole + 7;   // SpotSource of the spot
    static constexpr int SpotterRole = Qt::UserRole + 8;  // Callsign of the spotter

    explicit DXAssistantSpotModel(World *_world, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    void setColors(const QColor &_newOne, const QColor &_needed, const QColor &_worked);

    QSOStatus statusOf(const DXSpot &_spot) const;   // The status the view shows
    static QString statusName(QSOStatus _status);    // Translated label for the menu
    static QString sourceName(SpotSource _source);   // Shown in the Source column

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

// Proxy: applies every view filter (hidden calls, bands, spotter origin,
// status and DXCC) and breaks score ties with the ClubLog Most Wanted rank
// (lower rank first; unranked entities last).
class DXAssistantProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    friend class tst_DXClusterAssistant;

public:
    // Who the spotter must be, or where, for the spot to be shown.
    enum SpotterFilter
    {
        SpotterAll = 0,      // Any spotter (default)
        SpotterMyContinent,  // Only spotters on the user's continent
        SpotterMyDXCC,       // Only spotters in the user's own entity
        SpotterMyCall        // Only what the user spotted: heard here
    };

    explicit DXAssistantProxyModel(QObject *parent = nullptr);
    void setHiddenCalls(const QSet<QString> *_calls);
    void setDisabledBands(const QSet<int> *_bands);
    void setDisabledStatuses(const QSet<int> *_statuses);
    void setDisabledDXCCs(const QSet<int> *_dxccs);
    void setDisabledSources(const QSet<int> *_sources);
    void setSpotterFilter(SpotterFilter _filter, const QString &_continent, int _dxcc,
                          const QString &_callsign = QString());
    // "Follow my band": show only the band the operator is working on. It
    // overrides the per-band checkboxes while active.
    void setFollowBand(bool _follow, int _bandId);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const override;

private:
    int tieBreakValue(const QModelIndex &index) const;
    const QSet<QString> *hiddenCalls;
    const QSet<int> *disabledBands;
    const QSet<int> *disabledStatuses;
    const QSet<int> *disabledDXCCs;
    const QSet<int> *disabledSources;
    SpotterFilter spotterFilter;
    QString userContinent;
    int userDXCC;
    QString userCallsign;
    bool followBand;
    int currentBandId;
};

class DXClusterAssistant : public QWidget
{
    Q_OBJECT
    friend class tst_DXClusterAssistant;

public:
    static constexpr int SPOT_TTL_MINUTES = 30;   // Future: loaded from QSettings

    // How close to the operator the station that reported a spot is. When two
    // spots of the same station on the same band meet, the one from the
    // closest spotter is the one that stays.
    enum SpotterProximity
    {
        SpotterElsewhere = 0,   // Another continent, or nothing known about it
        SpotterInMyContinent,
        SpotterInMyDXCC,
        SpotterIsMe             // We heard the DX ourselves
    };

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

    // The band the operator is working on, for the "Follow my band" filter
    void setCurrentBand(int _bandId);
    // Whether a radio is connected: "QSY to this freq" is offered only then
    void setRigConnected(bool _connected);

    void addOrUpdateSpot(const DXSpot &_spot);
    // Called for EVERY spot arriving from the DXCluster, including the ones
    // the engine discards (already confirmed, unresolvable, over the cap):
    // feeds the raw activity tally behind "Most active band".
    void registerBandActivity(DXSpot _spot);
    void recalculateAll();      // Called after QSO logged or Most Wanted updated
    void setTTL(int _minutes);
    void setMaxSpots(int _max);
    void clearHiddenSpots();
    // "Clear all": drop every spot and forget the calls hidden this session,
    // so the list starts from scratch. The view filters are user settings and
    // are deliberately left untouched.
    void clearAll();
    // "Reset all": the mirror image of clearAll(). Every entry of the Filters
    // menu goes back to the value the DX Assistant starts with, so a list
    // narrowed down step by step opens up again in one go. The spots and the
    // calls hidden this session are data, not filters, and are left alone.
    void resetAllFilters();
    void updateBandSummary();   // Refresh "Most active band" / "Band to be"
    // Push the filter state to the proxy and refresh the summary. Public so
    // MainWindow can ask for it after changing something the filters read
    // from the engine, like the station callsign behind "My call".
    void applyViewFilters();

    QList<DXSpot> shownSpots() const;   // The spots the view is showing, in view order

signals:
    void spotSendToUI(const DXSpot &_spot);    // Fill the main QSO entry form
    void spotLogDirectly(const DXSpot &_spot); // Add QSO straight to the log
    void spotQSY(const DXSpot &_spot);         // Tune the radio to the spot frequency
    void spotsSendToMap(const QList<DXSpot> &_spots);   // Plot the shown spots

private slots:
    void slotTimerTick();
    void slotDoubleClicked(const QModelIndex &_index);
    void slotTableContextMenu(const QPoint &_pos);    // Right-click on a row
    void slotHeaderContextMenu(const QPoint &_pos);   // Right-click on the header

private:
    bool createUI();
    // Both right-click slots funnel here: there is a single menu, and the
    // entries that do not apply to the click are disabled rather than
    // dropped, so the menu looks the same wherever it was opened from.
    // _column is the column under the cursor (-1 if none) and _spot is the
    // spot under the cursor, valid only when _hasSpot is true.
    void showContextMenu(const QPoint &_globalPos, int _column,
                         const DXSpot &_spot, bool _hasSpot);
    void hideSpotCall(const QString &_call);
    void hideSpotCalls(const QStringList &_calls);   // Hides a whole selection at once
    QList<DXSpot> selectedSpots() const;   // The spots the user has picked in the table
    bool spotForProxyIndex(const QModelIndex &_index, DXSpot &_spot) const;
    void enforceMaxSpots();           // Evict the lowest-value spots over the cap
    void pruneBandActivity();         // Drop raw-activity entries past the age limit
    // Columns the table starts with hidden, and the way back to that state
    static bool isDefaultHiddenColumn(int _column);
    void applyDefaultColumns();
    // Whether every filter is already at its default: greys out "Reset all"
    bool filtersAreDefault() const;
    bool spotIsShown(DXSpot _spot) const;   // Same rules the proxy filter applies
    bool spotIsTooOld(DXSpot _spot) const;  // Already past the configured max age
    int spotterProximity(DXSpot _spot) const;   // One of SpotterProximity
    QList<int> dxccsInView() const;   // Entities currently held, sorted by name

    Awards *awards;
    World *world;
    DataProxy_SQLite *dataProxy;
    DXAssistantEngine *engine;

    DXAssistantSpotModel *model;
    DXAssistantProxyModel *proxy;
    QTableView *tableView;
    QLabel *mostActiveBandLabel;   // Band with most spots: raw cluster activity (#860)
    QLabel *bandToBeLabel;         // Band with highest cumulative score (#796)
    QTimer *ttlTimer;

    QSet<QString> hiddenCalls;   // Per-session only; never persisted to disk
    QSet<int> disabledBands;     // Bands the user filtered out of the view
    QSet<int> disabledStatuses;  // QSOStatus values filtered out of the view
    QSet<int> disabledDXCCs;     // Entities filtered out of the view
    QSet<int> disabledSources;   // SpotSource values filtered out of the view
    QHash<int, QList<QDateTime>> bandActivity;   // bandId -> raw spot arrival times
    DXAssistantProxyModel::SpotterFilter spotterFilter;
    bool followMyBand;      // Filter to the band the operator is working on
    int currentBandId;      // Pushed in by MainWindow through setCurrentBand()
    bool rigConnected;      // Gates the "QSY to this freq" entry
    int ttlMinutes;
    int maxSpots;
};

#endif // KLOG_CLUSTER_DXCLUSTERASSISTANT_H
