
/***************************************************************************
                          aboutdialog.cpp  -  description
                             -------------------
    begin                : feb 2017
    copyright            : (C) 2017 by Jaime Robles
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

#include "aboutdialog.h"
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QDebug>

AboutDialog::AboutDialog(const QString &tversion, QWidget *parent)
    : QDialog(parent)
{
      //qDebug() << "AboutDialog::AboutDialog" << QT_ENDL;

    QPixmap pixmap(":/img/klog_256x256.png");


    setWindowTitle(tr("About KLog"));
    setWindowFlags(windowFlags() & Qt::WindowContextHelpButtonHint);
    QGridLayout *layout = new QGridLayout(this);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    const QString br = QLatin1String("<br/>");

    const QString description = "<center><h2>KLog " + tversion + "</h2><h4> " +tr("By") +
            " <a href=\"https://www.qrz.com/db/ea4k\">EA4K</a> - 2002-2021</h4></center><br>" +
            tr("KLog is a free logbook for hamradio operators.") +"<br><br><b>" +
            tr("Please be aware that this is a development release and it may contain many bugs.<br>Backup your data before using this software!") +
            "</b><br><br>" +
            tr("Since 0.6.2, KLog has been fully rewritten to provide a cross-platform application that runs on all major operating systems (GNU/Linux, macOS and Windows) and to support new functionality.") +
            "<br><br>" +
            tr("Please provide your review in KLog's eHam review page:") +
            "<bR>" +
            "<a href=https://www.eham.net/reviews/detail/3118>https://www.eham.net/reviews/detail/3118</a><br><br>" +
            tr("Find more information and the latest release at") + "<br><a href=https://www.klog.xyz>https://www.klog.xyz</a><br><br>" +
            tr("Author") + ": <a href=https://jaime.robles.es/klog>Jaime Robles</a>, <a href=\"https://www.qrz.com/db/ea4k\">EA4K</a><br><a href=mailto:jaime@robles.es>jaime@robles.es</a>";


    QLabel *copyRightLabel = new QLabel(description);
    copyRightLabel->setWordWrap(true);
    copyRightLabel->setOpenExternalLinks(true);
    copyRightLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    QPushButton *closeButton = buttonBox->button(QDialogButtonBox::Close);

    buttonBox->addButton(closeButton, QDialogButtonBox::ButtonRole(QDialogButtonBox::RejectRole | QDialogButtonBox::AcceptRole));
    connect(buttonBox , &QDialogButtonBox::rejected, this, &QDialog::reject);

    QLabel *logoLabel = new QLabel;
    logoLabel->setPixmap(pixmap);
    QLabel *logoLabel2 = new QLabel;
    logoLabel2->setPixmap(pixmap);
    QLabel *logoLabel3 = new QLabel;
    logoLabel3->setPixmap(pixmap);
    QLabel *logoLabel4 = new QLabel;
    logoLabel4->setPixmap(pixmap);

    QString author1 = QString("<tr><td>Jaime Robles</td>") + QString("<td><a href=\"https://www.qrz.com/db/ea4k\">EA4K</a></td>") + "<td>(2002-" + tr("today") +") " +tr("Main developer") + "</td></tr>";
    QString author2 = QString("<tr><td>Juan Carlos Reig</td>")  + QString("<td><a href=\"https://www.qrz.com/db/ea5wa\">EA5WA</a></td>") + "<td>(2021-" + tr("today") + ")</td></tr>";
    QString author3 = QString("<tr><td>Ladislav Foldyna</td>")  + QString("<td><a href=\"https://www.qrz.com/db/ok1mlg\">OK1MLG</a></td>") + "<td>(2021-" + tr("today") + ")</td></tr>";
    QString author4 = QString("<tr><td>Akihiro Koda</td>")  + QString("<td><a href=\"https://www.qrz.com/db/jl3oxr\">JL3OXR</a></td>") + "<td>(2016-2017)</td></tr>";
    QString author5 = QString("<tr><td>Andrew Goldie</td>") + QString("<td><a href=\"https://www.qrz.com/db/zl2agc\">ZL2ACG</a></td>") + "<td>(2009-2010)</td></tr>";

    QString authorText = tr("KLog is developed by a very small team and you are invited to join!") + "<br><br>" + tr("If you want to provide support you are welcome to join the <a href=\"https://groups.io/g/klog\">KLog mailing list</a>!") + "<br><br>" + tr("You can also help us by sending bug reports or small code contributions, ideas or whatever you think may improve KLog.");
    QString authors = "<center><h2>" + tr("Authors") + "</h2></center><br>" + authorText + "<br><table>" + author1 + author2 + author3 + author4 + author5 + "</table>";

    QString translator1 = QString("<tr><td>Catalan</td>")  + QString("<td>Josep Ma. Ferrer</td>") + QString("<td><a href=\"mailto://kde-i18n-ca@kde.org\">KDE Catalan translation team</a></td></tr>");
    QString translator2 = QString("<tr><td>Croatian</td>")  + QString("<td>Kristijan Conkas</td>") + QString("<td><a href=\"https://www.qrz.com/db/m0nkc\">M0NKC</a></td></tr>");
    QString translator3 = QString("<tr><td>Czech</td>")  + QString("<td>Ladislav Foldyna</td>") + QString("<td><a href=\"https://www.qrz.com/db/ok1mlg\">OK1MLG</a></td></tr>");
    QString translator4 = QString("<tr><td>Finnish</td>")  + QString("<td>Kristjan Lorents</td>") + QString("<td><a href=\"debian-i18n@lists.debian.org\">Finnish Debian translation team</a></td></tr>");
    QString translator5 = QString("<tr><td>French</td>") + QString("<td>Christophe Lefebvre</td>") + QString("<td><a href=\"https://www.qrz.com/db/f4hwl\">F4HWL</a></td></tr>");
    QString translator6 = QString("<tr><td>Danish</td>")  + QString("<td>Joe Hansen</td>") + QString("<td><a href=\"mailto://debian-l10n-danish@list.debian.org\">Danish Debian translation team</a></td></tr>");
    QString translator7 = QString("<tr><td>German</td>") + QString("<td>Burhard Lück</td>") + QString("<td><a href=\"mailto://kde-i18n-de@kde.org\">KDE German translation team</a></td></tr>");
    QString translator8 = QString("<tr><td>Italian</td>")  + QString("<td>Simona Pisano</td>") + QString("<td><a href=\"https://www.qrz.com/db/iu5hiu\">IU5HIU</a></td></tr>");
    QString translator9 = QString("<tr><td>Japanese</td>")  + QString("<td>Akihiro Koda</td>") + QString("<td><a href=\"https://www.qrz.com/db/jl3oxr\">JL3OXR</a></td></tr>");
    QString translator10 = QString("<tr><td>Polish</td>")  + QString("<td>Piotr Ludwig</td>") + QString("<td><a href=\"https://www.qrz.com/db/la7rra\">LA7RRA</a></td></tr>");
    QString translator11 = QString("<tr><td>Spanish</td>") + QString("<td>Jaime Robles</td>") + QString("<td><a href=\"https://www.qrz.com/db/ea4k\">EA4K</a></td></tr>");


    QString translatorsText = tr("Translators bring KLog into your language. They are really an important part of the KLog development team.") + "<br><br>" + tr("If KLog is still not in your language and you want to help us, you are welcome to contact us through the <a href=\"https://groups.io/g/klog\">KLog mailing list</a>!");
    QString translators = "<center><h2>" + tr("Translators") + "</h2></center><br>" + translatorsText + "<br><table>" + translator1 + translator2 + translator3
                                                                                                                      + translator4 + translator5 + translator6
                                                                                                                      + translator7 + translator8 + translator9
                                                                                                                      + translator10 + translator11 + "</table>";
    QLabel *authorsLabel = new QLabel(authors);
    authorsLabel->setWordWrap(true);
    authorsLabel->setOpenExternalLinks(true);
    authorsLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QLabel *translatorsLabel = new QLabel(translators);
    translatorsLabel->setWordWrap(true);
    translatorsLabel->setOpenExternalLinks(true);
    translatorsLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QString privacy = "<center><h2>" + tr("Privacy advisory") + "</h2></h4></center><br>" +
            tr("KLog developers have included a feature that reports some user data to the KLog server with the sole purpose of identifying the number of installed versions, to focus development in one direction or another taking into account users' needs.") +
            "<br><br>" + tr("At present, the data that is provided is the following:") +
            "<ul><li>" + tr("Callsign") + "</li><li>" + tr("KLog version") + "</li><li>" + tr("Operating system") + "</li></ul><br><br><b>" +
            tr("Be aware that you can enable/disable this feature from the Misc tab in the Setup page.") + "</b>";

    QLabel *privacyLabel = new QLabel(privacy);
    privacyLabel->setAlignment(Qt::AlignJustify);
    privacyLabel->setWordWrap(true);
    privacyLabel->setOpenExternalLinks(true);
    privacyLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    tab1 = new QWidget;
    tab2 = new QWidget;
    tab3 = new QWidget;
    tab4 = new QWidget;


    QGridLayout *layout1 = new QGridLayout;
    layout1->addWidget(logoLabel , 0, 0, 1, 1);
    layout1->addWidget(copyRightLabel, 0, 1, 4, 4);
    tab1->setLayout(layout1);


    QGridLayout *layout2 = new QGridLayout;
    layout2->addWidget(logoLabel2 , 0, 0, 1, 1);
    layout2->addWidget(authorsLabel, 0, 1, 4, 4);
    tab2->setLayout(layout2);


    QGridLayout *layout3 = new QGridLayout;
    layout3->addWidget(logoLabel3 , 0, 0, 1, 1);
    layout3->addWidget(translatorsLabel, 0, 1, 4, 4);
    tab3->setLayout(layout3);

    QGridLayout *layout4 = new QGridLayout;
    layout4->addWidget(logoLabel4 , 0, 0, 1, 1);
    layout4->addWidget(privacyLabel, 0, 1, 4, 4);
    tab4->setLayout(layout4);

    tabw = new QTabWidget;

    tabw->addTab(tab1, tr("KLog"));
    tabw->addTab(tab2, tr("Authors"));
    tabw->addTab(tab3, tr("Translators"));
    tabw->addTab(tab4, tr("Privacy"));

    layout->addWidget(tabw);
    layout->addWidget(buttonBox, 1, 0, Qt::AlignRight);

    setLayout(layout);


       //qDebug() << "AboutDialog::AboutDialog - END" << QT_ENDL;

}
AboutDialog::~AboutDialog()
{

}

bool AboutDialog::event(QEvent *event)
{
    if (event->type() == QEvent::ShortcutOverride) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Escape && !ke->modifiers()) {
            ke->accept();
            return true;
        }
    }
    return QDialog::event(event);
}

