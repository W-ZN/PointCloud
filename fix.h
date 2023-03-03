#ifndef FIX_H
#define FIX_H
#include <vector>
#include <Eigen/Dense>
#include<algorithm>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/point_types.h>
#include <vtkPolyLine.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/visualization/pcl_plotter.h>
#include <pcl/common/common.h>
#include <pcl/features/boundary.h>

using namespace std;
using namespace pcl;
using namespace Eigen;
typedef PointXYZ PointT;

class Fixing
{
public:
    Fixing(double x_resolution, double y_resolution, int num_line);
    ~Fixing();
    void setinputcloud(pcl::PointCloud<pcl::PointXYZ>::Ptr input_cloud);
    void grid_mean_xyz(vector<double>&x_mean, vector<double> &y_mean, vector<double>&z_mean, pcl::PointCloud<pcl::PointXYZ>::Ptr &new_cloud);
    void display(pcl::PointCloud<PointXYZ>::Ptr new_cloud);
    void getEndPoints(pcl::PointCloud<pcl::PointXYZ>::Ptr input_cloud,std::vector<pcl::PointXYZ> &endPoints);
    void getInterPoints(std::vector<pcl::PointXYZ> endPoints, pcl::PointCloud<pcl::PointXYZ>::Ptr output);
private:
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud;
    double x_resolution_;
    double y_resolution_;
    pcl::PointXYZ point_min;
    pcl::PointXYZ point_max;
    int num_line_;//曲线个数
};

#endif // FIX_H
