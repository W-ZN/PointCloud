#pragma execution_character_set("utf-8")
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <pcl_function.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "tinyxml2.h"
#include <changepoint.h>
#include <ogr_spatialref.h>
//#include <unistd.h>

// vtk
#include "QVTKWidget.h"
#include <vtkRenderWindow.h>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingFreeType)

// qt
#include <QMainWindow>
#include <QDebug>
#include <QColorDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QTime>
#include <QDir>
#include <QFile>
#include <QtMath>
#include <QDirIterator>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>
#include <QMap>
#include <QIcon>
#include <QStandardItemModel>

// Other
#include "filter_pt.h"
#include "filter_guass.h"
#include "filter_voxel.h"
#include "filter_csf.h"
#include "color_selecter.h"
#include "view_rendering.h"
#include "cutplane.h"
#include "changepoint.h"
#include "dbscan_arg.h"
#include "fix.h"
#include "Fix_arg.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void view_updata(std::vector<pcl::PointCloud<pcl::PointXYZ>::Ptr> vector_cloud,std::vector<int> index);

private slots:

    void Open_clicked();
    void Save_clicked();
    void Savexml_clicked();

    void viewup_clicked();
    void viewdown_clicked();
    void viewleft_clicked();
    void viewright_clicked();
    void viewfront_clicked();
    void viewback_clicked();

    void PressBtn_pt();
    void Pt_clicked(QString data1, QString data2, QString data3, QString data4);
    void PressBtn_guass();
    void Guass_clicked(QString data1, QString data2, QString data3, QString data4);
    void PressBtn_voxel();
    void Voxel_clicked(QString data);
    void PressBtn_csf();
    void Csf_clicked(QString data1, QString data2);

    void PressBtn_rendering();
    void Rendering_setting(QString data);
    void Background_color();
    void Cloud_color();

    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionReload_triggered();
    void on_actionNoGround_triggered();
    void on_actionCutGround_triggered();
    void on_actionvoxel_triggered();
    void on_actionLinear_triggered();
    void on_actionBack_triggered();
    void on_actionExchange_triggered();
    void on_actionDbscan_triggered();
    void on_actionPlus_triggered();
    void on_actionCheck_triggered();

    void Linear();
    void on_NoGroud();
    void Cut(QString data1, QString data2);
    void PointVoxel();
    void fitMultipleLines(int n, std::vector<pcl::ModelCoefficients>& lineCoff);
    void show_dbscan(QString data1, QString data2);

    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
//    void keyPressEvent(QKeyEvent *event);
    void on_actionSelectMode_triggered();

    void on_actionFix_triggered();
    void Fixline(QString data);

private:
    Ui::MainWindow *ui;
    pcl::PointCloud<pcl::PointXYZ> cloudReload;
    std::vector<int> cloud_index;
    std::vector<std::string> cloud_name{"0","1","2"};
    std::vector<pcl::PointCloud<pcl::PointXYZ>::Ptr> cloud_vec;
    int point_size = 1;
    QStandardItemModel* model;
    Filter_pt *dialog_pt;
    Filter_guass *dialog_guass;
    Filter_voxel *dialog_voxel;
    Filter_csf *dialog_csf;
    CutPlane *dialog_cut;
    Dbscan_arg *dbscan_arg;
    Fix_arg *fix_arg;
    View_rendering *dialog_render;
    Color_selecter *dialog_colorselect;

    static bool select;
    static void pp_callback(const pcl::visualization::AreaPickingEvent& event, void* args);
};

#endif // MAINWINDOW_H

