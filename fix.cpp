#include "fix.h"

Fixing::Fixing(double x_resolution, double y_resolution, int num_line):x_resolution_(x_resolution),y_resolution_(y_resolution),num_line_(num_line){}

Fixing::~Fixing()
{
    cloud->clear();
}
void Fixing::setinputcloud(pcl::PointCloud<pcl::PointXYZ>::Ptr input_cloud)
{
    cloud = input_cloud;
    getMinMax3D(*input_cloud, point_min, point_max);
}

void Fixing::grid_mean_xyz(vector<double>&x_mean, vector<double> &y_mean, vector<double>&z_mean, pcl::PointCloud<pcl::PointXYZ>::Ptr &new_cloud)
{
    if (y_resolution_ <= 0)
    {
        y_resolution_ = point_max.y - point_min.y;
    }
    std::cout<<"X is :"<<point_max.x - point_min.x<<std::endl;
    std::cout<<"Y is :"<<point_max.y - point_min.y<<std::endl;
    if (x_resolution_ <= 0)
    {
        x_resolution_ = point_max.x - point_min.x;
    }
    int raster_rows, raster_cols;
    raster_rows = ceil((point_max.x - point_min.x) / x_resolution_);
    std::cout<<"raster_rows is:   "<<raster_rows<<std::endl;
    raster_cols = ceil((point_max.y - point_min.y) / y_resolution_);
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
        int row_idx = ceil((cloud->points[i_point].x - point_min.x) / x_resolution_) - 1;
        int col_idx = ceil((cloud->points[i_point].y - point_min.y) / y_resolution_) - 1;
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

void Fixing::display(pcl::PointCloud<pcl::PointXYZ>::Ptr new_cloud)
{
    pcl::visualization::PCLVisualizer::Ptr view(new pcl::visualization::PCLVisualizer("Visulization Window"));
    pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ>color_1(new_cloud, 255, 0, 0);
    view->addPointCloud(new_cloud, color_1, "11");
    view->setPointCloudRenderingProperties(visualization::PCL_VISUALIZER_POINT_SIZE, 3, "11");
    view->addPointCloud(cloud, "22");
    view->spin();
}

void Fixing::getEndPoints(pcl::PointCloud<pcl::PointXYZ>::Ptr input_cloud,std::vector<pcl::PointXYZ> &endPoints)
{
    //sort align x
    std::sort(input_cloud->points.begin(), input_cloud->points.end(),
                    [](pcl::PointXYZ p1, pcl::PointXYZ p2){
                        return p1.x < p2.x;
                    }
    );
    pcl::PointXYZ min_point = input_cloud->points.front();
    pcl::PointXYZ max_point = input_cloud->points.back();
    //getEndPoints
    for(std::size_t i = 0; i < input_cloud->points.size(); i++){
        if(i > 0 && i < input_cloud->points.size() -2){
            pcl::PointXYZ points_left = input_cloud->points[i - 1];
            pcl::PointXYZ points_current = input_cloud->points[i];
            pcl::PointXYZ points_right = input_cloud->points[i + 1];
            double diff_l = points_current.x - points_left.x;
            double diff_r = points_right.x - points_current.x;
            if(diff_r > 2 * x_resolution_){
                pcl::PointXYZ right_end = input_cloud->points[i];
                endPoints.push_back(right_end);
            }
            if(diff_l > 2 * x_resolution_){
                pcl::PointXYZ left_end = input_cloud->points[i];
                endPoints.push_back(left_end);
            }
        }
    }
    std::cout<<"The endPoints size is :    "<<endPoints.size()<<std::endl;
}

void Fixing::getInterPoints(std::vector<pcl::PointXYZ> endPoints,pcl::PointCloud<pcl::PointXYZ>::Ptr output)
{
    for(int i = 0; i < endPoints.size(); i+=2){
        Eigen::Vector3d P1(endPoints[i].x,endPoints[i].y,endPoints[i].z);
        Eigen::Vector3d P2(endPoints[i+1].x,endPoints[i+1].y,endPoints[i+1].z);
        Eigen::Vector3d dir = P2 - P1;//方向向量
        double dx = dir[0];
        double dy = dir[1];
        double dz = dir[2];
        double t = 1.0 / dir.norm();
        std::cout<< "dir is "<< dir[0]<<"    "<<dir[1]<<"   "<<dir[2]<<std::endl;
        double end_point = dx + 5 * x_resolution_;
        double interval = 0.1;
        for (int j = 0; j < int(end_point / interval ); j++){
            double x = endPoints[i].x + dx * t * interval * j;
            double y = endPoints[i].y + dy * t * interval * j;
            double z = endPoints[i].z  + dz * t * interval * j;

            pcl::PointXYZ pt;
            pt.x = x;
            pt.y = y;
            pt.z = z;
            output->points.push_back(pt);
        }
    }
}
