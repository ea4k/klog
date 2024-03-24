/***************************************************************************
                          showerrordialog.cpp  -  description
                             -------------------
    begin                : oct 2017
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

#include "showerrordialog.h"

ShowErrorDialog::ShowErrorDialog()
{
       //qDebug() << "ShowErrorDialog::ShowErrorDialog" ;
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
    //connect(buttonBox, SIGNAL(rejected()), this, &QDialog::reject);

    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject())) ;
    txtLabel->setFrameShadow(QFrame::Raised);
    txtLabel->setFrameStyle(QFrame::StyledPanel);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(closeButton);

    //QVBoxLayout *mainLayout = new QVBoxLayout;
    layout->addWidget(txtLabel);
    layout->addLayout(buttonsLayout);

    setLayout(layout);
       //qDebug() << "ShowErrorDialog::ShowErrorDialog - END" ;
}

void ShowErrorDialog::setText(const QString txt)
{
      //qDebug() << "ShowErrorDialog::setVersion: " << txt;
    text = txt;
    txtLabel->setText(txt);
    //textBrowser->setHtml(text);
}

ShowErrorDialog::~ShowErrorDialog()
{
       //qDebug() << "ShowErrorDialog::~ShowErrorDialog" ;
}

void ShowErrorDialog::slotAcceptButtonClicked()
{
       //qDebug() << "ShowErrorDialog::slotAcceptButtonClicked" ;
    accept();
}

void ShowErrorDialog::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        break;

    default:
        //QFrame::keyPressEvent(event)
        slotAcceptButtonClicked();
    }
}
