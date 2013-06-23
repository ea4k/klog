#include "helpaboutdialog.h"


HelpAboutDialog::HelpAboutDialog()
{

    QString text;
    //QString version = _version;
    text = "You can find the last version on <a href=\"http://jaime.robles.es/klog\">http://jaime.robles.es/klog</a>\n2002 - 2013 - Jaime Robles, EA4TV, jaime@robles.es\n2009 - 2010 - Andrew Goldie, ZL2ACG, andrew.goldie@rocketmail.com";
    textBrowser = new QTextBrowser;
    textBrowser->setFrameShadow (QFrame::Raised);
    textBrowser->setFrameShape (QFrame::StyledPanel);

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

    setWindowTitle("About Dialog");
    ///

    connect(acceptButton, SIGNAL(clicked()), this, SLOT(slotAcceptButtonClicked()));
}

HelpAboutDialog::~HelpAboutDialog()
{
}

void HelpAboutDialog::slotAcceptButtonClicked()
{

}


