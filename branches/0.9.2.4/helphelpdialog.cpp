#include "helphelpdialog.h"


HelpHelpDialog::HelpHelpDialog(const QString tversion)
{


     //http://www.gnu.org/licenses/gpl.txt
    QString text;
    QString _version = tversion;

  text = "<center><h2>KLog " + _version + tr("</h2><h4>By EA4TV - 2002-2016</h4></center><br>KLog is a free logging software for hamradio operators.<br><br><b>Please know that this is an BETA release and it may contain many bugs.<br>Backup your data before using this software!</b><br><br>KLog has been fully rewritten from the 0.6.2 to be able to provide a cross-platform application that runs in the main operating systems (Linux, OSX & Windows) and provide new functionalities that KLog was not providing. <br><br>Find more information and latest release at<br><a href=http://jaime.robles.es/klog>http://jaime.robles.es/klog</a><br><br>Author: Jaime Robles, EA4TV<br><a href=mailto:jaime@robles.es>jaime@robles.es</a>");


    textBrowser = new QTextBrowser;
    textBrowser->setOpenLinks(true);
    textBrowser->setOpenExternalLinks(true);
    textBrowser->setHtml(text);

    QPushButton *acceptButton = new QPushButton(tr("Ok"));


    textBrowser->setOpenExternalLinks(true);
    //textBrowser->setHTML(url);


    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(acceptButton);


    ///
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(textBrowser);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);


    setWindowTitle(tr("Help"));
    ///

    connect(acceptButton, SIGNAL(clicked()), this, SLOT(slotAcceptButtonClicked()));
}

HelpHelpDialog::~HelpHelpDialog()
{
}

void HelpHelpDialog::slotAcceptButtonClicked()
{
    accept();
}


