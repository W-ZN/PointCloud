#include "fix_arg.h"
#include "ui_fix_arg.h"

Fix_arg::Fix_arg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Fix_arg)
{
    ui->setupUi(this);
}

Fix_arg::~Fix_arg()
{
    delete ui;
}

void Fix_arg::on_buttonBox_accepted()
{
    emit sendData(ui->lineEdit->text());
    this->close();
}




