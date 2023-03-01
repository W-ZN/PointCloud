#include "view_rendering.h"
#include "ui_view_rendering.h"

View_rendering::View_rendering(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::View_rendering)
{
    ui->setupUi(this);
}

View_rendering::~View_rendering()
{
    delete ui;
}

void View_rendering::on_radioButton_clicked()
{
    if(ui->radioButton->isChecked()) {
        ui->radioButton_2->setChecked(false);
        ui->radioButton_3->setChecked(false);
        axis = "x";
    }
}

void View_rendering::on_radioButton_2_clicked()
{
    if(ui->radioButton_2->isChecked()) {
        ui->radioButton->setChecked(false);
        ui->radioButton_3->setChecked(false);
        axis = "y";
    }
}

void View_rendering::on_radioButton_3_clicked()
{
    if(ui->radioButton_3->isChecked()) {
        ui->radioButton->setChecked(false);
        ui->radioButton_2->setChecked(false);
        axis = "z";
    }
}


void View_rendering::on_buttonBox_accepted()
{
    emit sendData(axis);
    this->close();
}

