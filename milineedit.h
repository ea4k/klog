#ifndef MILINEEDIT_H
#define MILINEEDIT_H

#include <QWidget>
#include <QLineEdit>


class MiLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    MiLineEdit(QWidget *parent = 0);
    ~MiLineEdit();

    virtual void focusInEvent(QFocusEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);


};

#endif // MILINEEDIT_H
