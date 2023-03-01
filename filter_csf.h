#ifndef FILTER_CSF_H
#define FILTER_CSF_H

#include <QDialog>

namespace Ui {
class Filter_csf;
}

class Filter_csf : public QDialog
{
    Q_OBJECT

signals:
    void sendData(QString data1, QString data2);

public:
    explicit Filter_csf(QWidget *parent = 0);
    ~Filter_csf();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Filter_csf *ui;
};

#endif // FILTER_CSF_H



