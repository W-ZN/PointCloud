#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pcl_function.h"
#include "view_rendering.h"
#include "color_selecter.h"
#include "DBSCAN.h"
#include "KMeans.h"
#include "fitting.h"
#include "curve_fitting.h"
#include "dbscan_arg.h"

// 添加菜单栏、工具栏、状态栏等
#include <string>
#include <cstring>
#include <random>
#include <QMenu>
#include <QMenuBar>
#include <QLabel>
#include <QToolBar>
#include <QStatusBar>
#include <QMouseEvent>
#include <QAbstractButton>
#include <QProgressDialog>
#include <QStandardItemModel>
#include <QAbstractAnimation>

using namespace std;
using namespace pcl;
using namespace Eigen;
using namespace tinyxml2;

int num = 0;
int status = 0;
int kml_num = 0;
double dbscan_esp = 0;
double dbscan_pts = 0;
unsigned char* pointmark;
XMLDocument doc;
pcl::PointCloud<pcl::PointXYZ> nul;         // 空点云
pcl::PointCloud<pcl::PointXYZ> cloud;       // 主点云
pcl::PointCloud<pcl::PointXYZ> cloud_;      // 备份点云
pcl::PointCloud<pcl::PointXYZ> linecloud;   // 提取点云
pcl::PointCloud<pcl::PointXYZ> back;        // 回退点云
boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer;
pcl::PointCloud<pcl::PointXYZ>::Ptr clicked_points_3d(new pcl::PointCloud<pcl::PointXYZ>);
pcl::PointCloud<pcl::PointXYZRGB>::Ptr dbscan_all_cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
pcl::PointCloud<pcl::PointXYZ>::Ptr selected_cloud(new pcl::PointCloud<pcl::PointXYZ>);

bool MainWindow::select = false; // 初始化静态全局变量 select

void dbscan_with_density(pcl::PointCloud<pcl::PointXYZ>::Ptr input_cloud, std::vector<pcl::Indices>& cluster_indices);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // PCL 显示控件 —— 初始化
    viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
    viewer->setBackgroundColor(0, 0, 0);
    ui->qvtkwidget->SetRenderWindow(viewer->getRenderWindow());
    viewer->setupInteractor(ui->qvtkwidget->GetInteractor(), ui->qvtkwidget->GetRenderWindow());
    viewer->registerAreaPickingCallback(pp_callback, (void*)&cloud);
    ui->qvtkwidget->update();
    ui->qvtkwidget->setFocusPolicy(Qt::StrongFocus);

    // 菜单控件 —— 初始化
    QMenuBar *menu_bar = new QMenuBar(this);
    this->setMenuBar(menu_bar);
    menu_bar->setStyleSheet("font-size: 16px");
    QMenu *file_menu = new QMenu("文件" ,menu_bar);
    QMenu *view_menu=new QMenu("视角",menu_bar);
    QMenu *tools_menu=new QMenu("工具", menu_bar);
    QMenu *setting_menu=new QMenu("设置",menu_bar);

    // 菜单控件 —— 下拉框
    QAction *open_action = new QAction("打开 ");
    QAction *save_action = new QAction("另存为 ");
    QAction *out_action = new QAction("导出 ");

    QAction *view_up = new QAction("切换视角—上 ");
    QAction *view_down = new QAction("切换视角—下 ");
    QAction *view_left = new QAction("切换视角—左 ");
    QAction *view_right = new QAction("切换视角—右 ");
    QAction *view_front = new QAction("切换视角—前 ");
    QAction *view_back = new QAction("切换视角—后 ");

    QAction *view_rendering = new QAction("点云渲染 ");
    QAction *background_select = new QAction("背景颜色 ");
    QAction *cloud_select = new QAction("点云颜色 ");

    QAction *passthrough_action = new QAction("直通滤波 ");
    QAction *guess_action = new QAction("高斯滤波 ");
    QAction *voxel_action = new QAction("体素滤波 ");
    QAction *csf_action = new QAction("CSF布匹滤波 ");

    // 工具栏设置
    QStandardItemModel *model = new QStandardItemModel(this);

    //添加动作到文件菜单，QAction就会自动变成子菜单
    file_menu->addAction(open_action);
    file_menu->addAction(save_action);
    file_menu->addAction(out_action);

    view_menu->addAction(view_up);
    view_menu->addAction(view_down);
    view_menu->addAction(view_left);
    view_menu->addAction(view_right);
    view_menu->addAction(view_front);
    view_menu->addAction(view_back);

    tools_menu->addAction(passthrough_action);
    tools_menu->addAction(guess_action);
    tools_menu->addAction(voxel_action);
    tools_menu->addAction(csf_action);

    setting_menu->addAction(view_rendering);
    setting_menu->addAction(background_select);
    setting_menu->addAction(cloud_select);

    menu_bar->addMenu(file_menu);
    menu_bar->addMenu(view_menu);
    menu_bar->addMenu(tools_menu);
    menu_bar->addMenu(setting_menu);

    // 菜单控件 —— 方法链接
    connect(open_action, SIGNAL(triggered()), this, SLOT(Open_clicked()));  // 读取文件
    connect(save_action, SIGNAL(triggered()), this, SLOT(Save_clicked()));  // 保存文件
    connect(out_action, SIGNAL(triggered()), this, SLOT(Savexml_clicked()));   // 导出kml文件
    connect(passthrough_action, SIGNAL(triggered()), this, SLOT(PressBtn_pt()));    // 直通滤波
    connect(guess_action, SIGNAL(triggered()), this, SLOT(PressBtn_guass()));    // 高斯滤波
    connect(voxel_action, SIGNAL(triggered()), this, SLOT(PressBtn_voxel()));    // 体素滤波
    connect(csf_action, SIGNAL(triggered()), this, SLOT(PressBtn_csf()));    // CSF布匹滤波
    connect(view_rendering, SIGNAL(triggered(bool)),  this,SLOT(PressBtn_rendering())); // 点云渲染
    connect(background_select, SIGNAL(triggered()), this, SLOT(Background_color()));  // 改变点云颜色
    connect(cloud_select, SIGNAL(triggered()), this, SLOT(Cloud_color()));  // 改变背景颜色

    connect(view_up, SIGNAL(triggered()), this, SLOT(viewup_clicked()));  // 切换视角—上
    connect(view_down, SIGNAL(triggered()), this, SLOT(viewdown_clicked()));  // 切换视角—下
    connect(view_left, SIGNAL(triggered()), this, SLOT(viewleft_clicked()));   // 切换视角—左
    connect(view_right, SIGNAL(triggered()), this, SLOT(viewright_clicked()));    // 切换视角-右
    connect(view_front, SIGNAL(triggered()), this, SLOT(viewfront_clicked()));    // 切换视角—前
    connect(view_back, SIGNAL(triggered()), this, SLOT(viewback_clicked()));    // 切换视角-后

    // 状态栏控件 —— 初始化
    QLabel *status_label = new QLabel(" 点云数量: " + QString::number(cloud.size()), this);
    ui->statusBar->addWidget(status_label);
    // 过滤菜单控件的鼠标移动事件，让状态栏能够接收到鼠标事件
