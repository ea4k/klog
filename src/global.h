#ifndef KLOG_GLOBAL_H
#define KLOG_GLOBAL_H
/***************************************************************************
                          global.h  -  description
                             -------------------
    begin                : Apr 2023
    copyright            : (C) 2023 by Jaime Robles
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
/*
    This class defines global topics.
*/
extern bool g_callsignCheck;
#if __cplusplus >= 201309L
    // use modern deprecation feature
#define KLOG_DEPRECATED [[deprecated]]
#else
    //... no idea how to handle this, but this seems overly noisy?
#define KLOG_DEPRECATED
#warning "KLOG_DEPRECATED is not defined yet!"
#endif

#endif
