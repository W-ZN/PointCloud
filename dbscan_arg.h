#ifndef DBSCAN_ARG_H
#define DBSCAN_ARG_H

#include <QDialog>

namespace Ui {
class Dbscan_arg;
}

class Dbscan_arg : public QDialog
{
    Q_OBJECT

public:
    explicit Dbscan_arg(QWidget *parent = nullptr);
    ~Dbscan_arg();

signals:
    void sendData(QString eps, QString pts);

private slots:
    void on_buttonBox_accepted();
    void on_radioButton_clicked();
    void on_radioButton_2_clicked();
    void on_radioButton_3_clicked();

private:
    Ui::Dbscan_arg *ui;

    double eps1;
    int pts1;
    double eps2;
    int pts2;
    double eps3;
    int pts3;
};

#endif // DBSCAN_ARG_H
