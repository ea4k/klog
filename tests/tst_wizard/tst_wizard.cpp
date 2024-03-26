/***************************************************************************
                          tst_wizard.h  -  description
                             -------------------
    begin                : Mar 2024
    copyright            : (C) 2024 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of Testing suite of KLog.                               *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify           *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    The testing suite of KLog is distributed in the hope that it will      *
 *    be useful, but WITHOUT ANY WARRANTY; without even the implied          *
 *    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.       *
 *    See the GNU General Public License for more details.                   *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with Testing suite of KLog.                                      *
 *    If not, see <https://www.gnu.org/licenses/>.                           *
 *                                                                           *
 *****************************************************************************/

#include <QtTest>
#include <QtWidgets>

#include "../../src/startwizard.h"

class tst_Wizard : public QObject
{
    Q_OBJECT

public:
    tst_Wizard();
    ~tst_Wizard();

private slots:
    void initTestCase();        // Executed before any test
    void test_Constructor();
    void cleanupTestCase(); // Executed after the last test


private:

};

tst_Wizard::tst_Wizard(){}

tst_Wizard::~tst_Wizard(){}

void tst_Wizard::initTestCase()
{
    qDebug() << Q_FUNC_INFO;
    Utilities util(Q_FUNC_INFO);
    QFile file(util.getKLogDBFile());
    if (file.exists())
        if (file.rename(util.getKLogDBFile() + "-test"))
            qDebug() << Q_FUNC_INFO << "DB deleted";
    qDebug() << Q_FUNC_INFO << ": " << util.getCfgFile();
    QFile sfile(util.getCfgFile());
    if (sfile.exists())
        if (sfile.rename(util.getCfgFile() +  "-test"))
            qDebug() << Q_FUNC_INFO << "Restoring the config file";
}

void tst_Wizard::cleanupTestCase()
{
    qDebug() << Q_FUNC_INFO;
    Utilities util(Q_FUNC_INFO);
    QFile file(util.getKLogDBFile() + "-test");
    if (file.exists())
        if (file.rename(util.getKLogDBFile()))
            qDebug() << Q_FUNC_INFO << "DB deleted";
    QFile sfile(util.getCfgFile() +  "-test");
    if (sfile.exists())
        if (sfile.rename(util.getCfgFile()))
            qDebug() << Q_FUNC_INFO << "Restoring the config file";


}

void tst_Wizard::test_Constructor()
{
    qDebug() << Q_FUNC_INFO << " - 10";
    Utilities util(Q_FUNC_INFO);
    qDebug() << Q_FUNC_INFO << " - 11";
    StartWizard wizard(util.getHomeDir(), "test");
    qDebug() << Q_FUNC_INFO << " - 12";
    wizard.setModal(false);
    qDebug() << Q_FUNC_INFO << " - 13";
    wizard.show();
    qDebug() << Q_FUNC_INFO << " - 14";
    QCOMPARE( wizard.button(QWizard::NextButton)->isEnabled(), true);
    qDebug() << Q_FUNC_INFO << " - 15";
    QCOMPARE( wizard.button(QWizard::NextButton)->isVisible(), true);

    QCOMPARE( wizard.currentPage()->title(), "Welcome to KLog!");
    qDebug() << Q_FUNC_INFO << " - 20";
    qDebug() << Q_FUNC_INFO << " - " << wizard.currentPage()->title();
    //QTest::mouseClick(wizard.button(QWizard::NextButton), Qt::LeftButton, Qt::NoModifier, QPoint(), 5000);
    //QTest::mouseClick(wizard.button(QWizard::NextButton), Qt::LeftButton);
    QTest::mouseClick(wizard.button(QWizard::NextButton), Qt::LeftButton, Qt::NoModifier, QPoint(), 5000);
    qDebug() << Q_FUNC_INFO << " - 21";
    QCOMPARE( wizard.currentPage()->title(), "KLog License information");
    qDebug() << Q_FUNC_INFO << " - " << wizard.currentPage()->title();
    //qDebug() << Q_FUNC_INFO << " - " << (wizard.buttonText(QWizard::CancelButton));
    QCOMPARE( wizard.currentPage()->isComplete(), false); // the user has to accept the license first
    QCOMPARE( wizard.button(QWizard::NextButton)->isEnabled(), false);
    //QWizardPage *licPage = wizard.currentPage();

    //QTest::mouseClick(wizard.currentPage()->aceptLicCheckBox , Qt::LeftButton, Qt::NoModifier, QPoint(), 5000);

    //QTest::mouseClick(&wizard, Qt::LeftButton, Qt::NoModifier, QPoint(), 5000);
    //QTest::keyClick(wizard.button(QWizard::NextButton), Qt::Key_Escape);
    qDebug() << Q_FUNC_INFO << " - 30";


}


QTEST_MAIN(tst_Wizard)

#include "tst_wizard.moc"
