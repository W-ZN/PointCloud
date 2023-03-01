#ifndef CUTPLANE_H
#define CUTPLANE_H

#include <QDialog>

namespace Ui {
class CutPlane;
}

class CutPlane : public QDialog
{
    Q_OBJECT

signals:
    void sendData(QString data1, QString data2);

public:
    explicit CutPlane(QWidget *parent = 0);
    ~CutPlane();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CutPlane *ui;
};

#endif // CUTPLANE_H



