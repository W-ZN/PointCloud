#include "curve_fitting.h"

curve_fitting::curve_fitting()
{

}
curve_fitting::~curve_fitting()
{
    cloud->clear();
}
void curve_fitting::setinputcloud(pcl::PointCloud<pcl::PointXYZ>::Ptr input_cloud)
{
    cloud = input_cloud;
    getMinMax3D(*input_cloud, point_min, point_max);
}

void curve_fitting::grid_mean_xyz(double x_resolution, double y_resolution, vector<double>&x_mean, vector<double> &y_mean, vector<double>&z_mean, pcl::PointCloud<pcl::PointXYZ>::Ptr &new_cloud)
{
    if (y_resolution <= 0)
    {
        y_resolution = point_max.y - point_min.y;
    }
    std::cout<<"X is :"<<point_max.x - point_min.x<<std::endl;
    std::cout<<"Y is :"<<point_max.y - point_min.y<<std::endl;
    if (x_resolution <= 0)
    {
        x_resolution = point_max.x - point_min.x;
    }
    int raster_rows, raster_cols;
    raster_rows = ceil((point_max.x - point_min.x) / x_resolution);
    std::cout<<"raster_rows is:   "<<raster_rows<<std::endl;
    raster_cols = ceil((point_max.y - point_min.y) / y_resolution);
    std::cout<<"raster_cols is:   "<<raster_cols<<std::endl;
    vector<int>idx_point;
    vector<vector<vector<double>>>row_col;
    vector<vector<double>>col_;
    vector<double>vector_4;
    vector_4.resize(4);
    col_.resize(raster_cols, vector_4);
    row_col.resize(raster_rows, col_);
    int point_num = cloud->size();
    for (int i_point = 0; i_point < point_num; i_point++)
    {
        int row_idx = ceil((cloud->points[i_point].x - point_min.x) / x_resolution) - 1;
        int col_idx = ceil((cloud->points[i_point].y - point_min.y) / y_resolution) - 1;
        if (row_idx < 0)
            row_idx = 0;
        if (col_idx < 0)
            col_idx = 0;
        row_col[row_idx][col_idx][0] += cloud->points[i_point].x;
        row_col[row_idx][col_idx][1] += cloud->points[i_point].y;
        row_col[row_idx][col_idx][2] += cloud->points[i_point].z;
        row_col[row_idx][col_idx][3] += 1;
    }
    pcl::PointXYZ point_mean_tem;
    for (int i_row = 0; i_row < row_col.size(); i_row++)
    {
        for (int i_col = 0; i_col < row_col[i_row].size(); i_col++)
        {
            if (row_col[i_row][i_col][3] != 0)
            {
                double x_mean_tem = row_col[i_row][i_col][0] / row_col[i_row][i_col][3];
                double y_mean_tem = row_col[i_row][i_col][1] / row_col[i_row][i_col][3];
                double z_mean_tem = row_col[i_row][i_col][2] / row_col[i_row][i_col][3];
                x_mean.push_back(x_mean_tem);
                y_mean.push_back(y_mean_tem);
                z_mean.push_back(z_mean_tem);
                point_mean_tem.x = x_mean_tem;
                point_mean_tem.y = y_mean_tem;
                point_mean_tem.z = z_mean_tem;
                new_cloud->push_back(point_mean_tem);
            }
        }
    }
}
void curve_fitting::grid_mean_xyz_display(pcl::PointCloud<pcl::PointXYZ>::Ptr new_cloud)
{
    pcl::visualization::PCLVisualizer::Ptr view(new pcl::visualization::PCLVisualizer("Visulization Window"));
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ>color_1(new_cloud, 255, 0, 0);
    view->addPointCloud(new_cloud, color_1, "11");
    view->setPointCloudRenderingProperties(visualization::PCL_VISUALIZER_POINT_SIZE, 5, "11");
    view->addPointCloud(cloud, "22");
    view->spin();
}
