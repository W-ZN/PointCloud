#include "color_selecter.h"
#include <QHBoxLayout>
#include <qcolordialog.h>

Color_selecter::Color_selecter()
{
    QColor c = QColorDialog::getColor(Qt::white, this);

    if(c.isValid()) {
        color = c;
    } else {
        c.setRgb(0, 0, 0, 0);
        color = c;
    }
}

Color_selecter::~Color_selecter() {}
