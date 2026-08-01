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

#include <QDesktopServices>
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

QString DXAssistantSpotModel::statusText(const DXSpot &_spot) const
{
    switch (effectiveStatus(_spot))
    {
    case ATNO:   return tr("ATNO");
    case needed: return tr("Needed");
    case worked: return tr("Worked");
    default:     return QString();
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

    if (role == Qt::ToolTipRole)
    {   // Exact UTC arrival time of the spot
        if (index.column() == ColAge)
            return tr("Received at %1 UTC").arg(spot.getDateTime().toUTC().toString("HH:mm"));
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
    hiddenCalls     = nullptr;
    disabledBands   = nullptr;
    onlyMyContinent = false;
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

void DXAssistantProxyModel::setOnlyContinent(bool _enabled, const QString &_continent)
{
    onlyMyContinent = _enabled;
    userContinent   = _continent;
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
    if (disabledBands != nullptr)
    {
        int bandId = sourceModel()->data(idx, DXAssistantSpotModel::BandIdRole).toInt();
        if (disabledBands->contains(bandId))
            return false;
    }
    if (onlyMyContinent && !userContinent.isEmpty())
    {
        QString continent = sourceModel()->data(idx, DXAssistantSpotModel::SpotterContinentRole).toString();
        if (continent != userContinent)
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
    clearHiddenButton = nullptr;
    mostActiveBandLabel = nullptr;
    bandToBeLabel = nullptr;
    ttlTimer   = nullptr;
    ttlMinutes = SPOT_TTL_MINUTES;
    maxSpots   = MAX_SPOTS_DEFAULT;
    onlyMyContinentSpotters = false;

    // Floating independent window even when a parent keeps the lifetime
    setWindowFlags(Qt::Window);
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

    tableView = new QTableView(this);
    tableView->setModel(proxy);
    tableView->setSortingEnabled(true);
    tableView->sortByColumn(DXAssistantSpotModel::ColScore, Qt::DescendingOrder);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    tableView->verticalHeader()->setVisible(false);
    tableView->horizontalHeader()->setStretchLastSection(true);
    // Hidden by default: the Priority column condenses the score and the MW
    // rank, and most cluster spots carry no mode. All three can be brought
    // back through the header's Show columns menu.
    tableView->setColumnHidden(DXAssistantSpotModel::ColScore, true);
    tableView->setColumnHidden(DXAssistantSpotModel::ColMWRank, true);
    tableView->setColumnHidden(DXAssistantSpotModel::ColMode, true);
    tableView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(tableView, &QTableView::doubleClicked,
            this, &DXClusterAssistant::slotDoubleClicked);
    connect(tableView, &QTableView::customContextMenuRequested,
            this, &DXClusterAssistant::slotContextMenu);
    connect(tableView->horizontalHeader(), &QHeaderView::customContextMenuRequested,
            this, &DXClusterAssistant::slotHeaderContextMenu);

    clearHiddenButton = new QPushButton(tr("Clear hidden spots"), this);
    clearHiddenButton->setToolTip(tr("Show again all the spots hidden during this session."));
    clearHiddenButton->setEnabled(false);
    connect(clearHiddenButton, &QPushButton::clicked,
            this, &DXClusterAssistant::slotClearHiddenClicked);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(clearHiddenButton);

    // Summary bar pinned above the spot table (#796 / #860)
    mostActiveBandLabel = new QLabel(this);
    mostActiveBandLabel->setToolTip(tr("The band where most DX spots are arriving right now."));
    bandToBeLabel = new QLabel(this);
    bandToBeLabel->setToolTip(tr("The band where you have the most to gain, based on your log."));

    QHBoxLayout *summaryLayout = new QHBoxLayout;
    summaryLayout->addWidget(mostActiveBandLabel);
    summaryLayout->addStretch();
    summaryLayout->addWidget(bandToBeLabel);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(summaryLayout);
    layout->addWidget(tableView);
    layout->addLayout(buttonLayout);
    setLayout(layout);

    updateBandSummary();

    ttlTimer = new QTimer(this);
    ttlTimer->setInterval(60 * 1000);
    connect(ttlTimer, &QTimer::timeout, this, &DXClusterAssistant::slotTimerTick);
    ttlTimer->start();

    resize(700, 400);
    return true;
}

void DXClusterAssistant::setEngine(DXAssistantEngine *_engine)
{
    engine = _engine;
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

    if (hiddenCalls.contains(spot.getDxCall()))
        return;   // Hidden this session: silently dropped

    int row = model->indexOf(spot.getDxCall(), spot.getBandId());
    if (row < 0)
    {
        model->addSpot(spot);
        enforceMaxSpots();
        updateBandSummary();
        return;
    }

    // Duplicate (same call+band): replace only if the new spotter is "more
    // positive" — same continent as the user while the existing one is not.
    DXSpot existing = model->spotAt(row);
    QString userContinent = (engine != nullptr) ? engine->getUserContinent() : QString();
    bool newSame = !userContinent.isEmpty() && (spot.getSpotterContinent() == userContinent);
    bool oldSame = !userContinent.isEmpty() && (existing.getSpotterContinent() == userContinent);

    if (newSame && !oldSame)
    {
        model->replaceSpot(row, spot);
        updateBandSummary();
        return;
    }

    existing.setDateTime(spot.getDateTime());
    existing.setSpotter(spot.getSpotter());
    existing.setSpotterContinent(spot.getSpotterContinent());
    existing.setComment(spot.getComment());
    model->replaceSpot(row, existing);
    updateBandSummary();
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
    updateClearHiddenButton();
    updateBandSummary();
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

void DXClusterAssistant::slotContextMenu(const QPoint &_pos)
{
    DXSpot spot;
    if (!spotForProxyIndex(tableView->indexAt(_pos), spot))
        return;

    QMenu menu(this);

    QAction *sendToUIAct = menu.addAction(tr("Send to UI"));
    sendToUIAct->setToolTip(tr("Fill the QSO entry form with this spot."));

    QAction *logDirectlyAct = menu.addAction(tr("Log this QSO"));
    logDirectlyAct->setToolTip(tr("Add this QSO to the log immediately."));

    QAction *hideAct = menu.addAction(tr("Hide this spot"));
    hideAct->setToolTip(tr("Hide this callsign for the rest of the session."));

    QAction *qrzAct = menu.addAction(tr("Look up on QRZ.com"));
    qrzAct->setToolTip(tr("Open this callsign's page on QRZ.com."));

    QAction *chosen = menu.exec(tableView->viewport()->mapToGlobal(_pos));
    if (chosen == sendToUIAct)
        emit spotSendToUI(spot);
    else if (chosen == logDirectlyAct)
        emit spotLogDirectly(spot);
    else if (chosen == hideAct)
        hideSpotCall(spot.getDxCall());
    else if (chosen == qrzAct)
        QDesktopServices::openUrl(QUrl(QStringLiteral("https://www.qrz.com/db/") + spot.getDxCall()));
}

void DXClusterAssistant::slotHeaderContextMenu(const QPoint &_pos)
{
    if ((tableView == nullptr) || (model == nullptr))
        return;

    QHeaderView *header = tableView->horizontalHeader();
    int clickedColumn = header->logicalIndexAt(_pos);

    int visibleColumns = 0;
    QList<int> hiddenColumns;
    for (int i = 0; i < DXAssistantSpotModel::ColumnCount; i++)
    {
        if (tableView->isColumnHidden(i))
            hiddenColumns.append(i);
        else
            visibleColumns++;
    }

    QMenu menu(this);

    // 1 - Hide this column (never allow hiding the last visible one)
    QAction *hideColumnAct = menu.addAction(tr("Hide this column"));
    hideColumnAct->setEnabled((clickedColumn >= 0) && (visibleColumns > 1));

    // 2 - Show columns (only offered while something is hidden)
    QAction *showAllColumnsAct = nullptr;
    QHash<QAction *, int> showColumnActions;
    if (!hiddenColumns.isEmpty())
    {
        QMenu *showColumnsMenu = menu.addMenu(tr("Show columns"));
        showAllColumnsAct = showColumnsMenu->addAction(tr("All"));
        showColumnsMenu->addSeparator();
        for (int column : hiddenColumns)
        {
            QAction *act = showColumnsMenu->addAction(
                model->headerData(column, Qt::Horizontal, Qt::DisplayRole).toString());
            showColumnActions.insert(act, column);
        }
    }

    // 3 - Filter bands: every band present in the list plus the ones already
    // filtered out, checkable to enable/disable each one
    QSet<int> menuBands = disabledBands;
    for (int i = 0; i < model->spotCount(); i++)
        menuBands.insert(model->spotAt(i).getBandId());
    QList<int> sortedBands = menuBands.values();
    std::sort(sortedBands.begin(), sortedBands.end());

    QMenu *bandsMenu = menu.addMenu(tr("Filter bands"));
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
        bandActions.insert(act, bandId);
    }
    bandsMenu->setEnabled(!bandActions.isEmpty());

    // 4 - Spotter continent toggle
    QAction *continentAct = menu.addAction(onlyMyContinentSpotters
                                               ? tr("Show all DX spotters")
                                               : tr("Only my continent DX spotters"));

    // 5 - Max age for spots
    QMenu *ageMenu = menu.addMenu(tr("Max age for spots"));
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

    // 6 - Max number of spots
    QMenu *maxSpotsMenu = menu.addMenu(tr("Max number of spots"));
    const QList<int> maxSpotsOptions = { 10, 25, 50, 75, 100, 200, 500 };
    QHash<QAction *, int> maxSpotsActions;
    for (int option : maxSpotsOptions)
    {
        QAction *act = maxSpotsMenu->addAction(QString::number(option));
        act->setCheckable(true);
        act->setChecked(maxSpots == option);
        maxSpotsActions.insert(act, option);
    }

    QAction *chosen = menu.exec(header->mapToGlobal(_pos));
    if (chosen == nullptr)
        return;

    if (chosen == hideColumnAct)
    {
        tableView->setColumnHidden(clickedColumn, true);
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
    else if (bandActions.contains(chosen))
    {
        int bandId = bandActions.value(chosen);
        if (disabledBands.contains(bandId))
            disabledBands.remove(bandId);
        else
            disabledBands.insert(bandId);
        applyViewFilters();
    }
    else if (chosen == continentAct)
    {
        onlyMyContinentSpotters = !onlyMyContinentSpotters;
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
}

void DXClusterAssistant::applyViewFilters()
{
    if (proxy != nullptr)
    {
        QString userContinent = (engine != nullptr) ? engine->getUserContinent() : QString();
        proxy->setOnlyContinent(onlyMyContinentSpotters, userContinent);
        proxy->invalidate();
    }
    updateBandSummary();
}

bool DXClusterAssistant::spotIsShown(DXSpot _spot) const
{
    // Mirror of DXAssistantProxyModel::filterAcceptsRow
    if (hiddenCalls.contains(_spot.getDxCall()))
        return false;
    if (disabledBands.contains(_spot.getBandId()))
        return false;
    if (onlyMyContinentSpotters && (engine != nullptr))
    {
        QString userContinent = engine->getUserContinent();
        if (!userContinent.isEmpty() && (_spot.getSpotterContinent() != userContinent))
            return false;
    }
    return true;
}

void DXClusterAssistant::hideSpotCall(const QString &_call)
{
    if (_call.isEmpty())
        return;
    hiddenCalls.insert(_call);
    if (proxy != nullptr)
        proxy->invalidate();
    updateClearHiddenButton();
    updateBandSummary();
}

void DXClusterAssistant::slotClearHiddenClicked()
{
    clearHiddenSpots();
}

void DXClusterAssistant::updateClearHiddenButton()
{
    if (clearHiddenButton != nullptr)
        clearHiddenButton->setEnabled(!hiddenCalls.isEmpty());
}
