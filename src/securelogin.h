#ifndef SECURELOGIN_H
#define SECURELOGIN_H
/***************************************************************************
                          securelogin  -  description
                             -------------------
    begin                : Jun 2021
    copyright            : (C) 2021 by Ladislav Foldyna
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
class SecureLogin
{

public:
    SecureLogin(const QString &_secureStorageKey, const QString &_enterPasswordLabel);
    ~SecureLogin();
    void setUser(const QString _user);
    void setPass(const QString _pass);
    QString getUser();
    QString getPass(bool askEmptyPass=true);

private:
    QString user, pass, secureStorageKey, enterPasswordLabel;
    bool savePassword = true;

    int  savePass();
    void delPass();
};

#endif // SECURELOGIN_H
