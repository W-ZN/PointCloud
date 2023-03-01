#ifndef VIEW_RENDERING_H
#define VIEW_RENDERING_H

#include <QDialog>

namespace Ui {
class View_rendering;
}

class View_rendering : public QDialog
{
    Q_OBJECT

public:
    explicit View_rendering(QWidget *parent = nullptr);
    ~View_rendering();

signals:
    void sendData(QString data);

private slots:
    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_radioButton_3_clicked();

    void on_buttonBox_accepted();

private:
    Ui::View_rendering *ui;

    QString axis;
};

#endif // VIEW_RENDERING_H
