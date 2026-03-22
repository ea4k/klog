/***************************************************************************
                       locatorinfoprovider.h  -  description
                             -------------------
    begin                : 2024
    copyright            : (C) 2024 by Jaime Robles
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

#ifndef LOCATORINFOPROVIDER_H
#define LOCATORINFOPROVIDER_H

#include <QObject>
#include <QString>
#include <QSet>
#include <QVariant>
#include "../../dataproxy_sqlite.h"

/**
 * @brief QObject-based provider exposed to QML for locator hover tooltip data.
 *
 * Tracks the set of locators currently visible on the map and the active filter
 * (band/mode/propagation/satellite/confirmed). QML calls isLocatorVisible() to
 * decide whether to show a tooltip and getQSOsForLocator() to fill the table.
 */
class LocatorInfoProvider : public QObject
{
    Q_OBJECT

public:
    explicit LocatorInfoProvider(QObject *parent = nullptr);

    void setDataProxy(DataProxy_SQLite *dp);

    /** Update the active filter; used when showFiltered() rebuilds the map. */
    void setFilter(const QString &band, const QString &mode,
                   const QString &prop, const QString &sat, bool confirmed);

    /** Replace the whole set of visible locators at once. */
    void setVisibleLocators(const QSet<QString> &locators);

    /** Add one locator to the visible set. */
    void addVisibleLocator(const QString &locator);

    /** Remove all visible locators (called on clearDataLayers). */
    void clearVisibleLocators();

    /**
     * @brief Returns true if the given locator (any precision) is currently visible.
     *
     * Checks the exact locator first, then 4-char and 2-char prefixes so that
     * a hover anywhere inside a visible parent field also triggers the tooltip.
     */
    Q_INVOKABLE bool isLocatorVisible(const QString &locator) const;

    /**
     * @brief Returns QSO rows for a locator as a list of QVariantMap objects.
     *
     * Each map has keys: "id" (int), "callsign" (string), "band" (string), "mode" (string).
     * Uses the current active filter.
     */
    Q_INVOKABLE QVariantList getQSOsForLocator(const QString &locator) const;

private:
    DataProxy_SQLite *dataProxy = nullptr;
    QSet<QString>    visibleLocators;
    QString          currentBand;
    QString          currentMode;
    QString          currentProp;
    QString          currentSat;
    bool             currentConfirmed = false;
};

#endif // LOCATORINFOPROVIDER_H
