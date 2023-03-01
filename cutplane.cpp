#include "cutplane.h"
#include "ui_cutplane.h"

CutPlane::CutPlane(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CutPlane)
{
    ui->setupUi(this);
}

CutPlane::~CutPlane()
{
    delete ui;
}

void CutPlane::on_buttonBox_accepted()
{
    emit sendData(ui->lineEdit->text(), ui->lineEdit_2->text());
    this->close();
}