//    menu_bar->installEventFilter(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->menubar && event->type() == QEvent::MouseMove) {
        // 将鼠标移动事件转发给状态栏
        QMouseEvent *mouse_event = dynamic_cast<QMouseEvent*>(event);
        if (mouse_event) {
            QCoreApplication::sendEvent(ui->statusBar, mouse_event);
            return true;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::enterEvent(QEvent *event)
{
    QMainWindow::enterEvent(event);
    ui->statusBar->showMessage("点云数量: " + QString::number(cloud.size()));
}

void MainWindow::leaveEvent(QEvent *event)
{
    QMainWindow::leaveEvent(event);
    ui->statusBar->clearMessage();
}

/**********************************************   菜单栏   ********************************************************************/

// ---------------------------- 第一列、读取导出操作 ------------------------------------

// 一、读取（Txt 读取点云）
void CreateCloudFromTxt(const std::string& file_path, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud) {
    std::cout << file_path.c_str() << std::endl;
    std::ifstream file(file_path.c_str());
    std::string line;
    pcl::PointXYZ point;
    while (getline(file, line)) {
        std::stringstream ss(line);
        ss >> point.x;
        ss >> point.y;
        ss >> point.z;
        cloud->push_back(point);
    }
    file.close();
}

// 一、读取（Las 读取点云）
void Read_Las(const std::string& file_path, pcl::PointCloud<pcl::PointXYZ>::Ptr cloud) {
    LASreadOpener lasLoad;
    lasLoad.set_file_name(file_path.c_str());
    std::cout << file_path.c_str() << std::endl;
    LASreader* lasReader = lasLoad.open(false);
    uint32_t ptCount = lasReader->header.number_of_point_records;  //uint32_t 为 unsigned int

    cloud->width = ptCount;
    cloud->height = 1;
    cloud->is_dense = true;
    cloud->resize(ptCount);

    uint32_t i = 0;
    while (lasReader->read_point() && i < ptCount)
    {
        cloud->points[i].x = lasReader->point.get_x();
        cloud->points[i].y = lasReader->point.get_y();
        cloud->points[i].z = lasReader->point.get_z();
        ++i;
    }
}

// 一、文件读取
void MainWindow::Open_clicked() {
    // 文件选取
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), "", tr("Las Files(*.las) ;;Pcb Files(*.pcd *.ply *.txt) ;;All Files (*.*)"));

    // 文件校验
    if (fileName.endsWith("ply")) {
        qDebug() << fileName;
        if(pcl::io::loadPLYFile<pcl::PointXYZ>(fileName.toStdString(), cloud) == -1) {
            qDebug() << "Couldn't read file  \n";
            return;
        }
        cloudReload = cloud;
    }
    else if (fileName.endsWith("pcd")) {
        qDebug() << fileName;
        if(pcl::io::loadPCDFile<pcl::PointXYZ>(fileName.toStdString(), cloud) == -1) {
            qDebug() << "Couldn't read file  \n";
            return;
        }
        cloudReload = cloud;
    }
    else if (fileName.endsWith("txt")) {
        qDebug() << fileName;
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloudTemp(new pcl::PointCloud<pcl::PointXYZ>);
        CreateCloudFromTxt(fileName.toStdString(), cloudTemp);
        cloud = *cloudTemp;
        cloudReload = *cloudTemp;
  }
    else if (fileName.endsWith("las")) {
        qDebug() << fileName;
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloudTemp(new pcl::PointCloud<pcl::PointXYZ>);
        Read_Las(fileName.toStdString(), cloudTemp);
        cloud = *cloudTemp;
        cloudReload = *cloudTemp;
    }
    else if(cloud.empty()) return;

//    cloud_vec.push_back(cloud.makeShared());
//    cloud_index.push_back(1);

    pointmark = new unsigned char[cloud.size()];

    back = cloud;

    pcl::PointCloud<pcl::PointXYZ>::Ptr nu(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr nu_(new pcl::PointCloud<pcl::PointXYZRGB>);
    clicked_points_3d = nu;
    dbscan_all_cloud = nu_;
    selected_cloud = nu;

    // 显示点云数量
    int size = static_cast<int>(cloud.size());
    QString PointSize = QString("%1").arg(size);

    ui->statusBar->showMessage("点云数量: "+PointSize);

    //移除窗口点云
    viewer->removeAllPointClouds();
    viewer->removeAllShapes();

    //点云设置
    viewer->addPointCloud(cloud.makeShared(), cloud_name[0]);

    //设置点云大小
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, point_size, cloud_name[0]);
    viewer->resetCamera();
    ui->qvtkwidget->update();

    clicked_points_3d->width = 0;
    clicked_points_3d = nul.makeShared();
}

// 二、文件保存
void MainWindow::Save_clicked() {
    QString filename = QFileDialog::getSaveFileName(this, tr ("Open point cloud"), "", tr ("Point cloud data (*.pcd *.ply)"));
    if (cloud.empty()) {
        return;
    } else {
        if (filename.isEmpty ())  return;

        int return_status;
        if (filename.endsWith (".pcd", Qt::CaseInsensitive))
            return_status = pcl::io::savePCDFileBinary (filename.toStdString(), *cloud.makeShared());
        else if (filename.endsWith (".ply", Qt::CaseInsensitive))
            return_status = pcl::io::savePLYFileBinary (filename.toStdString(), *cloud.makeShared());
        else {
            filename.append(".ply");
            return_status = pcl::io::savePLYFileBinary (filename.toStdString(), *cloud.makeShared());
        }

        if (return_status != 0)
        {
            PCL_ERROR("Error writing point cloud %s\n", filename.toStdString().c_str ());
            return;
        }
    }
}

