#ifndef FILTER_PT_H
#define FILTER_PT_H

#include <QDialog>

namespace Ui {
class Filter_pt;
}

class Filter_pt : public QDialog
{
    Q_OBJECT

signals:
    void sendData(QString data1, QString data2, QString data3, QString data4);

public:
    explicit Filter_pt(QWidget *parent = 0);
    ~Filter_pt();

private slots:
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_radioButton_3_clicked();
    void on_checkBox_clicked();
    void on_checkBox_2_clicked();
    void on_buttonBox_accepted();

private:
    Ui::Filter_pt *ui;
    QString axis, isSave;
};

#endif // FILTER_PT_H
