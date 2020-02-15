/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>

#include "logviewsortfilterproxymodel.h"


LogViewSortFilterProxyModel::LogViewSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    //setDynamicSortFilter(true);
}

bool LogViewSortFilterProxyModel::lessThan(const QModelIndex &left,
                                      const QModelIndex &right) const
{
    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);

    QString leftString = leftData.toString();
    QString rightString = rightData.toString();
    //qDebug() << "LogViewSortFilterProxyModel::lessThan (leftString):  " << leftString << endl;
    //qDebug() << "LogViewSortFilterProxyModel::lessThan (rightString):  " << rightString << endl;

    QDateTime leftDT = QDateTime::fromString((sourceModel()->data(left.siblingAtColumn(1))).toString() + (sourceModel()->data(left.siblingAtColumn(2))).toString(), "yyyy/MM/ddHH:mm:ss");
    QDateTime rightDT = QDateTime::fromString((sourceModel()->data(right.siblingAtColumn(1))).toString() + (sourceModel()->data(right.siblingAtColumn(2))).toString(), "yyyy/MM/ddHH:mm:ss");

    //qDebug() << "LogViewSortFilterProxyModel::lessThan - DateTime: " << leftDT.toString("yyyy-MM-dd / hh:mm:ss") << endl;
    //qDebug() << "LogViewSortFilterProxyModel::lessThan - Columns: " << (sourceModel()->data(left.siblingAtColumn(2))).toString() << endl;

    /*
     * if (leftData.type() == QVariant::String)
    {
         //qDebug() << "LogViewSortFilterProxyModel::lessThan - QString" << endl;
    }
    */
    //qDebug() << "LogViewSortFilterProxyModel::lessThan - Left column: " << QString::number(left.column())<< endl;
    //qDebug() << "LogViewSortFilterProxyModel::lessThan - Right column: " << QString::number(right.column())<< endl;

    //qDebug() << "LogViewSortFilterProxyModel::lessThan - date: " << leftDT.toString("yyyy-MM-dd--hh:mm:ss") <<"<==>" << rightDT.toString("yyyy-MM-dd--hh:mm:ss")  << endl;
    if (left.column() == 1)
    {
        //qDebug() << "LogViewSortFilterProxyModel::lessThan - left: " << (sourceModel()->data(left.siblingAtColumn(1))).toString() << endl;
        //QDate leftD = QDate::fromString((sourceModel()->data(left.siblingAtColumn(1))).toString(), "yyyy/MM/dd");
        //QDate rightD = QDate::fromString((sourceModel()->data(right.siblingAtColumn(1))).toString(), "yyyy/MM/dd");

        if (leftDT.date() == rightDT.date())
        {
            //qDebug() << "LogViewSortFilterProxyModel::lessThan - SAME" << endl;

            if (leftDT.time() < rightDT.time())
            {
                //qDebug() << "LogViewSortFilterProxyModel::lessThan - SAME - (time) TRUE - Left" << endl;
            }
            else
            {
                //qDebug() << "LogViewSortFilterProxyModel::lessThan - SAME - (time) FALSE - Right" << endl;
            }
            return QString::localeAwareCompare(leftDT.toString("hh:mm:ss"), rightDT.toString("hh:mm:ss")) < 0;
            //return leftDT.time() < rightDT.time();

        }
        else
        {
            //qDebug() << "LogViewSortFilterProxyModel::lessThan - DIFFERENT: " << endl;
            //return leftD < rightD;
            if (leftDT.date() < rightDT.date())
            {
                //qDebug() << "LogViewSortFilterProxyModel::lessThan - DIFFERENT - (date) TRUE - Left" << endl;

            }
            else
            {
                //qDebug() << "LogViewSortFilterProxyModel::lessThan - DIFFERENT - (date) FALSE - Right" << endl;
            }
            //return leftDT.date() < rightDT.date();
            return QString::localeAwareCompare(leftString, rightString) < 0;
        }
    }
    else
    {
        //return leftData < rightData;
        //qDebug() << "LogViewSortFilterProxyModel::lessThan - No column 1: " << leftString << " / " << rightString << endl;
        return QString::localeAwareCompare(leftString, rightString) < 0;
    }



    /*
    if (rightData.type() == QVariant::DateTime) {
    //if (rightData.type() == QVariant::Time) {
        //qDebug() << "LogViewSortFilterProxyModel::lessThan - 1" << endl;
        return leftData.toDateTime() < rightData.toDateTime();

    }
    else
    {

        QString leftString = leftData.toString();
        QString rightString = rightData.toString();

        if (left.column() == 1)
        {

            //qDebug() << "LogViewSortFilterProxyModel::lessThan - left Column == 1" << endl;
            //qDebug() << "LogViewSortFilterProxyModel::lessThan - leftData/rightData: " << leftString<< "/" << rightString << endl;
            if (leftD == rightD)
            {
                //qDebug() << "LogViewSortFilterProxyModel::lessThan - leftData == rightData: " << leftString<< "/" << rightString << endl;
                QString leftN = (sourceModel()->data(left.siblingAtColumn(2))).toString();
                QString rightN = (sourceModel()->data(right.siblingAtColumn(2))).toString();

                QTime lT  = QTime::fromString(leftN, "hh:mm:ss");
                QTime rT  = QTime::fromString(rightN, "hh:mm:ss");

                return lT < rT;

            }
            else
            {
                //qDebug() << "LogViewSortFilterProxyModel::lessThan - left != right" << endl;
                //qDebug() << "LogViewSortFilterProxyModel::lessThan - leftData != rightData: " << leftString<< "/" << rightString << endl;
                return leftD < rightD;

            }
        }
        //qDebug() << "LogViewSortFilterProxyModel::lessThan - 2: " << leftString << "/" << rightString << endl;
        return QDate::fromString(leftString, "yyyy/MM/dd") < QDate::fromString(rightString, "yyyy/MM/dd");
        //return QString::localeAwareCompare(leftString, rightString) < 0;

    }
    */
}


