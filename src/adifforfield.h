#ifndef ADIFFORFIELD_H
#define ADIFFORFIELD_H

/***************************************************************************
                          adifforfield.h  -  description
                             -------------------
    begin                : oct 2021
    copyright            : (C) 2021 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                             *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify        *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,             *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/
#include <QObject>
#include <QString>
#include "klogdefinitions.h"
#include "utilities.h"

class ADIFForField: public QObject {
    Q_OBJECT
    // TODO:
    //      Add checks for all the data (maybe adding the dataProxy to check helps
    //      but may slow down the process as queries will be executed.
public:
    ADIFForField();

    QString getADIFForQSODate(const QString &_data, ExportMode _em = ModeADIF);
    QString getADIFForCall(const QString &_data);
    QString getADIFForRSTSent(const QString &_data);
    QString getADIFForRSTRcvd(const QString &_data);
    QString getADIFForBand(const QString &_data);
    QString getADIFForMode(const QString &_data);
    QString getADIFForSubMode(const QString &_data);
    QString getADIFForCQz(const QString &_data);
    QString getADIFForITUz(const QString &_data);
    QString getADIFForDXCC(const QString &_data);
    QString getADIFForAddress(const QString &_data);
    QString getADIFForAge(const QString &_data);
    QString getADIFForCNTY(const QString &_data);
    QString getADIFForComment(const QString &_data);
    QString getADIFForA_Index(const QString &_data);
    QString getADIFForAnt_az(const QString &_data);
    QString getADIFForAnt_el(const QString &_data);
    QString getADIFForAnt_path(const QString &_data);
    QString getADIFForARRL_sect(const QString &_data);
/*
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
    QString getADIFFor(const QString &_data);
  */


private:
    bool showInvalidCallMessage(const QString &_call);
    QString getADIFPair(const QString &_field, const QString &_data);
    Utilities *util;

};

#endif // ADIFFORFIELD_H
