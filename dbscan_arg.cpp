#include "dbscan_arg.h"
#include "ui_dbscan_arg.h"

Dbscan_arg::Dbscan_arg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dbscan_arg),
    eps1(1.0),
    pts1(5),
    eps2(2.0),
    pts2(10),
    eps3(3.0),
    pts3(15)
{
    ui->setupUi(this);
}

Dbscan_arg::~Dbscan_arg()
{
    delete ui;
}

void Dbscan_arg::on_buttonBox_accepted()
{
    double eps = 0.0;
    int pts = 0;

    if (ui->radioButton->isChecked()) {
        eps = eps1;
        pts = pts1;
    } else if (ui->radioButton_2->isChecked()) {
        eps = eps2;
        pts = pts2;
    } else if (ui->radioButton_3->isChecked()) {
        eps = eps3;
        pts = pts3;
    } else {
        eps = ui->lineEdit->text().toDouble();
        pts = ui->lineEdit_2->text().toInt();
    }

    emit sendData(QString::number(eps), QString::number(pts));
    this->close();
}

void Dbscan_arg::on_radioButton_clicked()
{
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");

    eps1 = 1.0;
    pts1 = 5;

}

void Dbscan_arg::on_radioButton_2_clicked()
{
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");

    eps2 = 2.0;
    pts2 = 10;

}

void Dbscan_arg::on_radioButton_3_clicked()
{
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");

    eps3 = 3.0;
    pts3 = 15;
}
