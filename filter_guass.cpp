#include "filter_guass.h"
#include "ui_filter_guass.h"

Filter_guass::Filter_guass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Filter_guass)
{
    ui->setupUi(this);
}

Filter_guass::~Filter_guass()
{
    delete ui;
}

void Filter_guass::on_buttonBox_accepted()
{
    emit sendData(ui->lineEdit->text(), ui->lineEdit_2->text(), ui->lineEdit_3->text(), ui->lineEdit_4->text());
    this->close();
}