// 三、XML导出
void MainWindow::Savexml_clicked() {
    QString filename = QFileDialog::getSaveFileName(this, tr ("Open point cloud"), "", tr ("Keyhole Markup Language data (*.kml)"));
    if (cloud.empty()) {
        return;
    } else {
        if (filename.isEmpty ())  return;
        if (filename.endsWith (".kml", Qt::CaseInsensitive)){
            doc.SaveFile(filename.toStdString ().c_str());
        }
    }
}

// ---------------------------- 第二列、点云视角 ------------------------------------

void MainWindow::viewup_clicked() {
    viewer->setCameraPosition(0, 0, 50, 0, -1, 0);
    viewer->spinOnce();

    int size_1 = static_cast<int> (cloud.size());
    QString PointSize = QString("%1").arg(size_1);
    ui->statusBar->showMessage("点云数量: "+PointSize);
    ui->qvtkwidget->update();
}

void MainWindow::viewdown_clicked() {
    viewer->setCameraPosition(0, 0, -50, 0, 1, 0);
    viewer->spinOnce();

    int size_1 = static_cast<int> (cloud.size());
    QString PointSize = QString("%1").arg(size_1);
    ui->statusBar->showMessage("点云数量: "+PointSize);
    ui->qvtkwidget->update();
};

void MainWindow::viewleft_clicked() {
    viewer->setCameraPosition(-50, 0, 0, 0, -1, 0);
    viewer->spinOnce();

    int size_1 = static_cast<int> (cloud.size());
    QString PointSize = QString("%1").arg(size_1);
    ui->statusBar->showMessage("点云数量: "+PointSize);
    ui->qvtkwidget->update();
};

void MainWindow::viewright_clicked() {
    viewer->setCameraPosition(50, 0, 0, 0, -1, 0);
    viewer->spinOnce();

    int size_1 = static_cast<int> (cloud.size());
    QString PointSize = QString("%1").arg(size_1);
    ui->statusBar->showMessage("点云数量: "+PointSize);
    ui->qvtkwidget->update();
};

void MainWindow::viewfront_clicked() {
    pcl::visualization::Camera camera;
    camera.pos[0] = 0; camera.pos[1] = 0; camera.pos[2] = 50;
    camera.view[0] = 0; camera.view[1] = 0; camera.view[2] = 0;
    camera.focal[0] = 0; camera.focal[1] = 1; camera.focal[2] = 0;
    camera.fovy = 45; camera.clip[0] = 0.1; camera.clip[1] = 1000;
    viewer->setCameraParameters(camera);
    viewer->spinOnce();

    int size_1 = static_cast<int> (cloud.size());
    QString PointSize = QString("%1").arg(size_1);
    ui->statusBar->showMessage("点云数量: "+PointSize);
    ui->qvtkwidget->update();
};

void MainWindow::viewback_clicked() {
    pcl::visualization::Camera camera;
    camera.pos[0] = 0; camera.pos[1] = 0; camera.pos[2] = -50;
    camera.view[0] = 0; camera.view[1] = 0; camera.view[2] = 0;
    camera.focal[0] = 0; camera.focal[1] = 1; camera.focal[2] = 0;
    camera.fovy = 45; camera.clip[0] = 0.1; camera.clip[1] = 1000;
    viewer->setCameraParameters(camera);
    viewer->spinOnce();

    int size_1 = static_cast<int> (cloud.size());
    QString PointSize = QString("%1").arg(size_1);
    ui->statusBar->showMessage("点云数量: "+PointSize);
    ui->qvtkwidget->update();
};

// ---------------------------- 第三列、点云处理 ------------------------------------

// 一、直通滤波（点击）
void MainWindow::PressBtn_pt() {
    dialog_pt = new Filter_pt();
    connect(dialog_pt, SIGNAL(sendData(QString, QString, QString, QString)), this, SLOT(Pt_clicked(QString, QString, QString, QString)));
    if (dialog_pt->exec() == QDialog::Accepted) {};
    delete dialog_pt;
}
// 一、直通滤波
void MainWindow::Pt_clicked(QString data1, QString data2, QString data3, QString data4) {
    if (cloud.empty()) {
        QMessageBox::warning(this, "Warning", "无点云输入！");
        return;
    } else {
        if (data1.isEmpty() || data2.isEmpty() || data3.isEmpty() || data4.isEmpty()) {
            QMessageBox::warning(this, "Warning", "参数读取错误！");
            return;
        }

        back = cloud;

        float a = data1.toFloat();
        float b = data2.toFloat();
        QString c = data3;
        float d = data4.toFloat();

        auto cloud_out = pcl_filter_passthrough(cloud.makeShared(), a, b, c, d);
        cloud = *cloud_out;
        cloud_ = cloud;

        int size_1 = static_cast<int> (cloud.size());
        QString PointSize = QString("%1").arg(size_1);

        ui->statusBar->showMessage("点云数量: "+PointSize);
        viewer->removeAllPointClouds();
        viewer->removeAllShapes();
        viewer->addPointCloud(cloud.makeShared() ,cloud_name[0]);
        viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, point_size, cloud_name[0]);
        viewer->resetCamera();
        ui->qvtkwidget->update();
    }
}

