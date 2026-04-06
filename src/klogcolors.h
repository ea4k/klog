#ifndef KLOG_KLOGCOLORS_H
#define KLOG_KLOGCOLORS_H
/***************************************************************************
                          klogdefinitions.h  -  description
                             -------------------
    begin                : oct 2020
    copyright            : (C) 2020 by Jaime Robles
    user                : jaime@robles.es
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

#include <QColor>

inline const QColor KLOG_COLOR_NEW_ONE   { QColor("#FF0000") };
inline const QColor KLOG_COLOR_NEEDED    { QColor("#FF8C00") };
inline const QColor KLOG_COLOR_WORKED    { QColor("#FFD700") };
inline const QColor KLOG_COLOR_CONFIRMED { QColor("#32CD32") };
inline const QColor KLOG_COLOR_DEFAULT   { QColor("#00BFFF") };

inline constexpr int KLOG_LOCATOR_ALPHA = 153;

#endif // KLOG_KLOGCOLORS_H

