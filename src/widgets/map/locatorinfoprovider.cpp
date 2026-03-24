/***************************************************************************
                       locatorinfoprovider.cpp  -  description
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

#include "locatorinfoprovider.h"

LocatorInfoProvider::LocatorInfoProvider(QObject *parent)
    : QObject(parent)
{
}

void LocatorInfoProvider::setDataProxy(DataProxy_SQLite *dp)
{
    dataProxy = dp;
}

void LocatorInfoProvider::setFilter(const QString &band, const QString &mode,
                                    const QString &prop, const QString &sat,
                                    bool confirmed)
{
    currentBand      = band;
    currentMode      = mode;
    currentProp      = prop;
    currentSat       = sat;
    currentConfirmed = confirmed;
}

void LocatorInfoProvider::setVisibleLocators(const QSet<QString> &locators)
{
    visibleLocators = locators;
}

void LocatorInfoProvider::addVisibleLocator(const QString &locator)
{
    visibleLocators.insert(locator.toUpper());
}

void LocatorInfoProvider::clearVisibleLocators()
{
    visibleLocators.clear();
}

bool LocatorInfoProvider::isLocatorVisible(const QString &locator) const
{
    const QString up = locator.toUpper();
    // Check from most specific to least specific
    if (visibleLocators.contains(up))
        return true;
    if (up.length() > 4 && visibleLocators.contains(up.left(4)))
        return true;
    if (up.length() > 2 && visibleLocators.contains(up.left(2)))
        return true;
    return false;
}

QVariantList LocatorInfoProvider::getQSOsForLocator(const QString &locator) const
{
    if (!dataProxy || locator.isEmpty())
        return QVariantList();

    // Find the best-matching visible locator to use as the search prefix
    const QString up = locator.toUpper();
    QString searchLoc = up;
    if (!visibleLocators.contains(up)) {
        if (up.length() > 4 && visibleLocators.contains(up.left(4)))
            searchLoc = up.left(4);
        else if (up.length() > 2 && visibleLocators.contains(up.left(2)))
            searchLoc = up.left(2);
        else
            return QVariantList();
    }

    return dataProxy->getQSOsForLocator(searchLoc, currentBand, currentMode,
                                        currentProp, currentSat, currentConfirmed);
}
