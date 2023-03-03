#ifndef FIX_ARG_H
#define FIX_ARG_H

#include <QDialog>

namespace Ui {
class Fix_arg;
}

class Fix_arg : public QDialog
{
    Q_OBJECT

signals:
    void sendData(QString data1);

public:
    explicit Fix_arg(QWidget *parent = nullptr);
    ~Fix_arg();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Fix_arg *ui;
};

#endif // FIX_ARG_H




