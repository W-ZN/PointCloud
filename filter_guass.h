#ifndef FILTER_GUASS_H
#define FILTER_GUASS_H

#include <QDialog>

namespace Ui {
class Filter_guass;
}

class Filter_guass : public QDialog
{
    Q_OBJECT

signals:
    void sendData(QString data1, QString data2, QString data3, QString data4);

public:
    explicit Filter_guass(QWidget *parent = 0);
    ~Filter_guass();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Filter_guass *ui;
};

#endif // FILTER_GUASS_H
