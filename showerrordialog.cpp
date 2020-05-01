#include "showerrordialog.h"

ShowErrorDialog::ShowErrorDialog()
{
     //qDebug() << "ShowErrorDialog::ShowErrorDialog"  << endl;
    text.clear();


    setWindowTitle(tr("KLog Message"));
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    txtLabel = new QLabel(text);
//    QLabel *txtLabel = new QLabel(text);
    txtLabel->setWordWrap(true);
    txtLabel->setOpenExternalLinks(true);
    txtLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    QPushButton *closeButton = buttonBox->button(QDialogButtonBox::Close);

    buttonBox->addButton(closeButton, QDialogButtonBox::ButtonRole(QDialogButtonBox::RejectRole | QDialogButtonBox::AcceptRole));
    connect(buttonBox , &QDialogButtonBox::rejected, this, &QDialog::reject);


    txtLabel->setFrameShadow(QFrame::Raised);
    txtLabel->setFrameStyle(QFrame::StyledPanel);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(closeButton);

    //QVBoxLayout *mainLayout = new QVBoxLayout;
    layout->addWidget(txtLabel);
    layout->addLayout(buttonsLayout);

    setLayout(layout);
     //qDebug() << "ShowErrorDialog::ShowErrorDialog - END"  << endl;
}

void ShowErrorDialog::setText(const QString txt)
{
    //qDebug() << "ShowErrorDialog::setVersion: " << txt << endl;

    text = txt;
    txtLabel->setText(txt);
    //textBrowser->setHtml(text);

}

ShowErrorDialog::~ShowErrorDialog()
{
     //qDebug() << "ShowErrorDialog::~ShowErrorDialog"  << endl;
}

void ShowErrorDialog::slotAcceptButtonClicked()
{
     //qDebug() << "ShowErrorDialog::slotAcceptButtonClicked"  << endl;
    accept();
}



void ShowErrorDialog::keyPressEvent(QKeyEvent *event){

    switch (event->key()) {

        break;

    default:
        //QFrame::keyPressEvent(event)
        slotAcceptButtonClicked();

    }
}
