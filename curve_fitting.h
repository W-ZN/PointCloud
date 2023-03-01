#include <vector>
#include <Eigen/Dense>
#include <pcl/point_types.h>
#include <vtkPolyLine.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/visualization/pcl_plotter.h>
#include <pcl/common/common.h>

using namespace std;
using namespace pcl;
using namespace Eigen;
typedef PointXYZ PointT;

class curve_fitting
{
public:
    curve_fitting();
    ~curve_fitting();
    void setinputcloud(pcl::PointCloud<pcl::PointXYZ>::Ptr input_cloud);
    void grid_mean_xyz(double x_resolution, double y_resolution, vector<double>&x_mean, vector<double> &y_mean, vector<double>&z_mean, pcl::PointCloud<pcl::PointXYZ>::Ptr &new_cloud);
    void grid_mean_xyz_display(pcl::PointCloud<PointXYZ>::Ptr new_cloud);
private:
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud;
    pcl::PointXYZ point_min;
    pcl::PointXYZ point_max;
};
