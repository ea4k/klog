/***************************************************************************
                          securelogin.cpp  -  description
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
#include <QEventLoop>
#include <QString>
#include <QDebug>
#include <QLineEdit>
#include <QInputDialog>
#include <qt5keychain/keychain.h>
#include "securelogin.h"

/*
 * SecureLogin::SecureLogin
 * params: _secureStorageKey - Storage Key unique identificator
 *         _enterPasswordLabel - if a password is requested and no password is present
 *                               then a dialog with enterPasswordLabel is displayed
 */
SecureLogin::SecureLogin(const QString &_secureStorageKey, const QString &_enterPasswordLabel)
{
    emit debugLog (Q_FUNC_INFO, "Start", Info);
    secureStorageKey = _secureStorageKey;
    enterPasswordLabel = _enterPasswordLabel;
    emit debugLog (Q_FUNC_INFO, "STOP", Info);
}

SecureLogin::~SecureLogin()
{
}

void SecureLogin::setUser(const QString _user)
{
    //qDebug() << "SecureLogin::setUser: " << _user << " - START" << endl;

    emit debugLog (Q_FUNC_INFO, QString("Start (%1)").arg(_user), Info);
    // If a username is changed then it is necessary to delete an old record in secure storage.
    // we do not want to store old records in the secure storage.
    if ( _user != user)
    {
        delPass();
    }
    user = _user;
    //qDebug() << "SecureLogin::setUser: - END" << endl;
    emit debugLog (Q_FUNC_INFO, "Stop", Info);
}

void SecureLogin::setPass(const QString _pass)
{
    //qDebug() << "SecureLogin::setPass: " << _pass << " - START" << endl;
    emit debugLog (Q_FUNC_INFO, QString("Start (%1)").arg(_pass.length()), Info);
    pass = _pass;

    // Save the password immediately after the change
    savePass();

    emit debugLog (Q_FUNC_INFO, "Stop", Info);
    //qDebug() << "SecureLogin::setPass - END" << endl;
}

QString SecureLogin::getUser()
{
   return user;
}

int SecureLogin::savePass()
{
    emit debugLog (Q_FUNC_INFO, QString("Start (%1: %2)").arg(user).arg(pass.length()), Info);
    //qDebug() << "SecureLogin::savePass " << user << ";" << pass << " - START" << endl;
    using namespace QKeychain;

    if (!savePassword)
    {
        //qDebug() << "SecureLogin::savePass - does not save a password - END" << endl;
        emit debugLog (Q_FUNC_INFO, "Stop - do no save a password", Info);
        return 0;
    }

    if (user.isEmpty())
    {
       //qDebug() << "SecureLogin::savePass - Empty Username - END" << endl;

       // it is not interesting to save password without username.
       emit debugLog (Q_FUNC_INFO, "Stop - doe not save a passowrd - empty user", Info);
       return 1;
    }

    // write a password to Secure Storage
    WritePasswordJob job(QLatin1String(secureStorageKey.toStdString().c_str()));
    job.setAutoDelete(false);
    job.setKey(user);
    job.setTextData(pass);
    QEventLoop loop;
    job.connect(&job, SIGNAL(finished(QKeychain::Job*)), &loop, SLOT(quit()));
    job.start();
    loop.exec();

    if (job.error())
    {
        //qDebug() << "SecureLogin::savePass - password storage error " << job.errorString() << " - END" << endl;
        emit debugLog (Q_FUNC_INFO, QString("Stop - job error (%1)").arg(job.errorString()), Info);
        return 1;
    }

    emit debugLog (Q_FUNC_INFO, "Stop", Info);
    //qDebug() << "SecureLogin::savePass - END" << endl;
    return 0;
}

QString SecureLogin::getPass(bool askEmptyPass)
{
    using namespace QKeychain;
    QString manual_pass;

    emit debugLog (Q_FUNC_INFO, QString("Start (%1; ask %2)").arg(user).arg(askEmptyPass), Info);
    //qDebug() << "SecureLogin::getPass : " << askEmptyPass << " - START" << endl;

    // if password already loaded then return it immediately
    if (!pass.isEmpty())
    {
       emit debugLog (Q_FUNC_INFO, "Stop - password in cache", Info);
       //qDebug() << "SecureLogin::getPass - password in cache: " << pass << " - END" << endl;
       return pass;
    }

    // get a password from Secure Storage
    ReadPasswordJob job(QLatin1String(secureStorageKey.toStdString().c_str()));
    job.setAutoDelete(false);
    job.setKey(user);
    QEventLoop loop;
    job.connect(&job, SIGNAL(finished(QKeychain::Job*)), &loop, SLOT(quit()));
    job.start();
    loop.exec();

    pass = job.textData();

    if (job.error() || pass.isEmpty())
    {
        // try to ask user when the password is empty or error.
        if (askEmptyPass)
        {
            // no password or error, try to enter password manually
            // do not store manually entered password
            bool ok;
            manual_pass = QInputDialog::getText(nullptr,
                                                QObject::tr("KLog needs a password"), //add TR
                                                enterPasswordLabel,
                                                QLineEdit::Password, "", &ok);
            if (!ok)
            {
               savePassword = true;
               pass = QString();
            }
            else
            {
               // do not save a password when the password is manually entered
               savePassword = false;
               emit debugLog (Q_FUNC_INFO, "Stop - manually entered", Info);
               return manual_pass;
            }
        }
        else
        {
            savePassword = true;
            pass = QString();
        }
    }
    savePassword = true;
    emit debugLog (Q_FUNC_INFO, "Stop", Info);
    //qDebug() << "SecureLogin::getPass :" << pass << " - END" << endl;
    return pass;
}

void SecureLogin::delPass()
{
    using namespace QKeychain;

    emit debugLog (Q_FUNC_INFO, QString("Start (%1)").arg(user), Info);
    //qDebug() << "SecureLogin::delPass " << user << ";" << pass << " - START" << endl;

    if (user.isEmpty())
    {
       emit debugLog (Q_FUNC_INFO, "Stop - empty user", Info);
       //qDebug() << "SecureLogin::delPass - User empty - END" << endl;
       return;
    }

    // delete password from Secure Storage
    DeletePasswordJob job(QLatin1String(secureStorageKey.toStdString().c_str()));
    job.setAutoDelete(false);
    job.setKey(user);
    QEventLoop loop;
    job.connect(&job, SIGNAL(finished(QKeychain::Job*)), &loop, SLOT(quit()));
    job.start();
    loop.exec();

    if (job.error())
    {
        //nothing to do when an error.
        emit debugLog (Q_FUNC_INFO, QString("Stop - job error (%1)").arg(job.errorString()), Info);
        //qDebug() << "SecureLogin::delPass - password storage error " << job.errorString() << endl;
    }

    pass = QString();
    //qDebug() << "SecureLogin::delPass - END" << endl;
    emit debugLog (Q_FUNC_INFO, "Stop", Info);
    return;
}

void SecureLogin::setSeverity(const DebugLogLevel _sev)
{
    logSeverity = _sev;
}
