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
#include "clublogmostwanted.h"
#include "../world.h"

#include <QDesktopServices>
#include <QUrl>

// ─────────────────────────────────────────────────────────────────────────────
// DXAssistantSpotModel
// ─────────────────────────────────────────────────────────────────────────────

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

void DXAssistantSpotModel::addSpot(const DXSpot &_spot)
{
    beginInsertRows(QModelIndex(), spots.count(), spots.count());
    spots.append(_spot);
    endInsertRows();
}

void DXAssistantSpotModel::replaceSpot(int _row, const DXSpot &_spot)
{
    if ((_row < 0) || (_row >= spots.count()))
        return;
    spots[_row] = _spot;
    emit dataChanged(index(_row, 0), index(_row, ColumnCount - 1));
}

void DXAssistantSpotModel::removeSpotAt(int _row)
{
    if ((_row < 0) || (_row >= spots.count()))
        return;
    beginRemoveRows(QModelIndex(), _row, _row);
    spots.removeAt(_row);
    endRemoveRows();
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
    hiddenCalls = nullptr;
    setSortRole(DXAssistantSpotModel::SortRole);
}

void DXAssistantProxyModel::setHiddenCalls(const QSet<QString> *_calls)
{
    hiddenCalls = _calls;
    invalidateFilter();
}

bool DXAssistantProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (hiddenCalls == nullptr)
        return true;
    QModelIndex idx = sourceModel()->index(sourceRow, 0, sourceParent);
    QString call = sourceModel()->data(idx, DXAssistantSpotModel::CallRole).toString();
    return !hiddenCalls->contains(call);
}

int DXAssistantProxyModel::tieBreakValue(const QModelIndex &index) const
{
    // Bigger is better. In-threshold entities beat everything else, and among
    // them a lower Most Wanted rank wins; entities outside the threshold (or
    // unranked) do not take part in tiebreaking.
    QModelIndex rankIdx = sourceModel()->index(index.row(), DXAssistantSpotModel::ColMWRank,
                                               index.parent());
    int rank = sourceModel()->data(rankIdx, DXAssistantSpotModel::SortRole).toInt();
    if ((rank > 0) && (rank <= ClubLogMostWanted::MOST_WANTED_THRESHOLD))
        return ClubLogMostWanted::MOST_WANTED_THRESHOLD - rank + 1;
    return 0;
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
    ttlTimer   = nullptr;
    ttlMinutes = SPOT_TTL_MINUTES;

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

    connect(tableView, &QTableView::doubleClicked,
            this, &DXClusterAssistant::slotDoubleClicked);
    connect(tableView, &QTableView::customContextMenuRequested,
            this, &DXClusterAssistant::slotContextMenu);

    clearHiddenButton = new QPushButton(tr("Clear hidden spots"), this);
    clearHiddenButton->setToolTip(tr("Show again all the spots hidden during this session."));
    clearHiddenButton->setEnabled(false);
    connect(clearHiddenButton, &QPushButton::clicked,
            this, &DXClusterAssistant::slotClearHiddenClicked);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(clearHiddenButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(tableView);
    layout->addLayout(buttonLayout);
    setLayout(layout);

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
        return;
    }

    existing.setDateTime(spot.getDateTime());
    existing.setSpotter(spot.getSpotter());
    existing.setSpotterContinent(spot.getSpotterContinent());
    existing.setComment(spot.getComment());
    model->replaceSpot(row, existing);
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
}

void DXClusterAssistant::setTTL(int _minutes)
{
    if (_minutes > 0)
        ttlMinutes = _minutes;
}

void DXClusterAssistant::clearHiddenSpots()
{
    hiddenCalls.clear();
    if (proxy != nullptr)
        proxy->invalidate();
    updateClearHiddenButton();
}

void DXClusterAssistant::slotTimerTick()
{
    if (model == nullptr)
        return;
    model->removeOlderThan(ttlMinutes);
    model->refreshAges();
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

void DXClusterAssistant::hideSpotCall(const QString &_call)
{
    if (_call.isEmpty())
        return;
    hiddenCalls.insert(_call);
    if (proxy != nullptr)
        proxy->invalidate();
    updateClearHiddenButton();
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
