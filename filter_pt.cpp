#include "filter_pt.h"
#include "ui_filter_pt.h"

Filter_pt::Filter_pt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Filter_pt)
{
    ui->setupUi(this);
}

Filter_pt::~Filter_pt()
{
    delete ui;
}

void Filter_pt::on_radioButton_clicked()
{
    if(ui->radioButton->isChecked()) {
        ui->radioButton_2->setChecked(false);
        ui->radioButton_3->setChecked(false);
        axis="x";
    }
}


void Filter_pt::on_radioButton_2_clicked()
{
    if(ui->radioButton_2->isChecked()) {
        ui->radioButton->setChecked(false);
        ui->radioButton_3->setChecked(false);
        axis="y";
    }
}


void Filter_pt::on_radioButton_3_clicked()
{
    if(ui->radioButton_3->isChecked()) {
        ui->radioButton_2->setChecked(false);
        ui->radioButton->setChecked(false);
        axis="z";
    }
}


void Filter_pt::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked()) {
        ui->checkBox_2->setChecked(false);
        isSave="0";
    }
}


void Filter_pt::on_checkBox_2_clicked()
{
    if(ui->checkBox_2->isChecked()) {
        ui->checkBox->setChecked(false);
        isSave="1";
    }
}


void Filter_pt::on_buttonBox_accepted()
{
    emit sendData(ui->lineEdit->text(), ui->lineEdit_2->text(), axis, isSave);
    this->close();
}