// 二、高斯滤波（点击）
void MainWindow::PressBtn_guass() {
    dialog_guass = new Filter_guass();
    connect(dialog_guass, SIGNAL(sendData(QString, QString, QString, QString)), this, SLOT(Guass_clicked(QString, QString, QString, QString)));
    if (dialog_guass->exec() == QDialog::Accepted) {};
    delete dialog_guass;
}
// 二、高斯滤波
void MainWindow::Guass_clicked(QString data1, QString data2, QString data3, QString data4) {
    if (cloud.empty()) {
        QMessageBox::warning(this, "Warning", "无点云输入！");
        return;
    } else {
        if (data1.isEmpty() || data2.isEmpty() || data3.isEmpty() || data4.isEmpty()) {
            QMessageBox::warning(this, "Warning", "参数读取错误！");
            return;
        }

        back = cloud;

        float a = data1.toFloat();
        float b = data2.toFloat();
        float c = data3.toFloat();
        float d = data4.toFloat();

        auto cloud_out = pcl_filter_guass(cloud.makeShared(), a, b, c, d);
        cloud = *cloud_out;
        cloud_ = cloud;

        int size_1 = static_cast<int> (cloud.size());
        QString PointSize = QString("%1").arg(size_1);
        ui->statusBar->showMessage("点云数量: "+PointSize);
        viewer->removeAllPointClouds();
        viewer->removeAllShapes();
        viewer->addPointCloud(cloud.makeShared() ,cloud_name[0]);
        viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, point_size, cloud_name[0]);
        viewer->resetCamera();
        ui->qvtkwidget->update();
    }
}

// 三、体素滤波（点击）
void MainWindow::PressBtn_voxel() {
    dialog_voxel = new Filter_voxel();
    connect(dialog_voxel, SIGNAL(sendData(QString)), this, SLOT(Voxel_clicked(QString)));
    if (dialog_voxel->exec() == QDialog::Accepted) {};
    delete dialog_voxel;
}
// 三、体素滤波
void MainWindow::Voxel_clicked(QString data) {
    if (cloud.empty()) {
        QMessageBox::warning(this, "Warning", "无点云输入！");
        return;
    } else {
        if (data.isEmpty()) {
            QMessageBox::warning(this, "Warning", "参数读取错误！");
            return;
        }

        back = cloud;

        float size = data.toFloat();

        auto cloud_out = pcl_filter_voxel(cloud.makeShared(), size);
        cloud = *cloud_out;
        cloud_ = cloud;

        int size_1 = static_cast<int> (cloud.size());
        QString PointSize = QString("%1").arg(size_1);
        ui->statusBar->showMessage("点云数量: "+PointSize);
        viewer->removeAllPointClouds();
        viewer->removeAllShapes();
        viewer->addPointCloud(cloud.makeShared() ,cloud_name[0]);
        viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, point_size, cloud_name[0]);
        viewer->resetCamera();
        ui->qvtkwidget->update();
    }
}

// 四、CSF 布匹滤波（点击）
void MainWindow::PressBtn_csf() {
    dialog_csf = new Filter_csf();
    connect(dialog_csf, SIGNAL(sendData(QString)), this, SLOT(Voxel_clicked(QString)));
    if (dialog_csf->exec() == QDialog::Accepted) {};
    delete dialog_csf;
}
// 四、CSF 布匹滤波
void MainWindow::Csf_clicked(QString data1, QString data2) {
    if (cloud.empty()) {
        QMessageBox::warning(this, "Warning", "无点云输入！");
        return;
    } else {
        if (data1.isEmpty() || data2.isEmpty()) {
            QMessageBox::warning(this, "Warning", "参数读取错误！");
            return;
        }

        back = cloud;

        float a = data1.toFloat();
        float b = data2.toFloat();

        auto cloud_out = pcl_filter_csf(cloud.makeShared(), a, b);
        cloud = *cloud_out;
        cloud_ = cloud;

        int size_1 = static_cast<int> (cloud.size());
        QString PointSize = QString("%1").arg(size_1);
        ui->statusBar->showMessage("点云数量: "+PointSize);
        viewer->removeAllPointClouds();
        viewer->removeAllShapes();
        viewer->addPointCloud(cloud.makeShared() ,cloud_name[0]);
        viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, point_size, cloud_name[0]);
        viewer->resetCamera();
        ui->qvtkwidget->update();
    }
}

// ---------------------------- 第四列、点云渲染 ------------------------------------

// 一、点云渲染（点击）
void MainWindow::PressBtn_rendering() {
    dialog_render = new View_rendering();
    connect(dialog_render, SIGNAL(sendData(QString)), this, SLOT(Rendering_setting(QString)));
    if(dialog_render->exec() == QDialog::Accepted) {};
    delete dialog_render;
}

// 一、点云渲染
void MainWindow::Rendering_setting(QString data) {
    if (!cloud.empty()) {
        pcl::visualization::PointCloudColorHandlerGenericField<pcl::PointXYZ>render(cloud.makeShared(), data.toStdString());
        viewer->updatePointCloud(cloud.makeShared(), render, cloud_name[0]);
    }
    return;
}

// 二、背景颜色改变
void MainWindow::Background_color() {
    dialog_colorselect = new Color_selecter();
    QColor color = dialog_colorselect->getColor();
    viewer->setBackgroundColor(color.redF(), color.greenF(), color.blueF());
    return;
}

// 三、颜色选择
void MainWindow::Cloud_color()
{
    dialog_colorselect =new Color_selecter();
    QColor color = dialog_colorselect->getColor();
    QColor temp;

    if (!cloud.empty() && (color!=temp) ) {
        //viewer->removeAllPointClouds();
        pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ>slected_color(cloud.makeShared(), color.redF() * 255, color.greenF() * 255, color.blueF() * 255);
        viewer->updatePointCloud(cloud.makeShared(),slected_color,cloud_name[0]);
        //viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, point_size, cloud_name[0]);
    } else {
        pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ>slected_color(cloud.makeShared(),255,255,255);
        viewer->updatePointCloud(cloud.makeShared(),slected_color,cloud_name[0]);
    }
    return ;
}

/*********************************************  交互功能   **********************************************************************/

// 线性特征提取
void MainWindow::Linear()
{
    back = cloud;

    // 分割电力线
    float max_dist = 0.14, threshould = 0.5;
    auto power_line_idx = powerLineSegment(cloud.makeShared(), max_dist, threshould);

    // 提取电力线
    pcl::ExtractIndices<pcl::PointXYZ> extr;
    extr.setInputCloud(cloud.makeShared());
    extr.setIndices(std::make_shared<const std::vector<int>>(power_line_idx));//设置索引
    pcl::PointCloud<pcl::PointXYZ>::Ptr power_line_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    extr.filter(*power_line_cloud);     //提取对应索引的点云
    cloud = *power_line_cloud;
    cloud_ = cloud;

    int size_1 = static_cast<int> (cloud.size());
    QString PointSize = QString("%1").arg(size_1);
    ui->statusBar->showMessage("点云数量: "+PointSize);
    viewer->removeAllPointClouds();
    viewer->removeAllShapes();
    viewer->addPointCloud(cloud.makeShared() ,cloud_name[0]);
    viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, point_size, cloud_name[0]);
    viewer->resetCamera();
    ui->qvtkwidget->update();
}

