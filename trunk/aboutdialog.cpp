#include "aboutdialog.h"
//#include <QDebug>
//#include "coreicons.h"

//#include <app/app_version.h>
//#include <coreplugin/coreicons.h>
//#include <coreplugin/icore.h>
//#include <utils/algorithm.h>
//#include <utils/hostosinfo.h>
//#include <utils/qtcassert.h>
//#include <utils/utilsicons.h>

#include <QDialogButtonBox>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>

AboutDialog::AboutDialog(const QString tversion, QWidget *parent)
    : QDialog(parent)
{
   //qDebug() << "AboutDialog::AboutDialog" << endl;

    QPixmap pixmap(":/img/klog_256x256.png");


    setWindowTitle(tr("About KLog"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QGridLayout *layout = new QGridLayout(this);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    const QString br = QLatin1String("<br/>");

    const QString description = "<center><h2>KLog " + tversion + "</h2><h4> " +tr("By") +
            " <a href=\"https://www.qrz.com/db/ea4tv\">EA4TV</a> - 2002-2017</h4></center><br>" +
            tr("KLog is a free logging software for hamradio operators.") +"<br><br><b>" +
            tr("Please know that this is an BETA release and it may contain many bugs.<br>Backup your data before using this software!") +
            "</b><br><br>" +
            tr("KLog has been fully rewritten from the 0.6.2 to be able to provide a cross-platform application that runs in the main operating systems (Linux, macOS & Windows) and provide new functionalities that KLog was not providing.") +
            "<br><br>" +
            tr("Please provide your review in KLog's eHam review page:") +
            "<bR>" +
            "<a href=http://www.eham.net/reviews/detail/3118>http://www.eham.net/reviews/detail/3118</a><br><br>" +
            tr("Find more information and latest release at<br>") + "<a href=http://www.klog.xyz>http://www.klog.xyz</a><br><br>" +
            tr("Author") + ": <a href=http://jaime.robles.es/klog>Jaime Robles</a>, <a href=\"https://www.qrz.com/db/ea4tv\">EA4TV</a><br><a href=mailto:jaime@robles.es>jaime@robles.es</a>";


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

    QString author1 = QString("<tr><td>Jaime Robles</td>") + QString("<td><a href=\"https://www.qrz.com/db/ea4tv\">EA4TV</a></td>") + "<td>(2002-" + tr("today") +")" +tr("Main developer") + "</td></tr>";
    QString author2 = QString("<tr><td>Akihiro Koda</td>")  + QString("<td><a href=\"https://www.qrz.com/db/jl3oxr\">JL3OXR</a></td>") + tr("<td>(2016)</td></tr>");
    QString author3 = QString("<tr><td>Andrew Goldie</td>") + QString("<td><a href=\"https://www.qrz.com/db/zl2agc\">ZL2ACG</a></td>") + tr("<td>(2009-2010)</td></tr>");

    QString authorText = tr("KLog is developed by a very small team and you are invited to join!") + "<br><br>" + tr("If you want to support you can join the <a href=\"https://lists.nongnu.org/mailman/listinfo/klog-devel\">KLog's development mailing list</a> and you will be welcome!") + "<br><br>" + tr("You can also help us sending bug reports or small code contributions, ideas or whatever you think it may improve KLog.");
    QString authors = "<center><h2>" + tr("Authors") + "</h2></center><br>" + authorText + "<br><table>" + author1 + author2 + author3 + "</table>";


    QString translator1 = QString("<tr><td>Catalan</td>")  + QString("<td>Luis</td>") + QString("<td><a href=\"https://www.qrz.com/db/ea3nm\">EA3NM</a></td></tr>");
    QString translator2 = QString("<tr><td>Croatian</td>")  + QString("<td>Kristijan Conkas</td>") + QString("<td><a href=\"https://www.qrz.com/db/m0nkc\">M0NKC</a></td></tr>");
    QString translator3 = QString("<tr><td>Japanese</td>")  + QString("<td>Akihiro Koda</td>") + QString("<td><a href=\"https://www.qrz.com/db/jl3oxr\">JL3OXR</a></td></tr>");
    QString translator4 = QString("<tr><td>Spanish</td>") + QString("<td>Jaime Robles</td>") + QString("<td><a href=\"https://www.qrz.com/db/ea4tv\">EA4TV</a></td></tr>");


    QString translatorsText = tr("Translators bring KLog into your language. They are really an important part of the KLog development team.") + "<br><br>" + tr("If KLog is still not in your language and you want to help us, please contact us through the <a href=\"https://lists.nongnu.org/mailman/listinfo/klog-devel\">KLog's development mailing list</a> and you will be welcome!");
    QString translators = "<center><h2>" + tr("Translators") + "</h2></center><br>" + translatorsText + "<br><table>" + translator1 + translator2 + translator3 + translator4 + "</table>";

    QLabel *authorsLabel = new QLabel(authors);
    authorsLabel->setWordWrap(true);
    authorsLabel->setOpenExternalLinks(true);
    authorsLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QLabel *translatorsLabel = new QLabel(translators);
    translatorsLabel->setWordWrap(true);
    translatorsLabel->setOpenExternalLinks(true);
    translatorsLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QString privacy = "<center><h2>" + tr("Privacy advisory") + "</h2></h4></center><br>" +
            tr("KLog developers have included a feature that reports to KLog's server some data of the user with the only purpose to identify the number of installed versions to focus the development in one or other direction taking into account the user's need.") +
            "<br><br>" + tr("Today, the data that is provided is the following:") +
            "<ul><li>" + tr("Callsign") + "</li><li>" + tr("KLog version") + "</li><li>" + tr("Operating system") + "</li></ul><br><br><b>" +
            tr("Please know that you can enable/disable this feature from the Misc tab in the Setup page") + ".</b>";

    QLabel *privacyLabel = new QLabel(privacy);
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
    layout1->addWidget(buttonBox, 4, 0, 1, 5);

    tab1->setLayout(layout1);


    QGridLayout *layout2 = new QGridLayout;
    layout2->addWidget(logoLabel2 , 0, 0, 1, 1);
    layout2->addWidget(authorsLabel, 0, 1, 4, 4);
    //layout2->addWidget(buttonBox, 4, 0, 1, 5);
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

    setLayout(layout);


    //qDebug() << "AboutDialog::AboutDialog - END" << endl;

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

