#ifndef FITTING_H
#define FITTING_H
#endif // FITTING_H

//fitting.h
#include <pcl/point_types.h>
#include <vector>
#include <Eigen/dense>
#include <vtkPolyLine.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/visualization/pcl_plotter.h>
#include <pcl/common/common.h>

using namespace std;
using namespace pcl;
using namespace Eigen;
typedef PointXYZ PointT;

struct  NPoint{
    double x;
    double y;
    double z;
};

class fitting
{
public:
    fitting();
    ~fitting();
    void setinputcloud(PointCloud<PointT>::Ptr input_cloud);//点云输入
    void grid_mean_xyz(double x_resolution,double y_resolution, vector<double>&x_mean, vector<double> &y_mean, vector<double>&z_mean, PointCloud<PointT>::Ptr &new_cloud);//投影至XOY，规则格网，求每个格网内点云坐标均值
    void grid_mean_xyz_display(PointCloud<PointT>::Ptr new_cloud);//均值结果三维展示
    void line_fitting(vector<double>x, vector<double>y, double &k, double &b);//y=kx+b
    void polynomial2D_fitting(vector<double>x, vector<double>y, double &a, double &b, double &c);//y=a*x^2+b*x+c;
    void polynomial3D_fitting(vector<double>x, vector<double>y, vector<double>z, double &a, double &b, double &c);//z=a*(x^2+y^2)+b*sqrt(x^2+y^2)+c
    void polynomial3D_fitting_display(double step_);//三维曲线展示
    void display_point(vector<double>vector_1,vector<double>vector_2);//散点图显示
    void display_line(vector<double>vector_1, vector<double>vector_2, double c, double b, double a = 0);//拟合的平面直线或曲线展示

private:
    PointCloud<PointT>::Ptr cloud;
    PointT point_min;
    PointT point_max;
    double a_3d;
    double b_3d;
    double c_3d;
    double k_line;
    double b_line;
};