// 地面去除工具
void MainWindow::on_NoGroud()
{
//    if (cloud.empty()) {
//        QMessageBox::warning(this, "Warning", "无点云输入！");
//        return;
//    }

    back = cloud;

    // CSF 布匹滤波
    float rigidness = 1.5;
    float time_step = 0.7;
    auto cloud_out = pcl_filter_csf(cloud.makeShared(), rigidness, time_step);
    cloud = *cloud_out;
    cloud_ = cloud;

    int size_1 = static_cast<int> (cloud.size());
    QString PointSize = QString("%1").arg(size_1);
    ui->statusBar->showMessage("点云数量: "+PointSize);
    viewer->removeAllPointClouds();
    viewer->removeAllShapes();
    viewer->addPointCloud(cloud.makeShared() ,cloud_name[0]);
    viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, point_size, cloud_name[0]);
    viewer->resetCamera();
    ui->qvtkwidget->update();
}

// 平面去除工具
void MainWindow::Cut(QString data1, QString data2)
{
    double max, min;
    if (cloud.empty()) {
        QMessageBox::warning(this, "Warning", "无点云输入！");
        return;
    }
    if (data1.isEmpty()) {
        QMessageBox::warning(this, "Warning", "参数读取错误！");
        return;
    } else if (!(data1.isEmpty()) && data2.isEmpty()) {
        max = data1.toFloat();
        min = -50;
    } else if (!(data1.isEmpty()) && !(data2.isEmpty())) {
        max = data1.toFloat();
        min = data2.toFloat();
    }

    back = cloud;

    pcl::PassThrough<pcl::PointXYZ> pass;//设置滤波器对象
    pass.setInputCloud(cloud.makeShared());
    pass.setFilterFieldName("z");
    pass.setFilterLimits(min, max);//区间设置
    pass.setFilterLimitsNegative(true);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_pt (new pcl::PointCloud<pcl::PointXYZ>()) ;
    pass.filter(*cloud_pt);
    cloud = *cloud_pt;
    cloud_ = cloud;

    int size_1 = static_cast<int> (cloud.size());
    QString PointSize = QString("%1").arg(size_1);
    ui->statusBar->showMessage("点云数量: "+PointSize);
    viewer->removeAllPointClouds();
    viewer->removeAllShapes();
    viewer->addPointCloud(cloud.makeShared() ,cloud_name[0]);
    viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, point_size, cloud_name[0]);
    viewer->resetCamera();
    ui->qvtkwidget->update();
}

// 点云缩放工具
void MainWindow::PointVoxel()
{
//    if (cloud.empty()) {
//        QMessageBox::warning(this, "Warning", "无点云输入！");
//        return;
//    }

    back = cloud;

    pcl::VoxelGrid<pcl::PointXYZ> voxel_grid;
    Eigen::Vector4f leafsize{0.2, 0.2, 0.2, 0.2};
    voxel_grid.setLeafSize(leafsize);
    voxel_grid.setMinimumPointsNumberPerVoxel(10);
    voxel_grid.setInputCloud(cloud.makeShared());
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_outv (new pcl::PointCloud<pcl::PointXYZ> ()) ;
    voxel_grid.filter(*cloud_outv);
    cloud = *cloud_outv;
    cloud_ = cloud;

    int size_1 = static_cast<int> (cloud.size());
    QString PointSize = QString("%1").arg(size_1);
    ui->statusBar->showMessage("点云数量: "+PointSize);
    viewer->removeAllPointClouds();
    viewer->removeAllShapes();
    viewer->addPointCloud(cloud.makeShared() ,cloud_name[0]);
    viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, point_size, cloud_name[0]);
    viewer->resetCamera();
    ui->qvtkwidget->update();
}

void MainWindow::view_updata(std::vector<pcl::PointCloud<pcl::PointXYZ>::Ptr> vector_cloud,std::vector<int> index)
{
    viewer->removeAllPointClouds();
    viewer->removeAllShapes();
    for(int i=0;i<vector_cloud.size();i++) {
        if(index[i]==1) {
            pcl::visualization::PointCloudColorHandlerGenericField<pcl::PointXYZ>render(vector_cloud[i], "intensity");
            viewer->addPointCloud<pcl::PointXYZ>(vector_cloud[i],render,std::to_string(i));
            viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, point_size, std::to_string(i));
        }
    }
    //viewer->resetCamera();
    ui->qvtkwidget->update();
}

// 分割直线
void MainWindow::fitMultipleLines(int n, std::vector<pcl::ModelCoefficients>& lineCoff)
{
    //QString filename = QFileDialog::getSaveFileName(this, tr ("Open point cloud"), "", tr ("Keyhole Markup Language data (*.kml)"));

    if (cloud.empty()) {
        return;
    } else {

    }
}

/*********************************************   工具栏   **********************************************************************/

// 一、快捷读取
void MainWindow::on_actionOpen_triggered()
{
    MainWindow::Open_clicked();
}

// 二、快捷保存
void MainWindow::on_actionSave_triggered()
{
    if (cloud.empty()) {
        QMessageBox::warning(this, "Warning", "无点云输入！");
        return;
    }
    QString filename = QFileDialog::getSaveFileName(this, tr ("Open point cloud"), "", tr ("Keyhole Markup Language data (*.kml)"));
    doc.SaveFile(filename.toStdString ().c_str());
//    int n = 5;
//    std::vector<pcl::ModelCoefficients> LinesCoefficients;
//    fitMultipleLines(n, LinesCoefficients);
//    std::vector<pcl::ModelCoefficients>nul;
//    LinesCoefficients = nul;
//    MainWindow::dbscanline();
//    MainWindow::Savexml_clicked();
}

