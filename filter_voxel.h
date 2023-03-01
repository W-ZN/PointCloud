#ifndef FILTER_VOXEL_H
#define FILTER_VOXEL_H

#include <QDialog>

namespace Ui {
class Filter_voxel;
}

class Filter_voxel : public QDialog
{
    Q_OBJECT

signals:
    void sendData(QString data);

public:
    explicit Filter_voxel(QWidget *parent = 0);
    ~Filter_voxel();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Filter_voxel *ui;
};

#endif // FILTER_VOXEL_H
