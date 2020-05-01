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
}
/*
void LogViewSortFilterProxyModel::setFilterMinimumDate(const QDate &date)
{
    minDate = date;
    invalidateFilter();
}

void LogViewSortFilterProxyModel::setFilterMaximumDate(const QDate &date)
{
    maxDate = date;
    invalidateFilter();
}

bool LogViewSortFilterProxyModel::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex index1 = sourceModel()->index(sourceRow, 1, sourceParent);
    QModelIndex index2 = sourceModel()->index(sourceRow, 2, sourceParent);

    return (sourceModel()->data(index0).toString().contains(filterRegExp())
            || sourceModel()->data(index1).toString().contains(filterRegExp()))
            && dateInRange(sourceModel()->data(index2).toDate());
}
*/

bool LogViewSortFilterProxyModel::lessThan(const QModelIndex &left,
                                      const QModelIndex &right) const
{
    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);

    //qDebug() << "LogViewSortFilterProxyModel::lessThan - Columns: " << (sourceModel()->data(left.siblingAtColumn(2))).toString() << endl;

    if (rightData.type() == QVariant::DateTime) {
    //if (rightData.type() == QVariant::Time) {
        //qDebug() << "LogViewSortFilterProxyModel::lessThan - 1" << endl;
        return leftData.toDateTime() < rightData.toDateTime();

    } else {

        QString leftString = leftData.toString();
        QString rightString = rightData.toString();

        if (left.column() == 1)
        {
            //qDebug() << "LogViewSortFilterProxyModel::lessThan - left Column == 1" << endl;
            if (leftData == rightData)
            {
                QString leftN = (sourceModel()->data(left.siblingAtColumn(2))).toString();
                QString rightN = (sourceModel()->data(right.siblingAtColumn(2))).toString();

                QTime lT  = QTime::fromString(leftN, "hh:mm:ss");
                QTime rT  = QTime::fromString(rightN, "hh:mm:ss");

                return lT < rT;

            }
            else
            {
                //qDebug() << "LogViewSortFilterProxyModel::lessThan - left != right" << endl;
            }
        }
        //qDebug() << "LogViewSortFilterProxyModel::lessThan - 2: " << leftString << "/" << rightString << endl;
        /*
        static QRegExp emailPattern("[\\w\\.]*@[\\w\\.]*)");


        if(left.column() == 1 && emailPattern.indexIn(leftString) != -1)
            leftString = emailPattern.cap(1);


        if(right.column() == 1 && emailPattern.indexIn(rightString) != -1)
            rightString = emailPattern.cap(1);
        */
        return QString::localeAwareCompare(leftString, rightString) < 0;

    }
}