// 三、点云重载
void MainWindow::on_actionReload_triggered()
{
    if (cloudReload.empty()) {
        QMessageBox::warning(this, "Warning", "无点云输入！");
        return;
    }

    cloud = cloudReload;

    pointmark = new unsigned char[cloud.size()];

    pcl::PointCloud<pcl::PointXYZ>::Ptr nu(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr nu_(new pcl::PointCloud<pcl::PointXYZRGB>);
    clicked_points_3d = nu;
    dbscan_all_cloud = nu_;
    selected_cloud = nu;

    // 显示点云数量
    int size = static_cast<int>(cloud.size());
    QString PointSize = QString("%1").arg(size);

    ui->statusBar->showMessage("点云数量: "+PointSize);

    //移除窗口点云
    viewer->removeAllPointClouds();
    viewer->removeAllShapes();

    //点云设置
    viewer->addPointCloud(cloud.makeShared(), cloud_name[0]);

    //设置点云大小
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, point_size, cloud_name[0]);
    viewer->resetCamera();
    ui->qvtkwidget->update();
}

// 四、回退
void MainWindow::on_actionBack_triggered()
{

//    if (cloud_.empty()) {
//        QMessageBox::warning(this, "Warning", "当前无退回！");
//        return;
//    }

    cloud = back;
    pointmark = new unsigned char[cloud.size()];

    pcl::PointCloud<pcl::PointXYZ>::Ptr nu(new pcl::PointCloud<pcl::PointXYZ>);
    clicked_points_3d = nu;

    // 显示点云数量
    int size = static_cast<int>(cloud.size());
    QString PointSize = QString("%1").arg(size);

    ui->statusBar->showMessage("点云数量: "+PointSize);

    //移除窗口点云
    viewer->removeAllPointClouds();
    viewer->removeAllShapes();

    //点云设置
    viewer->addPointCloud(cloud.makeShared(), cloud_name[0]);

    //设置点云大小
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, point_size, cloud_name[0]);
    viewer->resetCamera();
    ui->qvtkwidget->update();
}

// 五、地面去除
void MainWindow::on_actionNoGround_triggered()
{
    if (cloud.empty()) {
        QMessageBox::warning(this, "Warning", "当前无点云！");
        return;
    }

    QProgressDialog progressDialog("正在进行中", "取消", 0, 0, this);
    progressDialog.setWindowTitle("提示");
    progressDialog.setWindowModality(Qt::WindowModal);
    progressDialog.show();

    QFutureWatcher<void> watcher;
    MainWindow::on_NoGroud();


}

// 六、平面切割
void MainWindow::on_actionCutGround_triggered()
{
    if (cloud.empty()) {
        QMessageBox::warning(this, "Warning", "当前无点云！");
        return;
    }

    dialog_cut = new CutPlane();
    connect(dialog_cut, SIGNAL(sendData(QString, QString)), this, SLOT(Cut(QString, QString)));
    if (dialog_cut->exec() == QDialog::Accepted) {};
    delete dialog_cut;
}

// 七、点云缩放
void MainWindow::on_actionvoxel_triggered()
{
    if (cloud.empty()) {
        QMessageBox::warning(this, "Warning", "当前无点云！");
        return;
    }

    MainWindow::PointVoxel();
}

// 八、线性提取
void MainWindow::on_actionLinear_triggered()
{
    if (cloud.empty()) {
        QMessageBox::warning(this, "Warning", "当前无点云！");
        return;
    }

    MainWindow::Linear();
}

// 九、确定范围
void MainWindow::on_actionCheck_triggered()
{
    if (cloud.empty()) {
        QMessageBox::warning(this, "Warning", "当前无点云！");
        return;
    }

    linecloud = nul;
    linecloud = *clicked_points_3d;

    std::cout << linecloud.size() << std::endl;
}

// 十、选择模式
void MainWindow::on_actionSelectMode_triggered()
{
    if (cloud.empty()) {
        QMessageBox::warning(this, "Warning", "当前无点云！");
        return;
    }

    select = !select;
}

// 十一、分割展示
void MainWindow::on_actionDbscan_triggered()
{
    if (cloud.empty()) {
        QMessageBox::warning(this, "Warning", "当前无点云！");
        return;
    }

    dbscan_arg = new Dbscan_arg();
    connect(dbscan_arg, SIGNAL(sendData(QString, QString)), this, SLOT(show_dbscan(QString, QString)));
    if (dbscan_arg->exec() == QDialog::Accepted) {};
    delete dbscan_arg;
}

void MainWindow::show_dbscan(QString data1, QString data2) {
    dbscan_esp = data1.toDouble();
    dbscan_pts = data2.toDouble();
    std::cout << "esp: " << dbscan_esp << std::endl;
    std::cout << "pts: " << dbscan_pts << std::endl;
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in(new pcl::PointCloud<pcl::PointXYZ>);
    cloud_in = linecloud.makeShared();
    std::vector<pcl::Indices> cluster_indices;
    dbscan(*cloud_in, cluster_indices, dbscan_esp, dbscan_pts); // 2表示聚类的领域距离为2米，50表示聚类的最小点数。

    int begin = 1;
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr null_(new pcl::PointCloud<pcl::PointXYZRGB>);
    dbscan_all_cloud = null_;
    for (std::vector<pcl::Indices>::const_iterator it = cluster_indices.begin(); it != cluster_indices.end(); ++it)
    {
        // 获取每一个聚类点云团的点
        pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_dbscan(new pcl::PointCloud<pcl::PointXYZRGB>);

        // 同一点云团赋上同一种颜色
        uint8_t R = rand() % (256) + 0;
        uint8_t G = rand() % (256) + 0;
        uint8_t B = rand() % (256) + 0;

        for (auto pit = it->begin(); pit != it->end(); ++pit) {
            pcl::PointXYZRGB point_db;
            point_db.x = cloud_in->points[*pit].x;
            point_db.y = cloud_in->points[*pit].y;
            point_db.z = cloud_in->points[*pit].z;
            point_db.r = R;
            point_db.g = G;
            point_db.b = B;
            cloud_dbscan->points.push_back(point_db);
        }

        std::cout << begin << ": " << cloud_dbscan->size() << std::endl;
        *dbscan_all_cloud += *cloud_dbscan;
        begin ++;
    }

//    pcl::visualization::PCLVisualizer::Ptr view(new pcl::visualization::PCLVisualizer("Visulization Window"));
//    view->addPointCloud(dbscan_all_cloud);
//    view->spin();

    viewer->removeAllPointClouds();
    viewer->removeAllShapes();

    //点云设置
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> red(clicked_points_3d, 255, 0, 0);
    viewer->addPointCloud(cloud.makeShared(), cloud_name[0]);
    viewer->addPointCloud(dbscan_all_cloud, cloud_name[1]);
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, point_size, cloud_name[0]);
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, point_size, cloud_name[1]);
    viewer->resetCamera();
    ui->qvtkwidget->update();
}

