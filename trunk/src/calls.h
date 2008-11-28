/***************************************************************************
                          calls  -  description
                             -------------------
    begin                : jue jul 22 2004
    copyright            : (C) 2004 by Jaime Robles
    email                : jaime@kde.org
 ***************************************************************************/

/******************************************************************************
 *                                                                            *
 *   This program is free software; you can redistribute it and/or modify     *
*  it under the terms of the GNU General Public License as published by     *
*  the Free Software Foundation; either version 2 of the License, or        *
*  (at your option) any later version.                                      *
 *                                                                            *
*  This program is distributed in the hope that it will be useful,          *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
*  GNU General Public License for more details.                             *
 *                                                                            *
*  You should have received a copy of the GNU General Public License        *
*  along with this program; if not, write to the Free Software              *
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA*
 *****************************************************************************/ 

#ifndef CALLS_H
#define CALLS_H
//#include <klocale.h>
#include <qstring.h>

 #define CALL_LENGTH 15
// This class is to know if we have previously worked a call

class Calls{
public:
  Calls();
  Calls(const QString& tqrz, int tnumb);
  ~Calls();

  void setQrz(const QString &tqrz);
  QString getQrz() const;

//  void setRrst(int trst);
//  int getRrst() const;

  void setNumb (int tnumb);
  int getNumb() const;

  void setComment(const QString &tcomment);
  QString getComment()const;

  void setQslVia(const QString &tInfo); // (bureau, direct, ...)
  QString getQslVia() const;

  void setQslManager(const QString &tqrz); // (Qrz of manager)
  QString getQslManager() const;

  void setQslInfo(const QString &tqslInfo);  // Address, info...
  QString getQslInfo()const;

  void setLocator(const QString &tqrz);
  QString getLocator() const;

  void setIota(const QString &tiota);
  QString getIota() const;

  QString getIotaContinent() const;
  int getIotaNumber() const;

  void setName(const QString &tname);
  QString getName() const;

  void setQth(const QString &tqth);
  QString getQth() const;


private:
  QString qrz;        // CALL
  int numb;           // STX
  QString comment;    // COMMENT
  QString qslVia;     // QSL_VIA (C) //Manager
  QString qslInfo;    // ADDRESS /QSLMSG
  QString manager;
  QString gridSquare; // GRIDSQUARE
  QString iota;       // IOTA
  QString state;    // STATE
  QString name;       // NAME
  QString qth;        // QTH
  QString prop_mode;  // PROP_MODE QSO propagation mode

};


#endif

