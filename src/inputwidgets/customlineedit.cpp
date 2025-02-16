#include "customlineedit.h"

CustomLineEdit::CustomLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
}

void CustomLineEdit::setPalette(const QPalette &palette) {
    QLineEdit::setPalette(palette);
    emit paletteChanged(palette);
}