void MainWindow::on_actionFix_triggered()
{
    if (selected_cloud->empty()) {
        QMessageBox::warning(this, "Warning", "当前无点云！");
        return;
    }

    fix_arg = new Fix_arg();
    connect(fix_arg, SIGNAL(sendData(QString)), this, SLOT(Fixline(QString)));
    if (fix_arg->exec() == QDialog::Accepted) {};
    delete fix_arg;
}

void MainWindow::Fixline(QString data)
{
    std::vector<double>x_mean, y_mean, z_mean;
    pcl::PointCloud<pcl::PointXYZ>::Ptr point_mean(new pcl::PointCloud<pcl::PointXYZ>);
    /*参数设置：分辨率 曲线个数*/
    double x_resolution = 2.0;
    double y_resolution = -1;
    int num_line = data.toInt();

    Fixing fixs(x_resolution, y_resolution,num_line);
    fixs.setinputcloud(selected_cloud);
    fixs.grid_mean_xyz(x_mean, y_mean, z_mean, point_mean);
    std::cout<<"The points size of  point_mean is :  "<<point_mean->points.size()<<std::endl;
    //求出曲线的端点
    std::vector<pcl::PointXYZ> endPoints;
    fixs.getEndPoints(point_mean,endPoints);
    //插值
    pcl::PointCloud<pcl::PointXYZ>::Ptr inter_pc(new pcl::PointCloud<pcl::PointXYZ>);
    fixs.getInterPoints(endPoints,inter_pc);
    std::cout<<inter_pc->points.size()<<std::endl;

    //可视化
    fixs.display(selected_cloud);
    fixs.display(inter_pc);
    pcl::PointCloud<pcl::PointXYZ>::Ptr nu(new pcl::PointCloud<pcl::PointXYZ>);
    selected_cloud = nu;
}

// 十二、kml导入
void MainWindow::on_actionPlus_triggered() {

    if (cloud.empty()) {
        QMessageBox::warning(this, "Warning", "当前无点云！");
        return;
    }

    const char* declaration = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    XMLError ret = doc.Parse(declaration);

    XMLElement* kml = doc.NewElement("kml");

    XMLElement* Document = doc.NewElement("Document");

    XMLElement* name = doc.NewElement("name");
    XMLText* nameText = doc.NewText("test");    // filename.toStdString ().c_str()

    XMLElement* Style = doc.NewElement("Style");
    XMLElement* LineStyle = doc.NewElement("LineStyle");
    XMLElement* color = doc.NewElement("color");
    XMLText* colorText = doc.NewText("ffffff00");
    XMLElement* width = doc.NewElement("width");
    XMLText* widthText = doc.NewText("3");

    if(!kml_num) {
        std::cout << "add First kml" << std::endl;
        color->InsertEndChild(colorText);
        width->InsertEndChild(widthText);
        LineStyle->InsertEndChild(color);
        LineStyle->InsertEndChild(width);
        Style->InsertEndChild(LineStyle);

        doc.InsertEndChild(kml);
        kml->InsertFirstChild(Document);
        name->InsertEndChild(nameText);
        Style->SetAttribute("id", "Line");
        Document->InsertEndChild(name);
        Document->InsertEndChild(Style);
    }

    Converter converter;
    std::string exe_path = "D:\\gdal\\bin";
    converter.init_epsg(32649, (exe_path + "\\gdal_data").c_str(), (exe_path + "\\proj").c_str());
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in(new pcl::PointCloud<pcl::PointXYZ>);
    cloud_in = linecloud.makeShared();
    std::vector<pcl::Indices> cluster_indices;
    dbscan(*cloud_in, cluster_indices, dbscan_esp, dbscan_pts); // 2表示聚类的领域距离为2米，50表示聚类的最小点数。

    int begin = 1;
    pcl::PointCloud<pcl::PointXYZ>::Ptr dbscan_all_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    for (std::vector<pcl::Indices>::const_iterator it = cluster_indices.begin(); it != cluster_indices.end(); ++it) {
        // 获取每一个聚类点云团的点
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_tem(new pcl::PointCloud<pcl::PointXYZ>);
        for (auto pit = it->begin(); pit != it->end(); ++pit) {
            pcl::PointXYZ point;
            pcl::PointXYZ point_db;
            // cout << *pit << endl;
            point_db.x = cloud_in->points[*pit].x;
            point_db.y = cloud_in->points[*pit].y;
            point_db.z = cloud_in->points[*pit].z;
            cloud_tem->points.push_back(point_db);
        }

        vector<double>x_mean, y_mean, z_mean;
        PointCloud<PointXYZ>::Ptr point_mean(new PointCloud<PointXYZ>);

        curve_fitting fit;
        fit.setinputcloud(cloud_tem);
        //前两个参数可调，表示x y方向的栅格大小 -1表示该方向不划分栅格
        fit.grid_mean_xyz(1.5, -1, x_mean, y_mean, z_mean, point_mean);
//        fit.grid_mean_xyz_display(point_mean);

        std::string point_kml;
        point_kml += "\n";
        for (int i = 0; i < point_mean->size(); i ++) {
            point_kml += "\t\t\t\t";
            // point_kml += "-" + std::to_string(cloud.points[i].x) + "," + std::to_string(cloud.points[i].y) + "," + std::to_string(cloud.points[i].z) + "\n";
            double coord[3] = {point_mean->points[i].x, point_mean->points[i].y, point_mean->points[i].z};
            if (converter.convert(coord)) {
                point_kml += std::to_string(coord[0]) + "," + std::to_string(coord[1]) + "," + std::to_string(coord[2]) + "\n";
            }
        }
        point_kml += "\t\t\t\t";

        XMLElement* Placemark = doc.NewElement("Placemark");
        XMLElement* name_ = doc.NewElement("name");
        XMLElement* visibility = doc.NewElement("visibility");
        XMLText* visibilityText = doc.NewText("0");
        XMLElement* styleUrl = doc.NewElement("styleUrl");
        XMLText* styleUrlText = doc.NewText("Line");
        XMLElement* LineString = doc.NewElement("LineString");
        XMLElement* altitudeMode = doc.NewElement("altitudeMode");
        XMLText* altitudeModeText = doc.NewText("absolute");
        XMLElement* coordinates = doc.NewElement("coordinates");
        XMLText* coordinatesText = doc.NewText(point_kml.c_str());

        visibility->InsertEndChild(visibilityText);
        styleUrl->InsertEndChild(styleUrlText);
        altitudeMode->InsertEndChild(altitudeModeText);
        coordinates->InsertEndChild(coordinatesText);

        LineString->InsertEndChild(altitudeMode);
        LineString->InsertEndChild(altitudeMode);
        LineString->InsertEndChild(coordinates);

        Placemark->InsertEndChild(name_);
        Placemark->InsertEndChild(visibility);
        Placemark->InsertEndChild(styleUrl);
        Placemark->InsertEndChild(LineString);
        Document->InsertEndChild(Placemark);

        *dbscan_all_cloud += *cloud_tem;

        begin++;
    }

    kml_num ++;
}

