/***************************************************************************
                          setupentitydialog.cpp  -  description
                             -------------------
    begin                : sept 2012
    copyright            : (C) 2012 by Jaime Robles
    email                : jaime@robles.es
 ***************************************************************************/

/*****************************************************************************
 * This file is part of KLog.                                             *
 *                                                                           *
 *    KLog is free software: you can redistribute it and/or modify        *
 *    it under the terms of the GNU General Public License as published by   *
 *    the Free Software Foundation, either version 3 of the License, or      *
 *    (at your option) any later version.                                    *
 *                                                                           *
 *    KLog is distributed in the hope that it will be useful,             *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *    GNU General Public License for more details.                           *
 *                                                                           *
 *    You should have received a copy of the GNU General Public License      *
 *    along with KLog.  If not, see <https://www.gnu.org/licenses/>.       *
 *                                                                           *
 *****************************************************************************/

#include "setuppages/setupentitydialog.h"
//#include <QDebug>
/*
This class calls all the othet "Setup..." to manage the configuration

*/

SetupEntityDialog::SetupEntityDialog(){


       //qDebug() << "SetupEntityDialog::SetupEntityDialog: " << Qt::endl;

    //QPalette::ColorRole QWidget::foregroundRole () const;
    //QPalette::ColorRole

    //paletteOrig = new QPalette;
    //paletteWrong = new QPalette;
    //paletteWrong.setColor(QPalette::Normal, QPalette::WindowText, Qt::red);
    //paletteWrong = new QPalette();
    //*paletteWrong->setColor(QPalette::WindowText, Qt::red);

    //paletteWrong.setColor(QPalette::WindowText, color.setNamedColor("red"));
    palw.setColor(QPalette::Text, Qt::red);
    //pal.setColor(QPalette::Text, Qt::black);


    entityData.clear();

    entityBool = false;
    mainPrefixBool = false;
    cqBool = false;
    ituBool = false;
    contBool = false;
    latBool = false;
    lonBool = false;
    utcBool = false;
    arrlidBool = false;
    delBool = false;
    delDateBool = false;
    prefBool = false;
   //qDebug() << "SetupEntityDialog::SetupEntityDialog -  0" << Qt::endl;

    QLabel *entityLabel = new QLabel(tr("Entity"));
    entityLineEdit  = new QLineEdit;
    entityLineEdit->setToolTip(tr("Name of the Entity."));

    QLabel *cqLabel = new QLabel(tr("CQ"));
    cqLineEdit  = new QLineEdit;
    cqLineEdit->setToolTip(tr("CQ zone."));

    QLabel *ituLabel = new QLabel(tr("ITU"));
    ituLineEdit = new QLineEdit;
    ituLineEdit->setToolTip(tr("ITU zone."));

    //QLabel *contLabel = new QLabel(tr("Continent"));
    //contLineEdit = new QLineEdit;
    //contLineEdit->setToolTip(tr("Continent of the Entity"));

    QLabel *latLabel = new QLabel(tr("Latitude"));
    latLineEdit  = new QLineEdit;
    latLineEdit->setToolTip(tr("Longitude of the Entity."));

    QLabel *lonLabel = new QLabel(tr("Longitude"));
    lonLineEdit  = new QLineEdit;
    lonLineEdit->setToolTip(tr("Longitude of the Entity."));

    QLabel *utcLabel = new QLabel(tr("UTC"));
    utcLineEdit  = new QLineEdit;
    utcLineEdit->setToolTip(tr("Local time difference to UTC."));

    QLabel *mprefLabel = new QLabel(tr("Main prefix"));
    mprefLineEdit = new QLineEdit;
    mprefLineEdit->setToolTip(tr("Main prefix of the entity."));

       //qDebug() << "SetupEntityDialog::SetupEntityDialog -  1" << Qt::endl;

    QLabel *arrlidLabel = new QLabel(tr("ARRL ID"));
    arrlidLineEdit = new QLineEdit;
    arrlidLineEdit->setToolTip(tr("ARRL ID."));

    //QLabel *deletedLabel = new QLabel(tr("Deleted"));
    //deletedLineEdit = new QLineEdit;
    //deletedLineEdit->setToolTip(tr("Mark if the entity is deleted"));

    QLabel *prefLabel = new QLabel(tr("Prefixes"));
    prefLineEdit  = new QLineEdit;
    prefLineEdit->setToolTip(tr("Comma separated possible prefixes, e.g. EA1, EA2, ..."));

    delQDateEdit = new QDateEdit;
    delQDateEdit->setToolTip(tr("Date of the deletion."));

    delRbutton = new QCheckBox(tr("Deleted"), this);

    QPushButton *closeButton = new QPushButton(tr("Cancel"));
    QPushButton *okButton = new QPushButton(tr("Ok"));

       //qDebug() << "SetupEntityDialog::SetupEntityDialog -  2" << Qt::endl;
/*
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(slotOkButtonClicked()));

    connect(entityLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotCheckEntity() ) );
    connect(mprefLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotCheckMainprefix() ) );
    connect(cqLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotCheckCQz() ) );
    connect(ituLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotCheckITUz() ) );
    connect(contLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotCheckContinent() ) );
    connect(latLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotCheckLatitude() ) );
    connect(lonLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotCheckLongitude() ) );
    connect(utcLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotCheckUTC() ) );
    connect(arrlidLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotCheckARRLid() ) );
    connect(delRbutton, SIGNAL(checked), this, SLOT(slotCheckDeleted() ) );
    connect(delQDateEdit, SIGNAL(dateChanged), this, SLOT(slotCheckDeletedDate() ) );
    connect(prefLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotCheckPrefixes() ) );
*/
   //qDebug() << "SetupEntityDialog::SetupEntityDialog -  3" << Qt::endl;

    QVBoxLayout *cqLayout = new QVBoxLayout;
    cqLayout->addWidget(cqLabel);
    cqLayout->addWidget(cqLineEdit);

    QVBoxLayout *ituLayout = new QVBoxLayout;
    ituLayout->addWidget(ituLabel);
    ituLayout->addWidget(ituLineEdit);

    QVBoxLayout *latLayout = new QVBoxLayout;
    latLayout->addWidget(latLabel);
    latLayout->addWidget(latLineEdit);

    QVBoxLayout *lonLayout = new QVBoxLayout;
    lonLayout->addWidget(lonLabel);
    lonLayout->addWidget(lonLineEdit);

    QHBoxLayout *posLayout = new QHBoxLayout;
    posLayout->addLayout(cqLayout);
    posLayout->addLayout(ituLayout);
    posLayout->addLayout(latLayout);
    posLayout->addLayout(lonLayout);

    QVBoxLayout *utcLayout = new QVBoxLayout;
    utcLayout->addWidget(utcLabel);
    utcLayout->addWidget(utcLineEdit);

    QVBoxLayout *arrlidLayout = new QVBoxLayout;
    arrlidLayout->addWidget(arrlidLabel);
    arrlidLayout->addWidget(arrlidLineEdit);

    QVBoxLayout *delLayout = new QVBoxLayout;
    delLayout->addWidget(delRbutton);
    delLayout->addWidget(delQDateEdit);

    QHBoxLayout *thirdLayout = new QHBoxLayout;
    thirdLayout->addLayout(utcLayout);
    thirdLayout->addLayout(arrlidLayout);
    thirdLayout->addLayout(delLayout);


    QVBoxLayout *prefLayout = new QVBoxLayout;
    prefLayout->addWidget(prefLabel);
    prefLayout->addWidget(prefLineEdit);

    QGridLayout *dataLayout = new QGridLayout;
    dataLayout->addWidget(entityLabel, 0, 0);
    dataLayout->addWidget(entityLineEdit, 1, 0);
    dataLayout->addWidget(mprefLabel, 0, 1);
    dataLayout->addWidget(mprefLineEdit, 1, 1);
    dataLayout->addLayout(posLayout, 2, 0, 2, -1);
    dataLayout->addLayout(thirdLayout, 4, 0, 4, -1);
    dataLayout->addLayout(prefLayout, 8, 0, 8, -1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(dataLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
   //qDebug() << "SetupEntityDialog::SetupEntityDialog -  3" << Qt::endl;
    setLayout(mainLayout);

    setWindowTitle(tr("Entity Dialog"));

    pal = lonLineEdit->palette();
       //qDebug() << "SetupEntityDialog::SetupEntityDialog: END" << Qt::endl;
}

SetupEntityDialog::~SetupEntityDialog()
{
       //qDebug() << "SetupEntityDialog::~SetupEntityDialog " << Qt::endl;
}

void SetupEntityDialog::slotOkButtonClicked()
{
       //qDebug() << "SetupEntityDialog::slotOkButtonClicked " << Qt::endl;
    QStringList ql;
    ql.clear();


    if (entityBool && mainPrefixBool &&
        cqBool &&
        ituBool &&
        contBool &&
        latBool &&
        lonBool &&
        utcBool &&
        arrlidBool &&
        delBool &&
        delDateBool &&
        prefBool)
    {
        ql << checkEntity();
        ql << checkMainprefix();
        ql << checkContinent();
        ql << checkCQz();
        ql << checkITUz();
        ql << checkLatitude();
        ql << checkLongitude();
        ql << checkUTC();
        ql << checkARRLid();
        ql << checkDeleted();
        //ql << checkDeletedDate();
        ql << checkPrefixes();

        emit entityAdded(ql);
        accept();
    }
    else
    {
        reject();
    }
    reject();

}
QString SetupEntityDialog::checkContinent()
{
    //qDebug() << "SetupEntityDialog::checkContinent" << Qt::endl;

    if(contBool)
    {
        return contLineEdit->text();
    }
    else
    {
        return "";
    }
    return "";
}

QString SetupEntityDialog::checkEntity()
{
    //qDebug() << "SetupEntityDialog::checkEntity" << Qt::endl;

    if(entityBool)
    {
         return entityLineEdit->text();
    }
    else
    {
         return "";
    }
    return "";
}

QString SetupEntityDialog::checkMainprefix()
{
    //qDebug() << "SetupEntityDialog::checkMainprefix" << Qt::endl;

    if(mainPrefixBool)
    {
          return mprefLineEdit->text();
    }
    else
    {
          return "";
    }
     return "";
}

QString SetupEntityDialog::checkCQz()
{
       //qDebug() << "SetupEntityDialog::checkCQz" << Qt::endl;

    if(cqBool)
    {
         return cqLineEdit->text();
    }
    else
    {
         return "";
    }
    return "";

}

QString SetupEntityDialog::checkITUz()
{
       //qDebug() << "SetupEntityDialog::checkITUz" << Qt::endl;

    if(ituBool)
    {
         return ituLineEdit->text();
    }
    else
    {
         return "";
    }
    return "";

}

QString SetupEntityDialog::checkLatitude()
{
       //qDebug() << "SetupEntityDialog::checkLatitude" << Qt::endl;

    if(latBool)
    {
         return latLineEdit->text();
    }
    else
    {
         return "";
    }
    return "";

}

QString SetupEntityDialog::checkLongitude()
{
       //qDebug() << "SetupEntityDialog::checkLongitude" << Qt::endl;

    if(lonBool)
    {
         return lonLineEdit->text();
    }
    else
    {
         return "";
    }
    return "";

}

QString SetupEntityDialog::checkUTC()
{
       //qDebug() << "SetupEntityDialog::checkUTC" << Qt::endl;

    if(utcBool)
    {
         return utcLineEdit->text();
    }
    else
    {
         return "";
    }
    return "";

}

QString SetupEntityDialog::checkARRLid()
{
       //qDebug() << "SetupEntityDialog::checkARRLid" << Qt::endl;

    if(arrlidBool)
    {
         return arrlidLineEdit->text();
    }
    else
    {
         return "";
    }
    return "";


}

QString SetupEntityDialog::checkDeleted()
{
       //qDebug() << "SetupEntityDialog::checkDeleted" << Qt::endl;

    if(delBool)
    {
        return "Y";
    }
    else
    {
         return "N";
    }
    return "";

}

QString SetupEntityDialog::checkDeletedDate()
{
       //qDebug() << "SetupEntityDialog::checkDeletedDate" << Qt::endl;

    if(delDateBool)
    {
        return "00/00/0000";
    }
    else
    {
         return "";
    }
    return "";
}

QString SetupEntityDialog::checkPrefixes()
{
       //qDebug() << "SetupEntityDialog::checkPrefixes" << Qt::endl;

    if(prefBool)
    {
         return prefLineEdit->text();
    }
    else
    {
         return "";
    }
    return "";

}


void SetupEntityDialog::slotCancelButtonClicked()
{
       //qDebug() << "SetupEntityDialog::slotCancelButtonClicked " << Qt::endl;

    reject();

}

void SetupEntityDialog::slotCheckEntity()
{
       //qDebug() << "SetupEntityDialog::slotCheckEntity " << Qt::endl;

    QString aux;
    aux = entityLineEdit->text();
    if (aux.length()>2)
    {
        entityBool = true;
        entityLineEdit->setPalette(pal);
    }
    else
    {
        entityBool = false;
        entityLineEdit->setPalette(palw);
    }

}

void SetupEntityDialog::slotCheckMainprefix()
{
       //qDebug() << "SetupEntityDialog::slotCheckMainprefix" << Qt::endl;

    QString aux;
    aux = mprefLineEdit->text();
    if (aux.length()>0)
    {
        mprefLineEdit->setPalette(pal);
        mainPrefixBool = true;
    }
    else
    {
        mainPrefixBool = false;
        mprefLineEdit->setPalette(palw);
    }

}

void SetupEntityDialog::slotCheckCQz()
{
       //qDebug() << "SetupEntityDialog::slotCheckCQz" << Qt::endl;

    QString aux;
    aux = cqLineEdit->text();
    if (aux.length()>0)
    {
          cqBool = true;
          cqLineEdit->setPalette(pal);
    }
    else
    {
          cqBool = false;
          cqLineEdit->setPalette(palw);
    }

}

void SetupEntityDialog::slotCheckITUz()
{
       //qDebug() << "SetupEntityDialog::slotCheckITUz" << Qt::endl;


    QString aux;
    aux = ituLineEdit->text();
    if (aux.length()>0)
    {
         ituBool = true;
         ituLineEdit->setPalette(pal);
    }
    else
    {
         ituBool = false;
         ituLineEdit->setPalette(palw);
    }

}

void SetupEntityDialog::slotCheckContinent()
{
       //qDebug() << "SetupEntityDialog::slotCheckContinent" << Qt::endl;

    QString aux;
    aux = contLineEdit->text();
    if (aux.length()>2)
    {
        contBool = true;
        contLineEdit->setPalette(pal);
    }
    else
    {
        contBool = false;
        contLineEdit->setPalette(palw);
    }

}

void SetupEntityDialog::slotCheckLatitude()
{
       //qDebug() << "SetupEntityDialog::slotCheckLatitude" << Qt::endl;

    QString aux;
    aux = latLineEdit->text();
    if (aux.length()>0)
    {
         latBool = true;
         latLineEdit->setPalette(pal);
    }
    else
    {
         latBool = false;
         latLineEdit->setPalette(palw);
    }

}

void SetupEntityDialog::slotCheckLongitude()
{
       //qDebug() << "SetupEntityDialog::slotCheckLongitude" << Qt::endl;





    QString aux;
    aux = lonLineEdit->text();
    if (aux.length()>3)
    {
        //lonLineEdit->setPalette(*paletteOrig);
        lonBool = true;
        lonLineEdit->setPalette(pal);
    }
    else
    {
        //lonLineEdit->setForegroundRole(QPalette::WindowText);
        lonLineEdit->setPalette(palw);
        //lonLineEdit->setPalette(*paletteWrong);
        lonBool = false;
    }

}

void SetupEntityDialog::slotCheckUTC()
{
       //qDebug() << "SetupEntityDialog::slotCheckUTC" << Qt::endl;

    QString aux;
    aux = utcLineEdit->text();
    if (aux.length()>0)
    {
         utcBool = true;
         utcLineEdit->setPalette(pal);
    }
    else
    {
         utcBool = false;
         utcLineEdit->setPalette(palw);
    }

}

void SetupEntityDialog::slotCheckARRLid()
{
       //qDebug() << "SetupEntityDialog::slotCheckARRLid" << Qt::endl;

    QString aux;
    aux = arrlidLineEdit->text();

//    int n = aux.toInt();
    if (aux.length()>0)
    {
        arrlidBool = false;
        arrlidLineEdit->setPalette(pal);
    }
    else
    {
        arrlidBool = false;
        arrlidLineEdit->setPalette(palw);
    }

}

void SetupEntityDialog::slotCheckDeleted()
{
       //qDebug() << "SetupEntityDialog::slotCheckDeleted" << Qt::endl;

    if (delRbutton->isChecked())
    {
        delBool = true;
    }
    else
    {
         delBool = false;
    }

}

void SetupEntityDialog::slotCheckDeletedDate()
{
       //qDebug() << "SetupEntityDialog::slotCheckDeletedDate" << Qt::endl;
    delDateBool = false;
    //arrlidLineEdit->setPalette(palw);

}

void SetupEntityDialog::slotCheckPrefixes()
{
       //qDebug() << "SetupEntityDialog::slotCheckPrefixes" << Qt::endl;

    QString aux;
    aux = prefLineEdit->text();
    aux = aux.simplified();

    QStringList list = aux.split(",", Qt::SkipEmptyParts);

    if (aux.length()>0)
    {
         prefBool = true;
         prefLineEdit->setPalette(pal);
         //"EA1, EA2, EA3"
    }
    else
    {
         prefBool = false;
         prefLineEdit->setPalette(palw);
    }
}

