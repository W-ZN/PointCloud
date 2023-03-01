#include "filter_voxel.h"
#include "ui_filter_voxel.h"

Filter_voxel::Filter_voxel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Filter_voxel)
{
    ui->setupUi(this);
}

Filter_voxel::~Filter_voxel()
{
    delete ui;
}

void Filter_voxel::on_buttonBox_accepted()
{
    emit sendData(ui->lineEdit->text());
    this->close();
}