// 十三、显示切换
void MainWindow::on_actionExchange_triggered()
{
    if (clicked_points_3d->empty()) {
        QMessageBox::warning(this, "Warning", "未框选！");
        return;
    }

    QMessageBox msgBox(this);
    msgBox.setWindowTitle(tr("Confirm"));
    msgBox.setText(tr("是否确定保留框选点云？"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setButtonText(QMessageBox::Yes, tr("确定"));
    msgBox.setButtonText(QMessageBox::No, tr("取消"));
    int reply = msgBox.exec();

    if (reply == QMessageBox::Yes) {
        cloud_ = cloud;
        cloud = *clicked_points_3d;

        // 显示点云数量
        int size = static_cast<int>(cloud.size());
        QString PointSize = QString("%1").arg(size);
        ui->statusBar->showMessage("点云数量: "+PointSize);

        //移除窗口点云
        viewer->removeAllPointClouds();
        viewer->removeAllShapes();

        //点云设置
        pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> red(clicked_points_3d, 255, 0, 0);
        viewer->addPointCloud(clicked_points_3d, cloud_name[0]);
        viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, point_size, cloud_name[0]);
        viewer->resetCamera();
        ui->qvtkwidget->update();

        // 初始化 pointmark
        pointmark = new unsigned char[clicked_points_3d->size()];
        memset(pointmark, 0, cloud.size() * sizeof(unsigned char));
    } else {
        // 如果用户点击了“取消”，则什么都不做
        return;
    }
}

// 点云框选
void MainWindow::pp_callback(const pcl::visualization::AreaPickingEvent& event, void* args)
{
    if (!select) {

        std::vector<int> indices;
        if (event.getPointsIndices(indices) == -1) return;
        //添加点云（去掉重复的点）
        int addcount = 0;//添加点计数
        long bt = clock();//开始时间
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloudPtr = cloud.makeShared();

        if (pointmark == nullptr) {
            pointmark = new unsigned char[cloudPtr->size()];
            memset(pointmark, 0, cloudPtr->size() * sizeof(unsigned char));
        }

        for (int i = 0; i < indices.size(); ++ i) {
            if (pointmark[indices[i]] == 0) {
                clicked_points_3d->points.push_back(cloudPtr->points.at(indices[i]));
                addcount ++;
                pointmark[indices[i]] = 1;//将点云标记为已圈选状态
            }
        }

        long et = clock();//结束时间
        std::cout << "花费时间:" << et - bt << "ms" << std::endl;
        std::cout << "添加点数:" << addcount << std::endl;

        clicked_points_3d->height = 1;
        clicked_points_3d->width += addcount;
        pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> red(clicked_points_3d, 255, 0, 0);

        //每添加一次点云都要取一次别名，不然只能选择一次
        std::stringstream ss;
        std::string cloudName;
        ss << num ++;
        ss >> cloudName;
        cloudName += "_cloudName";

        //显示点云
        viewer->addPointCloud(clicked_points_3d, red, cloudName);
        viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, cloudName);
    } else if (select) {
        std::vector<int> indices;
        if (event.getPointsIndices(indices) == -1) return;

        // 统计颜色出现次数
        std::map<std::tuple<uint8_t, uint8_t, uint8_t>, int> color_count;
        for (int i = 0; i < indices.size(); i++) {
            pcl::PointXYZRGB p = dbscan_all_cloud->points.at(indices[i]);
            std::tuple<uint8_t, uint8_t, uint8_t> color_tuple(p.r, p.g, p.b);
            if (color_count.find(color_tuple) == color_count.end()) {
                color_count[color_tuple] = 1;
            } else {
                color_count[color_tuple]++;
            }
        }

        // 找出出现次数最多的颜色
        std::tuple<uint8_t, uint8_t, uint8_t> max_color;
        int max_count = 0;
        for (auto it = color_count.begin(); it != color_count.end(); it++) {
            if (it->second > max_count) {
                max_count = it->second;
                max_color = it->first;
            }
        }

        uint8_t r = std::get<0>(max_color);
        uint8_t g = std::get<1>(max_color);
        uint8_t b = std::get<2>(max_color);
        std::cout << "Selected color: " << (int)r << ", " << (int)g << ", " << (int)b << std::endl;

        // 选择颜色相同的点云
        pcl::PointCloud<pcl::PointXYZ>::Ptr selected(new pcl::PointCloud<pcl::PointXYZ>);
        for (int i = 0; i < dbscan_all_cloud->size(); ++i) {
            pcl::PointXYZRGB p = dbscan_all_cloud->points[i];
            if (p.r == r && p.g == g && p.b == b) {
                pcl::PointXYZ q;
                q.x = p.x;
                q.y = p.y;
                q.z = p.z;
                selected->points.push_back(q);
            }
        }

        *selected_cloud += *selected;
        std::cout << "select: " << selected_cloud->size() << std:: endl;
    }
}





