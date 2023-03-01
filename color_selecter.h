#ifndef COLOR_SELECTER_H
#define COLOR_SELECTER_H

#include <QWidget>

class Color_selecter : public QWidget
{
    Q_OBJECT

public:

    Color_selecter();

    ~Color_selecter();

    void setColor(const QColor &c)
    {
        if(c.isValid()) {
            color=c;
        }

    }

    QColor getColor() {
        return color;
    }

private:

    QColor color;


};
#endif // COLOR_SELECTER_H
