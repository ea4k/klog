/***************************************************************************
    dxclusterassistant.cpp  -  description
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
#include "dxclusterassistant.h"
#include "dxassistantengine.h"
#include "../world.h"
#include "../dataproxy_sqlite.h"

#include <QActionGroup>
#include <QClipboard>
#include <QDesktopServices>
#include <QGuiApplication>
#include <QMenu>
#include <QUrl>

// ─────────────────────────────────────────────────────────────────────────────
// DXAssistantSpotModel
// ─────────────────────────────────────────────────────────────────────────────

// Value ordering shared by the Priority column, the sort tiebreak and the
// max-spots eviction: higher score first, then the ClubLog Most Wanted rank
// (lower rank first, unranked last).
static int mwTieValue(int _rank)
{
    return (_rank > 0) ? (std::numeric_limits<int>::max() - _rank) : 0;
}

static bool isBetterSpot(DXSpot _a, DXSpot _b)
{
    if (_a.getScore() != _b.getScore())
        return _a.getScore() > _b.getScore();
    return mwTieValue(_a.getMostWantedRank()) > mwTieValue(_b.getMostWantedRank());
}

DXAssistantSpotModel::DXAssistantSpotModel(World *_world, QObject *parent)
    : QAbstractTableModel(parent)
{
    world = _world;
}

int DXAssistantSpotModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return spots.count();
}

int DXAssistantSpotModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return ColumnCount;
}

QVariant DXAssistantSpotModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((orientation != Qt::Horizontal) || (role != Qt::DisplayRole))
        return QVariant();

    switch (section)
    {
    case ColPriority:  return tr("Priority");
    case ColScore:     return tr("Score");
    case ColDXCall:    return tr("DX Call");
    case ColCountry:   return tr("Country");
    case ColFrequency: return tr("Freq (kHz)");
    case ColMode:      return tr("Mode");
    case ColStatus:    return tr("Status");
    case ColSpotter:   return tr("Spotter");
    case ColSource:    return tr("Source");
    case ColAge:       return tr("Age");
    case ColMWRank:    return tr("MW Rank");
    default:           return QVariant();
    }
}

QSOStatus DXAssistantSpotModel::effectiveStatus(const DXSpot &_spot) const
{
    // Collapse the scoring category into the ATNO/needed/worked triple the
    // widget shows (confirmed spots never reach the widget).
    DXSpot spot(_spot);
    switch (spot.getStatusBand())
    {
    case ATNO:
        return ATNO;
    case needed:
        return needed;
    case worked:
    case confirmed:
        // Band-level entry exists: the band+mode status decides
        return (spot.getQSOStatus() == needed) ? needed : worked;
    default:
        return unknown;
    }
}

QSOStatus DXAssistantSpotModel::statusOf(const DXSpot &_spot) const
{
    return effectiveStatus(_spot);
}

QString DXAssistantSpotModel::statusName(QSOStatus _status)
{
    switch (_status)
    {
    case ATNO:   return tr("ATNO");
    case needed: return tr("Needed");
    case worked: return tr("Worked");
    default:     return QString();
    }
}

QString DXAssistantSpotModel::statusText(const DXSpot &_spot) const
{
    return statusName(effectiveStatus(_spot));
}

QString DXAssistantSpotModel::sourceName(SpotSource _source)
{
    switch (_source)
    {
    case SpotSourceDXCluster: return tr("DXCluster");
    case SpotSourceWSJTX:     return tr("WSJT-X");
    default:                  return QString();
    }
}

QVariant DXAssistantSpotModel::sortValue(const DXSpot &_spot, int _column) const
{
    DXSpot spot(_spot);
    switch (_column)
    {
    case ColPriority:  return priorityOfSpot(spot);
    case ColScore:     return spot.getScore();
    case ColDXCall:    return spot.getDxCall();
    case ColCountry:   return (world != nullptr) ? world->getEntityName(spot.getDXCC())
                                                 : QString::number(spot.getDXCC());
    case ColFrequency: return spot.getFrequency().toDouble(KHz);
    case ColMode:      return spot.getMode();
    case ColStatus:    return static_cast<int>(effectiveStatus(spot));
    case ColSpotter:   return spot.getSpotter();
    case ColSource:    return sourceName(spot.getSource());
    case ColAge:       return spot.getDateTime().secsTo(QDateTime::currentDateTimeUtc());
    case ColMWRank:    // Unranked entities (0) must sort after any ranked one
        return (spot.getMostWantedRank() > 0) ? spot.getMostWantedRank()
                                              : std::numeric_limits<int>::max();
    default:           return QVariant();
    }
}

QVariant DXAssistantSpotModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() < 0) || (index.row() >= spots.count()))
        return QVariant();

    DXSpot spot(spots.at(index.row()));

    if (role == SortRole)
        return sortValue(spot, index.column());

    if (role == CallRole)
        return spot.getDxCall();

    if (role == BandIdRole)
        return spot.getBandId();

    if (role == SpotterContinentRole)
        return spot.getSpotterContinent();

    if (role == SpotterDXCCRole)
        return spot.getSpotterDXCC();

    if (role == StatusRole)
        return static_cast<int>(effectiveStatus(spot));

    if (role == DXCCRole)
        return spot.getDXCC();

    if (role == SourceRole)
        return static_cast<int>(spot.getSource());

    if (role == SpotterRole)
        return spot.getSpotter();

    if (role == Qt::ToolTipRole)
    {   // Exact UTC arrival time of the spot
        if (index.column() == ColAge)
            return tr("Received at %1 UTC").arg(spot.getDateTime().toUTC().toString("HH:mm"));
        // What the spot came with: the comment of a DXCluster spot or, for a
        // station heard locally, that it was WSJT-X who decoded it
        if (((index.column() == ColDXCall) || (index.column() == ColSpotter)) &&
            !spot.getComment().isEmpty())
            return spot.getComment();
        return QVariant();
    }

    if (role == Qt::ForegroundRole)
    {
        switch (effectiveStatus(spot))
        {
        case ATNO:   return newOneColor.isValid() ? QVariant(newOneColor) : QVariant();
        case needed: return neededColor.isValid() ? QVariant(neededColor) : QVariant();
        case worked: return workedColor.isValid() ? QVariant(workedColor) : QVariant();
        default:     return QVariant();
        }
    }

    if (role != Qt::DisplayRole)
        return QVariant();

    switch (index.column())
    {
    case ColPriority:
        return priorityOfSpot(spot);
    case ColScore:
        return spot.getScore();
    case ColDXCall:
        return spot.getDxCall();
    case ColCountry:
        return (world != nullptr) ? world->getEntityName(spot.getDXCC()) : QString();
    case ColFrequency:
        return spot.getFrequency().toQString(KHz);
    case ColMode:
        return spot.getMode().isEmpty() ? QStringLiteral("---") : spot.getMode();
    case ColStatus:
        return statusText(spot);
    case ColSpotter:
        return spot.getSpotter();
    case ColSource:
    {
        const QString name = sourceName(spot.getSource());
        return name.isEmpty() ? QStringLiteral("---") : name;
    }
    case ColAge:
    {
        qint64 minutes = spot.getDateTime().secsTo(QDateTime::currentDateTimeUtc()) / 60;
        if (minutes < 0)
            minutes = 0;
        return tr("%1 min").arg(minutes);
    }
    case ColMWRank:
        return (spot.getMostWantedRank() > 0) ? QVariant(spot.getMostWantedRank())
                                              : QVariant(QStringLiteral("—"));
    default:
        return QVariant();
    }
}

void DXAssistantSpotModel::setColors(const QColor &_newOne, const QColor &_needed, const QColor &_worked)
{
    newOneColor = _newOne;
    neededColor = _needed;
    workedColor = _worked;
    if (!spots.isEmpty())
        emit dataChanged(index(0, 0), index(spots.count() - 1, ColumnCount - 1));
}

int DXAssistantSpotModel::indexOf(const QString &_call, int _bandId) const
{
    for (int i = 0; i < spots.count(); i++)
    {
        DXSpot spot(spots.at(i));
        if ((spot.getDxCall() == _call) && (spot.getBandId() == _bandId))
            return i;
    }
    return -1;
}

DXSpot DXAssistantSpotModel::spotAt(int _row) const
{
    if ((_row < 0) || (_row >= spots.count()))
        return DXSpot();
    return spots.at(_row);
}

int DXAssistantSpotModel::priorityOfSpot(const DXSpot &_spot) const
{
    // 1 + number of strictly better spots; equally valuable spots share the
    // same priority number
    int better = 0;
    for (const DXSpot &other : spots)
    {
        if (isBetterSpot(other, _spot))
            better++;
    }
    return better + 1;
}

void DXAssistantSpotModel::refreshPriorities()
{
    if (!spots.isEmpty())
        emit dataChanged(index(0, ColPriority), index(spots.count() - 1, ColPriority));
}

int DXAssistantSpotModel::worstSpotRow() const
{
    int worst = -1;
    for (int i = 0; i < spots.count(); i++)
    {
        if ((worst < 0) || isBetterSpot(spots.at(worst), spots.at(i)))
            worst = i;
    }
    return worst;
}

void DXAssistantSpotModel::addSpot(const DXSpot &_spot)
{
    beginInsertRows(QModelIndex(), spots.count(), spots.count());
    spots.append(_spot);
    endInsertRows();
    refreshPriorities();
}

void DXAssistantSpotModel::replaceSpot(int _row, const DXSpot &_spot)
{
    if ((_row < 0) || (_row >= spots.count()))
        return;
    spots[_row] = _spot;
    emit dataChanged(index(_row, 0), index(_row, ColumnCount - 1));
    refreshPriorities();
}

void DXAssistantSpotModel::removeSpotAt(int _row)
{
    if ((_row < 0) || (_row >= spots.count()))
        return;
    beginRemoveRows(QModelIndex(), _row, _row);
    spots.removeAt(_row);
    endRemoveRows();
    refreshPriorities();
}

int DXAssistantSpotModel::spotCount() const
{
    return spots.count();
}

void DXAssistantSpotModel::clearSpots()
{
    beginResetModel();
    spots.clear();
    endResetModel();
}

void DXAssistantSpotModel::refreshAges()
{
    if (spots.isEmpty())
        return;
    emit dataChanged(index(0, ColAge), index(spots.count() - 1, ColAge));
}

void DXAssistantSpotModel::removeOlderThan(int _minutes)
{
    QDateTime now = QDateTime::currentDateTimeUtc();
    for (int i = spots.count() - 1; i >= 0; i--)
    {
        DXSpot spot(spots.at(i));
        if (spot.getDateTime().secsTo(now) > qint64(_minutes) * 60)
            removeSpotAt(i);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// DXAssistantProxyModel
// ─────────────────────────────────────────────────────────────────────────────

DXAssistantProxyModel::DXAssistantProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    hiddenCalls      = nullptr;
    disabledBands    = nullptr;
    disabledStatuses = nullptr;
    disabledDXCCs    = nullptr;
    disabledSources  = nullptr;
    spotterFilter    = SpotterAll;
    userCallsign     = QString();
    userDXCC         = -1;
    followBand       = false;
    currentBandId    = -1;
    setSortRole(DXAssistantSpotModel::SortRole);
}

void DXAssistantProxyModel::setHiddenCalls(const QSet<QString> *_calls)
{
    hiddenCalls = _calls;
    invalidateFilter();
}

void DXAssistantProxyModel::setDisabledBands(const QSet<int> *_bands)
{
    disabledBands = _bands;
    invalidateFilter();
}

void DXAssistantProxyModel::setDisabledStatuses(const QSet<int> *_statuses)
{
    disabledStatuses = _statuses;
    invalidateFilter();
}

void DXAssistantProxyModel::setDisabledDXCCs(const QSet<int> *_dxccs)
{
    disabledDXCCs = _dxccs;
    invalidateFilter();
}

void DXAssistantProxyModel::setDisabledSources(const QSet<int> *_sources)
{
    disabledSources = _sources;
    invalidateFilter();
}

void DXAssistantProxyModel::setSpotterFilter(SpotterFilter _filter, const QString &_continent,
                                             int _dxcc, const QString &_callsign)
{
    spotterFilter = _filter;
    userContinent = _continent;
    userDXCC      = _dxcc;
    userCallsign  = _callsign.trimmed().toUpper();
    invalidateFilter();
}

void DXAssistantProxyModel::setFollowBand(bool _follow, int _bandId)
{
    followBand    = _follow;
    currentBandId = _bandId;
    invalidateFilter();
}

bool DXAssistantProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex idx = sourceModel()->index(sourceRow, 0, sourceParent);

    if (hiddenCalls != nullptr)
    {
        QString call = sourceModel()->data(idx, DXAssistantSpotModel::CallRole).toString();
        if (hiddenCalls->contains(call))
            return false;
    }
    // "Follow my band" replaces the per-band checkboxes while it is active:
    // the rig decides which band is shown.
    if (followBand && (currentBandId > 0))
    {
        int bandId = sourceModel()->data(idx, DXAssistantSpotModel::BandIdRole).toInt();
        if (bandId != currentBandId)
            return false;
    }
    else if (disabledBands != nullptr)
    {
        int bandId = sourceModel()->data(idx, DXAssistantSpotModel::BandIdRole).toInt();
        if (disabledBands->contains(bandId))
            return false;
    }
    if (disabledStatuses != nullptr)
    {
        int status = sourceModel()->data(idx, DXAssistantSpotModel::StatusRole).toInt();
        if (disabledStatuses->contains(status))
            return false;
    }
    if (disabledDXCCs != nullptr)
    {
        int dxcc = sourceModel()->data(idx, DXAssistantSpotModel::DXCCRole).toInt();
        if (disabledDXCCs->contains(dxcc))
            return false;
    }
    if (disabledSources != nullptr)
    {
        int source = sourceModel()->data(idx, DXAssistantSpotModel::SourceRole).toInt();
        if (disabledSources->contains(source))
            return false;
    }
    // An unknown reference (no continent / no entity for the station) leaves
    // the spotter filter inactive: better to show a spot than to hide it for
    // a reason the user cannot see.
    if ((spotterFilter == SpotterMyContinent) && !userContinent.isEmpty())
    {
        QString continent = sourceModel()->data(idx, DXAssistantSpotModel::SpotterContinentRole).toString();
        if (continent != userContinent)
            return false;
    }
    else if ((spotterFilter == SpotterMyDXCC) && (userDXCC > 0))
    {
        int dxcc = sourceModel()->data(idx, DXAssistantSpotModel::SpotterDXCCRole).toInt();
        if (dxcc != userDXCC)
            return false;
    }
    else if ((spotterFilter == SpotterMyCall) && !userCallsign.isEmpty())
    {   // Only what we spotted ourselves, and nothing we were told about
        QString spotter = sourceModel()->data(idx, DXAssistantSpotModel::SpotterRole).toString();
        if (spotter.toUpper() != userCallsign)
            return false;
    }
    return true;
}

int DXAssistantProxyModel::tieBreakValue(const QModelIndex &index) const
{
    // Bigger is better. Every entity takes part in tiebreaking: a lower Most
    // Wanted rank (more wanted worldwide) wins, and any ranked entity beats
    // an unranked one. The SortRole already maps "unranked" to INT_MAX, so
    // inverting it yields exactly that ordering.
    QModelIndex rankIdx = sourceModel()->index(index.row(), DXAssistantSpotModel::ColMWRank,
                                               index.parent());
    int rank = sourceModel()->data(rankIdx, DXAssistantSpotModel::SortRole).toInt();
    return std::numeric_limits<int>::max() - rank;
}

bool DXAssistantProxyModel::lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const
{
    if (sourceLeft.column() == DXAssistantSpotModel::ColScore)
    {
        int leftScore  = sourceModel()->data(sourceLeft,  DXAssistantSpotModel::SortRole).toInt();
        int rightScore = sourceModel()->data(sourceRight, DXAssistantSpotModel::SortRole).toInt();
        if (leftScore != rightScore)
            return leftScore < rightScore;
        return tieBreakValue(sourceLeft) < tieBreakValue(sourceRight);
    }
    return QSortFilterProxyModel::lessThan(sourceLeft, sourceRight);
}

// ─────────────────────────────────────────────────────────────────────────────
// DXClusterAssistant
// ─────────────────────────────────────────────────────────────────────────────

DXClusterAssistant::DXClusterAssistant(Awards *_awards, World *_world,
                                       DataProxy_SQLite *_dataProxy,
                                       const QString &_parentFunction,
                                       QWidget *parent) : QWidget(parent)
{
    Q_UNUSED(_parentFunction);
    awards     = _awards;
    world      = _world;
    dataProxy  = _dataProxy;
    engine     = nullptr;
    model      = nullptr;
    proxy      = nullptr;
    tableView  = nullptr;
    mostActiveBandLabel = nullptr;
    bandToBeLabel = nullptr;
    ttlTimer   = nullptr;
    ttlMinutes = SPOT_TTL_MINUTES;
    maxSpots   = MAX_SPOTS_DEFAULT;
    spotterFilter = DXAssistantProxyModel::SpotterAll;
    followMyBand  = false;
    currentBandId = -1;
    rigConnected  = false;

    // No window flags: the widget lives as a tab page of the main window.
    // The title is kept for accessibility and for any future undocking.
    setWindowTitle(tr("DX Assistant"));
}

DXClusterAssistant::~DXClusterAssistant()
{
}

bool DXClusterAssistant::init()
{
    if (tableView != nullptr)
        return true;   // Already initialised
    return createUI();
}

bool DXClusterAssistant::createUI()
{
    model = new DXAssistantSpotModel(world, this);
    proxy = new DXAssistantProxyModel(this);
    proxy->setSourceModel(model);
    proxy->setHiddenCalls(&hiddenCalls);
    proxy->setDisabledBands(&disabledBands);
    proxy->setDisabledStatuses(&disabledStatuses);
    proxy->setDisabledDXCCs(&disabledDXCCs);
    proxy->setDisabledSources(&disabledSources);

    tableView = new QTableView(this);
    tableView->setModel(proxy);
    tableView->setSortingEnabled(true);
    tableView->sortByColumn(DXAssistantSpotModel::ColScore, Qt::DescendingOrder);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Several spots can be picked at once, so a batch of them can be hidden
    // in one go rather than one right-click at a time.
    tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    tableView->verticalHeader()->setVisible(false);
    tableView->horizontalHeader()->setStretchLastSection(true);
    applyDefaultColumns();
    tableView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(tableView, &QTableView::doubleClicked,
            this, &DXClusterAssistant::slotDoubleClicked);
    connect(tableView, &QTableView::customContextMenuRequested,
            this, &DXClusterAssistant::slotTableContextMenu);
    connect(tableView->horizontalHeader(), &QHeaderView::customContextMenuRequested,
            this, &DXClusterAssistant::slotHeaderContextMenu);

    // Summary bar pinned above the spot table (#796 / #860)
    mostActiveBandLabel = new QLabel(this);
    mostActiveBandLabel->setToolTip(tr("The band where most DX spots are arriving right now."));
    bandToBeLabel = new QLabel(this);
    bandToBeLabel->setToolTip(tr("The band where you have the most to gain, based on your log."));

    QHBoxLayout *summaryLayout = new QHBoxLayout;
    summaryLayout->addWidget(mostActiveBandLabel);
    summaryLayout->addStretch();
    summaryLayout->addWidget(bandToBeLabel);

    // No button row: everything lives in the context menu, so the table gets
    // the whole panel below the summary bar.
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(summaryLayout);
    layout->addWidget(tableView);
    setLayout(layout);

    updateBandSummary();

    ttlTimer = new QTimer(this);
    ttlTimer->setInterval(60 * 1000);
    connect(ttlTimer, &QTimer::timeout, this, &DXClusterAssistant::slotTimerTick);
    ttlTimer->start();

    // No resize() here: as a tab page the size comes from the host layout.
    return true;
}

void DXClusterAssistant::setEngine(DXAssistantEngine *_engine)
{
    engine = _engine;
}

void DXClusterAssistant::setCurrentBand(int _bandId)
{
    if (currentBandId == _bandId)
        return;
    currentBandId = _bandId;
    if (followMyBand)      // Only then does the new band change what is shown
        applyViewFilters();
}

void DXClusterAssistant::setRigConnected(bool _connected)
{
    rigConnected = _connected;
}

void DXClusterAssistant::setColors(const QColor &_newOne, const QColor &_needed, const QColor &_worked)
{
    if (model != nullptr)
        model->setColors(_newOne, _needed, _worked);
}

void DXClusterAssistant::addOrUpdateSpot(const DXSpot &_spot)
{
    if (model == nullptr)
        return;

    DXSpot spot(_spot);
    if (spot.getScore() < 0)
        return;   // Discarded by the engine (confirmed / unresolvable)

    if (spotIsTooOld(spot))
        return;   // Already past the age the user allows: never worth showing

    if (hiddenCalls.contains(spot.getDxCall()))
        return;   // Hidden this session: silently dropped

    // Already worked on this band this session: the DXCluster goes on
    // reporting a station for as long as it is active, and putting it back in
    // the list would undo what logging the QSO just did.
    if (alreadyWorked(spot.getDxCall(), spot.getBandId(), modeFamily(spot.getMode())))
        return;

    int row = model->indexOf(spot.getDxCall(), spot.getBandId());
    if (row < 0)
    {
        model->addSpot(spot);
        enforceMaxSpots();
        updateBandSummary();
        return;
    }

    // Duplicate (same call+band): the report that stays is the one from the
    // closest spotter, so the arriving spot only takes the entry over when it
    // climbs the proximity ladder. Everything it carries goes with it: the
    // score it was given, the comment describing what that spotter heard and
    // the source it came from all belong to the same report.
    DXSpot existing = model->spotAt(row);
    if (spotterProximity(spot) > spotterProximity(existing))
    {
        model->replaceSpot(row, spot);
        updateBandSummary();
        return;
    }

    // Otherwise nothing new was learnt about the station beyond the fact that
    // it is still being heard, which is what the age says.
    existing.setDateTime(spot.getDateTime());
    model->replaceSpot(row, existing);
    updateBandSummary();
}

bool DXClusterAssistant::spotIsTooOld(DXSpot _spot) const
{
    // Mirror of DXAssistantSpotModel::removeOlderThan: a spot the very next
    // TTL sweep would drop has no business entering the list, not even to
    // refresh an entry already there. WSJT-X in particular replies to the
    // replay request with everything its band activity window is showing,
    // which after a few hours of monitoring is mostly history.
    const QDateTime when = _spot.getDateTime();
    if (!when.isValid())
        return false;   // Undated: treated as just arrived, as it always was
    return when.secsTo(QDateTime::currentDateTimeUtc()) > qint64(ttlMinutes) * 60;
}

int DXClusterAssistant::spotterProximity(DXSpot _spot) const
{
    // How close to the operator the station that reported a spot is. Being
    // the operator beats being in the same entity, which beats being in the
    // same continent, which beats anything else: the closer the spotter, the
    // more the report is worth, and the more likely the DX is workable from
    // here right now.
    if (engine == nullptr)
        return SpotterElsewhere;

    const QString userCallsign = engine->getUserCallsign();
    if (!userCallsign.isEmpty() && (_spot.getSpotter().toUpper() == userCallsign))
        return SpotterIsMe;

    const int userDXCC = engine->getUserDXCC();
    if ((userDXCC > 0) && (_spot.getSpotterDXCC() == userDXCC))
        return SpotterInMyDXCC;

    const QString userContinent = engine->getUserContinent();
    if (!userContinent.isEmpty() && (_spot.getSpotterContinent() == userContinent))
        return SpotterInMyContinent;

    return SpotterElsewhere;
}

void DXClusterAssistant::registerBandActivity(DXSpot _spot)
{
    int bandId = _spot.getBandId();
    if ((bandId <= 0) && (dataProxy != nullptr))
    {   // Discarded spots may arrive with the band unresolved
        bandId = dataProxy->getBandIdFromFreq(_spot.getFrequency());
    }
    if (bandId <= 0)
        return;

    QDateTime when = _spot.getDateTime().isValid() ? _spot.getDateTime().toUTC()
                                                   : QDateTime::currentDateTimeUtc();
    bandActivity[bandId].append(when);
    pruneBandActivity();
    updateBandSummary();
}

void DXClusterAssistant::pruneBandActivity()
{
    QDateTime limit = QDateTime::currentDateTimeUtc().addSecs(-qint64(ttlMinutes) * 60);
    for (auto it = bandActivity.begin(); it != bandActivity.end(); )
    {
        QList<QDateTime> &times = it.value();
        times.removeIf([&limit](const QDateTime &t) { return t < limit; });
        if (times.isEmpty())
            it = bandActivity.erase(it);
        else
            ++it;
    }
}

void DXClusterAssistant::recalculateAll()
{
    if ((model == nullptr) || (engine == nullptr))
        return;

    for (int i = model->spotCount() - 1; i >= 0; i--)
    {
        DXSpot spot = model->spotAt(i);
        if (engine->rescore(spot))
            model->replaceSpot(i, spot);
        else
            model->removeSpotAt(i);   // Just confirmed (or no longer resolvable)
    }
    updateBandSummary();
}

QString DXClusterAssistant::modeFamily(const QString &_mode) const
{
    if (_mode.isEmpty() || (dataProxy == nullptr))
        return QString();
    // Submodes answer with the mode they belong to, so a QSO logged as USB and
    // a spot reported as SSB are the same mode. A mode KLog does not know
    // answers with nothing, and then the mode simply does not take part.
    return dataProxy->getNameFromSubMode(_mode).toUpper();
}

bool DXClusterAssistant::alreadyWorked(const QString &_call, int _bandId,
                                       const QString &_mode) const
{
    const QString call = _call.trimmed().toUpper();
    for (const WorkedQSO &worked : workedQSOs)
    {
        if ((worked.call != call) || (worked.bandId != _bandId))
            continue;
        // Same rule the removal uses: the mode only tells them apart when the
        // QSO and the spot both carry one
        if (!worked.mode.isEmpty() && !_mode.isEmpty() && (worked.mode != _mode))
            continue;
        return true;
    }
    return false;
}

int DXClusterAssistant::removeSpotsOfLoggedQSO(const QString &_call, int _bandId,
                                               const QString &_mode)
{
    const QString call = _call.trimmed().toUpper();
    if ((model == nullptr) || call.isEmpty() || (_bandId < 0))
        return 0;

    const QString qsoMode = modeFamily(_mode);

    // Remembered for the rest of the session, whether or not the station is
    // in the list right now: it may well be spotted again in a minute, and it
    // may equally have been worked before any spot of it arrived.
    if (!alreadyWorked(call, _bandId, qsoMode))
        workedQSOs.append(WorkedQSO{call, _bandId, qsoMode});

    int removed = 0;
    for (int row = model->spotCount() - 1; row >= 0; row--)
    {
        DXSpot spot = model->spotAt(row);
        if ((spot.getDxCall().toUpper() != call) || (spot.getBandId() != _bandId))
            continue;
        // The mode only tells two spots apart when both sides know theirs: a
        // DXCluster spot usually carries none, and then the band is the whole
        // answer, which is what the operator means by "I have worked it".
        const QString spotMode = modeFamily(spot.getMode());
        if (!qsoMode.isEmpty() && !spotMode.isEmpty() && (qsoMode != spotMode))
            continue;
        model->removeSpotAt(row);
        removed++;
    }

    if (removed > 0)
    {
        if (proxy != nullptr)
            proxy->invalidate();
        updateBandSummary();
    }
    return removed;
}

void DXClusterAssistant::updateBandSummary()
{
    if ((mostActiveBandLabel == nullptr) || (bandToBeLabel == nullptr) || (model == nullptr))
        return;

    // Most active band counts ALL the spots arriving from the DXCluster
    // within the age window — needed or not, and regardless of any view
    // filter (#860): it is raw cluster activity.
    int mostActiveBand = -1;
    int bestCount = 0;
    for (auto it = bandActivity.constBegin(); it != bandActivity.constEnd(); ++it)
    {
        if (it.value().count() > bestCount)
        {
            bestCount = it.value().count();
            mostActiveBand = it.key();
        }
    }

    // Band to be weighs the score the user can still get on each band, so it
    // only counts the spots actually shown (#796): hiding a spot, filtering
    // a band or a spotter continent, and working or confirming a QSO (which
    // drops the spot in recalculateAll) all change what is achievable.
    QHash<int, int> scorePerBand;   // bandId -> cumulative score of shown spots
    for (int i = 0; i < model->spotCount(); i++)
    {
        DXSpot spot = model->spotAt(i);
        if (!spotIsShown(spot))
            continue;
        scorePerBand[spot.getBandId()] += spot.getScore();
    }

    int bandToBe = -1;         // argmax(scorePerBand): personal value
    int bestScore = 0;
    for (auto it = scorePerBand.constBegin(); it != scorePerBand.constEnd(); ++it)
    {
        if (it.value() > bestScore)
        {
            bestScore = it.value();
            bandToBe = it.key();
        }
    }
    if (bandToBe < 0)
    {   // Nothing to gain anywhere (no shown spots): point the user at the
        // band where the DX is actually happening.
        bandToBe = mostActiveBand;
    }

    QString mostActiveName = ((mostActiveBand > 0) && (dataProxy != nullptr))
                                 ? dataProxy->getNameFromBandId(mostActiveBand) : QString();
    QString bandToBeName   = ((bandToBe > 0) && (dataProxy != nullptr))
                                 ? dataProxy->getNameFromBandId(bandToBe) : QString();

    if (mostActiveName.isEmpty())
        mostActiveName = QStringLiteral("—");
    if (bandToBeName.isEmpty())
        bandToBeName = QStringLiteral("—");

    mostActiveBandLabel->setText(tr("Most active band: %1").arg(mostActiveName));
    bandToBeLabel->setText(tr("Band to be: %1").arg(bandToBeName));
}

void DXClusterAssistant::setTTL(int _minutes)
{
    if (_minutes <= 0)
        return;
    ttlMinutes = _minutes;
    // Apply the new age limit at once: spots and raw activity beyond it are
    // dropped and both band metrics follow.
    if (model != nullptr)
    {
        model->removeOlderThan(ttlMinutes);
        model->refreshAges();
    }
    pruneBandActivity();
    updateBandSummary();
}

void DXClusterAssistant::setMaxSpots(int _max)
{
    if (_max > 0)
    {
        maxSpots = _max;
        enforceMaxSpots();
        updateBandSummary();
    }
}

void DXClusterAssistant::enforceMaxSpots()
{
    if (model == nullptr)
        return;
    while (model->spotCount() > maxSpots)
    {   // Evict the least valuable spot (lowest score, tiebreak included)
        int worst = model->worstSpotRow();
        if (worst < 0)
            break;
        model->removeSpotAt(worst);
    }
}

void DXClusterAssistant::clearHiddenSpots()
{
    hiddenCalls.clear();
    if (proxy != nullptr)
        proxy->invalidate();
    updateBandSummary();
}

void DXClusterAssistant::clearAll()
{
    if (model != nullptr)
        model->clearSpots();
    hiddenCalls.clear();
    workedQSOs.clear();   // Otherwise a station worked this session could never come back
    bandActivity.clear();
    if (proxy != nullptr)
        proxy->invalidate();
    updateBandSummary();
}

// Hidden when the table is first built: the Priority column already condenses
// the score and the MW rank, and most cluster spots carry no mode. All three
// can be brought back through the Columns entry of the context menu.
bool DXClusterAssistant::isDefaultHiddenColumn(int _column)
{
    return (_column == DXAssistantSpotModel::ColScore)
        || (_column == DXAssistantSpotModel::ColMWRank)
        || (_column == DXAssistantSpotModel::ColMode);
}

void DXClusterAssistant::applyDefaultColumns()
{
    if (tableView == nullptr)
        return;
    for (int i = 0; i < DXAssistantSpotModel::ColumnCount; i++)
        tableView->setColumnHidden(i, isDefaultHiddenColumn(i));
}

bool DXClusterAssistant::filtersAreDefault() const
{
    if (!disabledBands.isEmpty() || !disabledStatuses.isEmpty()
        || !disabledDXCCs.isEmpty() || !disabledSources.isEmpty())
        return false;
    if (followMyBand || (spotterFilter != DXAssistantProxyModel::SpotterAll))
        return false;
    if ((ttlMinutes != SPOT_TTL_MINUTES) || (maxSpots != MAX_SPOTS_DEFAULT))
        return false;
    if (tableView != nullptr)
    {
        for (int i = 0; i < DXAssistantSpotModel::ColumnCount; i++)
        {
            if (tableView->isColumnHidden(i) != isDefaultHiddenColumn(i))
                return false;
        }
    }
    return true;
}

void DXClusterAssistant::resetAllFilters()
{
    disabledBands.clear();
    disabledStatuses.clear();
    disabledDXCCs.clear();
    disabledSources.clear();
    spotterFilter = DXAssistantProxyModel::SpotterAll;
    followMyBand  = false;
    applyDefaultColumns();
    // Through the setters: going back to the default cap trims a list that
    // grew past it, and the default age drops whatever is already older.
    setMaxSpots(MAX_SPOTS_DEFAULT);
    setTTL(SPOT_TTL_MINUTES);
    applyViewFilters();
}

QList<DXSpot> DXClusterAssistant::shownSpots() const
{
    QList<DXSpot> spots;
    if ((proxy == nullptr) || (model == nullptr))
        return spots;

    for (int row = 0; row < proxy->rowCount(); row++)
    {
        QModelIndex sourceIndex = proxy->mapToSource(proxy->index(row, 0));
        if (sourceIndex.isValid())
            spots.append(model->spotAt(sourceIndex.row()));
    }
    return spots;
}

void DXClusterAssistant::slotTimerTick()
{
    if (model == nullptr)
        return;
    model->removeOlderThan(ttlMinutes);
    model->refreshAges();
    pruneBandActivity();
    updateBandSummary();
}

bool DXClusterAssistant::spotForProxyIndex(const QModelIndex &_index, DXSpot &_spot) const
{
    if (!_index.isValid() || (proxy == nullptr) || (model == nullptr))
        return false;
    QModelIndex sourceIndex = proxy->mapToSource(_index);
    if (!sourceIndex.isValid())
        return false;
    _spot = model->spotAt(sourceIndex.row());
    return true;
}

void DXClusterAssistant::slotDoubleClicked(const QModelIndex &_index)
{
    DXSpot spot;
    if (spotForProxyIndex(_index, spot))
        emit spotSendToUI(spot);
}

void DXClusterAssistant::slotTableContextMenu(const QPoint &_pos)
{
    const QModelIndex clicked = tableView->indexAt(_pos);
    // Right-clicking outside the selection works on the row under the cursor,
    // as everywhere else: the menu must never act on rows the user cannot see
    // it is about to act on.
    if (clicked.isValid() && (tableView->selectionModel() != nullptr)
        && !tableView->selectionModel()->isRowSelected(clicked.row(), clicked.parent()))
    {
        tableView->selectionModel()->select(clicked,
                                            QItemSelectionModel::ClearAndSelect |
                                            QItemSelectionModel::Rows);
        tableView->setCurrentIndex(clicked);
    }

    DXSpot spot;
    bool hasSpot = spotForProxyIndex(clicked, spot);
    showContextMenu(tableView->viewport()->mapToGlobal(_pos),
                    tableView->columnAt(_pos.x()), spot, hasSpot);
}

QList<DXSpot> DXClusterAssistant::selectedSpots() const
{
    QList<DXSpot> spots;
    if ((tableView == nullptr) || (tableView->selectionModel() == nullptr))
        return spots;

    for (const QModelIndex &index : tableView->selectionModel()->selectedRows())
    {
        DXSpot spot;
        if (spotForProxyIndex(index, spot))
            spots.append(spot);
    }
    return spots;
}

void DXClusterAssistant::slotHeaderContextMenu(const QPoint &_pos)
{
    if (tableView == nullptr)
        return;
    QHeaderView *header = tableView->horizontalHeader();
    // The header carries no spot: the Spot submenu is shown disabled so the
    // menu stays identical wherever the user opened it.
    showContextMenu(header->mapToGlobal(_pos), header->logicalIndexAt(_pos),
                    DXSpot(), false);
}

QList<int> DXClusterAssistant::dxccsInView() const
{
    // Entities currently held by the widget, plus the ones already filtered
    // out (so the user can always switch them back on), sorted by name.
    QSet<int> entities = disabledDXCCs;
    if (model != nullptr)
    {
        for (int i = 0; i < model->spotCount(); i++)
        {
            int dxcc = model->spotAt(i).getDXCC();
            if (dxcc > 0)
                entities.insert(dxcc);
        }
    }

    QList<int> sorted = entities.values();
    World *w = world;
    std::sort(sorted.begin(), sorted.end(), [w](int _a, int _b)
    {
        if (w == nullptr)
            return _a < _b;
        return w->getEntityName(_a).localeAwareCompare(w->getEntityName(_b)) < 0;
    });
    return sorted;
}

void DXClusterAssistant::showContextMenu(const QPoint &_globalPos, int _column,
                                         const DXSpot &_spot, bool _hasSpot)
{
    if ((tableView == nullptr) || (model == nullptr) || (proxy == nullptr))
        return;

    QMenu menu(this);

    // 1 - Hiding spots is what the operator does most often, so it opens the
    // menu. It works on the whole selection: several spots can be picked and
    // dropped from the list in one go.
    const QList<DXSpot> selection = selectedSpots();
    const bool severalSpots = (selection.count() > 1);
    QAction *hideAct = menu.addAction(severalSpots ? tr("Hide these spots")
                                                   : tr("Hide this spot"));
    hideAct->setToolTip(severalSpots
                        ? tr("Hide these callsigns for the rest of the session.")
                        : tr("Hide this callsign for the rest of the session."));
    hideAct->setEnabled(_hasSpot);

    // 2 - Copy Callsign: the other action worth reaching without a submenu
    QAction *copyCallAct = menu.addAction(tr("Copy Callsign"));
    copyCallAct->setToolTip(tr("Copy the DX callsign of this spot to the clipboard."));
    copyCallAct->setEnabled(_hasSpot);

    menu.addSeparator();

    // 2 - Filters: everything that shapes what the table shows
    QMenu *filtersMenu = menu.addMenu(tr("Filters"));

    // 2.1 - Reset all: back to the list the DX Assistant starts with, without
    // walking every submenu back. Greyed out while nothing is filtered, so the
    // menu tells at a glance whether the list is being narrowed down.
    QAction *resetFiltersAct = filtersMenu->addAction(tr("Reset all"));
    resetFiltersAct->setToolTip(tr("Remove all filtering and put every filter back to its default."));
    resetFiltersAct->setEnabled(!filtersAreDefault());
    filtersMenu->addSeparator();

    // 2.2 - Columns
    int visibleColumns = 0;
    QList<int> hiddenColumns;
    for (int i = 0; i < DXAssistantSpotModel::ColumnCount; i++)
    {
        if (tableView->isColumnHidden(i))
            hiddenColumns.append(i);
        else
            visibleColumns++;
    }

    QMenu *columnsMenu = filtersMenu->addMenu(tr("Columns"));

    // Never allow hiding the last visible column
    QAction *hideColumnAct = columnsMenu->addAction(tr("Hide this column"));
    hideColumnAct->setEnabled((_column >= 0) && !tableView->isColumnHidden(_column)
                              && (visibleColumns > 1));

    QAction *showAllColumnsAct = nullptr;
    QHash<QAction *, int> showColumnActions;
    QMenu *showColumnsMenu = columnsMenu->addMenu(tr("Show"));
    showColumnsMenu->setEnabled(!hiddenColumns.isEmpty());
    if (!hiddenColumns.isEmpty())
    {
        showAllColumnsAct = showColumnsMenu->addAction(tr("All"));
        showColumnsMenu->addSeparator();
        for (int column : hiddenColumns)
        {
            QAction *act = showColumnsMenu->addAction(
                model->headerData(column, Qt::Horizontal, Qt::DisplayRole).toString());
            showColumnActions.insert(act, column);
        }
    }

    // 2.3 - Bands: "Follow my band" first, then every band present in the
    // list plus the ones already filtered out, checkable one by one.
    QMenu *bandsMenu = filtersMenu->addMenu(tr("Bands"));
    QAction *followBandAct = bandsMenu->addAction(tr("Follow my band"));
    followBandAct->setCheckable(true);
    followBandAct->setChecked(followMyBand);
    followBandAct->setToolTip(tr("Show only the spots on the band you are working on."));
    bandsMenu->addSeparator();

    QSet<int> menuBands = disabledBands;
    for (int i = 0; i < model->spotCount(); i++)
        menuBands.insert(model->spotAt(i).getBandId());
    QList<int> sortedBands = menuBands.values();
    std::sort(sortedBands.begin(), sortedBands.end());

    QHash<QAction *, int> bandActions;
    for (int bandId : sortedBands)
    {
        QString bandName = (dataProxy != nullptr) ? dataProxy->getNameFromBandId(bandId)
                                                  : QString::number(bandId);
        if (bandName.isEmpty())
            bandName = QString::number(bandId);
        QAction *act = bandsMenu->addAction(bandName);
        act->setCheckable(true);
        act->setChecked(!disabledBands.contains(bandId));
        // While following the rig the per-band choices do not apply
        act->setEnabled(!followMyBand);
        bandActions.insert(act, bandId);
    }

    // 2.4 - Spotter: who the spotting station must be, or where. Exclusive
    // choice, from the narrowest to the widest.
    QMenu *spotterMenu = filtersMenu->addMenu(tr("Spotter"));
    QActionGroup *spotterGroup = new QActionGroup(spotterMenu);
    spotterGroup->setExclusive(true);
    const QList<QPair<DXAssistantProxyModel::SpotterFilter, QString>> spotterOptions =
        { {DXAssistantProxyModel::SpotterMyCall,      tr("My call")},
          {DXAssistantProxyModel::SpotterMyDXCC,      tr("My DXCC")},
          {DXAssistantProxyModel::SpotterMyContinent, tr("My continent")},
          {DXAssistantProxyModel::SpotterAll,         tr("ALL")} };
    QHash<QAction *, int> spotterActions;
    for (const auto &option : spotterOptions)
    {
        QAction *act = spotterMenu->addAction(option.second);
        act->setCheckable(true);
        act->setChecked(spotterFilter == option.first);
        spotterGroup->addAction(act);
        spotterActions.insert(act, static_cast<int>(option.first));
    }

    // 2.5 - Age: how long a spot stays in the list
    QMenu *ageMenu = filtersMenu->addMenu(tr("Age"));
    const QList<QPair<int, QString>> ageOptions =
        { {15, tr("15 minutes")}, {30, tr("30 minutes")},
          {60, tr("1 hour")},     {120, tr("2 hours")} };
    QHash<QAction *, int> ageActions;
    for (const auto &option : ageOptions)
    {
        QAction *act = ageMenu->addAction(option.second);
        act->setCheckable(true);
        act->setChecked(ttlMinutes == option.first);
        ageActions.insert(act, option.first);
    }

    // 2.6 - Number of spots the widget manages
    QMenu *maxSpotsMenu = filtersMenu->addMenu(tr("Number spots"));
    const QList<int> maxSpotsOptions = { 10, 25, 50, 75, 100, 200, 500 };
    QHash<QAction *, int> maxSpotsActions;
    for (int option : maxSpotsOptions)
    {
        QAction *act = maxSpotsMenu->addAction(QString::number(option));
        act->setCheckable(true);
        act->setChecked(maxSpots == option);
        maxSpotsActions.insert(act, option);
    }

    // 2.7 - Status: the ATNO/Needed/Worked triple the Status column shows
    QMenu *statusMenu = filtersMenu->addMenu(tr("Status"));
    const QList<QSOStatus> statusOptions = { ATNO, needed, worked };
    QHash<QAction *, int> statusActions;
    for (QSOStatus status : statusOptions)
    {
        QAction *act = statusMenu->addAction(DXAssistantSpotModel::statusName(status));
        act->setCheckable(true);
        act->setChecked(!disabledStatuses.contains(static_cast<int>(status)));
        statusActions.insert(act, static_cast<int>(status));
    }

    // 2.8 - Source: where the spots come from, one entry per source KLog can
    // be fed from, whether or not it is currently feeding it.
    QMenu *sourceMenu = filtersMenu->addMenu(tr("Source"));
    const QList<SpotSource> sourceOptions = { SpotSourceDXCluster, SpotSourceWSJTX };
    QHash<QAction *, int> sourceActions;
    for (SpotSource source : sourceOptions)
    {
        QAction *act = sourceMenu->addAction(DXAssistantSpotModel::sourceName(source));
        act->setCheckable(true);
        act->setChecked(!disabledSources.contains(static_cast<int>(source)));
        sourceActions.insert(act, static_cast<int>(source));
    }

    // 2.9 - DXCC: the entities currently in the DX Assistant
    QMenu *dxccMenu = filtersMenu->addMenu(tr("DXCC"));
    QHash<QAction *, int> dxccActions;
    for (int dxcc : dxccsInView())
    {
        QString name = (world != nullptr) ? world->getEntityName(dxcc) : QString();
        if (name.isEmpty())
            name = QString::number(dxcc);
        QAction *act = dxccMenu->addAction(name);
        act->setCheckable(true);
        act->setChecked(!disabledDXCCs.contains(dxcc));
        dxccActions.insert(act, dxcc);
    }
    dxccMenu->setEnabled(!dxccActions.isEmpty());

    // 3 - Spot: the actions that need a spot under the cursor. Opened from
    // the header there is none, so the whole submenu is disabled.
    QMenu *spotMenu = menu.addMenu(tr("Spot"));
    spotMenu->setEnabled(_hasSpot);

    QAction *sendToUIAct = spotMenu->addAction(tr("Send to UI"));
    sendToUIAct->setToolTip(tr("Fill the QSO entry form with this spot."));

    QAction *logDirectlyAct = spotMenu->addAction(tr("Log this QSO"));
    logDirectlyAct->setToolTip(tr("Add this QSO to the log immediately."));

    // Offered only with a radio connected: without one there is nothing to tune
    QAction *qsyAct = nullptr;
    if (rigConnected)
    {
        qsyAct = spotMenu->addAction(tr("QSY to this freq"));
        qsyAct->setToolTip(tr("Tune the radio to the frequency of this spot."));
    }

    QAction *qrzAct = spotMenu->addAction(tr("Look up on QRZ.com"));
    qrzAct->setToolTip(tr("Open this callsign's page on QRZ.com."));

    menu.addSeparator();

    // 4 - Whole-list actions
    QAction *clearAllAct = menu.addAction(tr("Clear all"));
    clearAllAct->setToolTip(tr("Remove every spot and list again the ones hidden or worked this session."));
    clearAllAct->setEnabled((model->spotCount() > 0) || !hiddenCalls.isEmpty()
                            || !workedQSOs.isEmpty());

    QAction *showToMapAct = menu.addAction(tr("Show to map"));
    showToMapAct->setToolTip(tr("Plot the spots currently shown on the map."));
    showToMapAct->setEnabled(proxy->rowCount() > 0);

    QAction *refreshAct = menu.addAction(tr("Refresh"));
    refreshAct->setToolTip(tr("Score every spot again against your log."));

    QAction *chosen = menu.exec(_globalPos);
    if (chosen == nullptr)
        return;

    DXSpot spot(_spot);

    if (chosen == hideAct)
    {
        QStringList calls;
        for (DXSpot selected : selection)
            calls.append(selected.getDxCall());
        if (calls.isEmpty())
            calls.append(spot.getDxCall());   // Nothing selected: the one clicked
        hideSpotCalls(calls);
    }
    else if (chosen == copyCallAct)
    {
        QGuiApplication::clipboard()->setText(spot.getDxCall());
    }
    else if (chosen == resetFiltersAct)
    {
        resetAllFilters();
    }
    else if (chosen == hideColumnAct)
    {
        tableView->setColumnHidden(_column, true);
    }
    else if ((showAllColumnsAct != nullptr) && (chosen == showAllColumnsAct))
    {
        for (int column : hiddenColumns)
            tableView->setColumnHidden(column, false);
    }
    else if (showColumnActions.contains(chosen))
    {
        tableView->setColumnHidden(showColumnActions.value(chosen), false);
    }
    else if (chosen == followBandAct)
    {
        followMyBand = !followMyBand;
        applyViewFilters();
    }
    else if (bandActions.contains(chosen))
    {
        int bandId = bandActions.value(chosen);
        if (disabledBands.contains(bandId))
            disabledBands.remove(bandId);
        else
            disabledBands.insert(bandId);
        applyViewFilters();
    }
    else if (spotterActions.contains(chosen))
    {
        spotterFilter = static_cast<DXAssistantProxyModel::SpotterFilter>(
            spotterActions.value(chosen));
        applyViewFilters();
    }
    else if (ageActions.contains(chosen))
    {
        setTTL(ageActions.value(chosen));   // Purges over-age spots and refreshes
    }
    else if (maxSpotsActions.contains(chosen))
    {
        setMaxSpots(maxSpotsActions.value(chosen));
    }
    else if (statusActions.contains(chosen))
    {
        int status = statusActions.value(chosen);
        if (disabledStatuses.contains(status))
            disabledStatuses.remove(status);
        else
            disabledStatuses.insert(status);
        applyViewFilters();
    }
    else if (sourceActions.contains(chosen))
    {
        int source = sourceActions.value(chosen);
        if (disabledSources.contains(source))
            disabledSources.remove(source);
        else
            disabledSources.insert(source);
        applyViewFilters();
    }
    else if (dxccActions.contains(chosen))
    {
        int dxcc = dxccActions.value(chosen);
        if (disabledDXCCs.contains(dxcc))
            disabledDXCCs.remove(dxcc);
        else
            disabledDXCCs.insert(dxcc);
        applyViewFilters();
    }
    else if (chosen == clearAllAct)
    {
        clearAll();
    }
    else if (chosen == showToMapAct)
    {
        emit spotsSendToMap(shownSpots());
    }
    else if (chosen == refreshAct)
    {
        recalculateAll();   // Re-scores every spot; no-op without an engine
        if (model != nullptr)
        {
            model->removeOlderThan(ttlMinutes);
            model->refreshAges();
        }
        pruneBandActivity();
        updateBandSummary();
    }
    else if (_hasSpot)
    {
        if (chosen == sendToUIAct)
            emit spotSendToUI(spot);
        else if (chosen == logDirectlyAct)
            emit spotLogDirectly(spot);
        else if ((qsyAct != nullptr) && (chosen == qsyAct))
            emit spotQSY(spot);
        else if (chosen == qrzAct)
            QDesktopServices::openUrl(QUrl(QStringLiteral("https://www.qrz.com/db/") + spot.getDxCall()));
    }
}


void DXClusterAssistant::applyViewFilters()
{
    if (proxy != nullptr)
    {
        QString userContinent = (engine != nullptr) ? engine->getUserContinent() : QString();
        int userDXCC = (engine != nullptr) ? engine->getUserDXCC() : -1;
        QString userCallsign = (engine != nullptr) ? engine->getUserCallsign() : QString();
        proxy->setSpotterFilter(spotterFilter, userContinent, userDXCC, userCallsign);
        proxy->setFollowBand(followMyBand, currentBandId);
        proxy->invalidate();
    }
    updateBandSummary();
}

bool DXClusterAssistant::spotIsShown(DXSpot _spot) const
{
    // Mirror of DXAssistantProxyModel::filterAcceptsRow
    if (hiddenCalls.contains(_spot.getDxCall()))
        return false;
    if (followMyBand && (currentBandId > 0))
    {
        if (_spot.getBandId() != currentBandId)
            return false;
    }
    else if (disabledBands.contains(_spot.getBandId()))
    {
        return false;
    }
    if (disabledDXCCs.contains(_spot.getDXCC()))
        return false;
    if (disabledSources.contains(static_cast<int>(_spot.getSource())))
        return false;
    if ((model != nullptr)
        && disabledStatuses.contains(static_cast<int>(model->statusOf(_spot))))
        return false;
    if ((spotterFilter == DXAssistantProxyModel::SpotterMyContinent) && (engine != nullptr))
    {
        QString userContinent = engine->getUserContinent();
        if (!userContinent.isEmpty() && (_spot.getSpotterContinent() != userContinent))
            return false;
    }
    else if ((spotterFilter == DXAssistantProxyModel::SpotterMyDXCC) && (engine != nullptr))
    {
        int userDXCC = engine->getUserDXCC();
        if ((userDXCC > 0) && (_spot.getSpotterDXCC() != userDXCC))
            return false;
    }
    else if ((spotterFilter == DXAssistantProxyModel::SpotterMyCall) && (engine != nullptr))
    {
        QString userCallsign = engine->getUserCallsign();
        if (!userCallsign.isEmpty() && (_spot.getSpotter().toUpper() != userCallsign))
            return false;
    }
    return true;
}

void DXClusterAssistant::hideSpotCall(const QString &_call)
{
    hideSpotCalls(QStringList(_call));
}

void DXClusterAssistant::hideSpotCalls(const QStringList &_calls)
{
    // A batch of spots is hidden in one go: the view is only rebuilt once,
    // however many of them the user picked.
    bool hidden = false;
    for (const QString &call : _calls)
    {
        if (call.isEmpty())
            continue;
        hiddenCalls.insert(call);
        hidden = true;
    }
    if (!hidden)
        return;

    if (proxy != nullptr)
        proxy->invalidate();
    updateBandSummary();
}
