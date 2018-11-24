#include "softwareupdatedialog.h"

//#include <QDebug>


SoftwareUpdateDialog::SoftwareUpdateDialog()
{
     //qDebug() << "SoftwareUpdateDialog::SoftwareUpdateDialog"  << endl;

    textBrowser = new QTextBrowser;
    textBrowser->setOpenLinks(true);
    textBrowser->setOpenExternalLinks(true);

    //textBrowser->setFrameShadow(QFrame::Raised);
    //textBrowser->setFrameStyle(QFrame::StyledPanel);

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

    setWindowTitle(tr("KLog update"));

    ///

    connect(acceptButton, SIGNAL(clicked()), this, SLOT(slotAcceptButtonClicked()));
     //qDebug() << "SoftwareUpdateDialog::SoftwareUpdateDialog - END"  << endl;
}

void SoftwareUpdateDialog::setVersion(const QString tversion, const bool updateNeeded)
{
    //qDebug() << "SoftwareUpdateDialog::setVersion: " << tversion << endl;
    _version = tversion;
    if (updateNeeded)
    {
        text = "<center><h2>KLog new version ("+ tversion + ") is available! </h2></center><br>There is a new version of KLog available.<br><br><b>You can get the new version from:<br><br><center><a href=http://www.klog.xyz>http://www.klog.xyz</a></center>";
    }
    else
    {
        text = "<center><h2>" + tr("Congratulations!") + "</h2></center><br><br>" + tr("Your KLog has been updated.") + "<br><br>" + tr("You already have the latest version.") + "<br><center>("+ tversion + ")</center>";
    }

    textBrowser->setHtml(text);

}

SoftwareUpdateDialog::~SoftwareUpdateDialog()
{
     //qDebug() << "SoftwareUpdateDialog::~SoftwareUpdateDialog"  << endl;
}

void SoftwareUpdateDialog::slotAcceptButtonClicked()
{
     //qDebug() << "SoftwareUpdateDialog::slotAcceptButtonClicked"  << endl;
    accept();
}



void SoftwareUpdateDialog::keyPressEvent(QKeyEvent *event){

    switch (event->key()) {

        break;

    default:
        //QFrame::keyPressEvent(event)
        slotAcceptButtonClicked();

    }
}
