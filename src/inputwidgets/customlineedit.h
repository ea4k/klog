#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QLineEdit>
#include <QPalette>

class CustomLineEdit : public QLineEdit {
    Q_OBJECT

public:
    explicit CustomLineEdit(QWidget *parent = nullptr);

    void setPalette(const QPalette &palette);

signals:
    void paletteChanged(const QPalette &newPalette);
};

#endif // CUSTOMLINEEDIT_H
