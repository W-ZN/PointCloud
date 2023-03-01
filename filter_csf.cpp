#include "filter_csf.h"
#include "ui_filter_csf.h"

Filter_csf::Filter_csf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Filter_csf)
{
    ui->setupUi(this);
}

Filter_csf::~Filter_csf()
{
    delete ui;
}

void Filter_csf::on_buttonBox_accepted()
{
    emit sendData(ui->lineEdit->text(), ui->lineEdit_2->text());
    this->close();
}

