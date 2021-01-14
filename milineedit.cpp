#include "milineedit.h"

MiLineEdit::MiLineEdit(QWidget *parent) : QLineEdit(parent)
{

}
MiLineEdit::~MiLineEdit()
{

}

void MiLineEdit::focusInEvent(QFocusEvent *e)
{
       QPalette p=QPalette();
       p.setColor(QPalette::Base,Qt::green);    //QPalette::Base
       setPalette(p);
}

void MiLineEdit::focusOutEvent(QFocusEvent *e)
{
       QPalette p1=QPalette();
       p1.setColor(QPalette::Base,Qt::white);
       setPalette(p1);
}
