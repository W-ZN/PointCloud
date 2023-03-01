#include <CSF.h>
#include "mainwindow.h"
#include "pcl_function.h"

using std::vector;

// 直通滤波
pcl::PointCloud<pcl::PointXYZ>::Ptr pcl_filter_passthrough(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in, float min, float max, QString axis, bool is_save)
{
    pcl::PassThrough<pcl::PointXYZ> pass;//设置滤波器对象
    //参数设置
    pass.setInputCloud(cloud_in);
    pass.setFilterFieldName(axis.toStdString());
    pass.setFilterLimits(min, max);//区间设置
    pass.setFilterLimitsNegative(is_save);//设置为保留还是去除（true是去除上述坐标范围内的点）

    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_out (new pcl::PointCloud<pcl::PointXYZ>()) ;
    pass.filter(*cloud_out);

    return cloud_out;
}

// 高斯滤波
pcl::PointCloud<pcl::PointXYZ>::Ptr pcl_filter_guass(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in, float paraA, float paraB, float paraC, float paraD)
{
    //设置卷积核
    pcl::filters::GaussianKernel<pcl::PointXYZ, pcl::PointXYZ>::Ptr kernel(new pcl::filters::GaussianKernel<pcl::PointXYZ, pcl::PointXYZ>);
    (*kernel).setSigma(paraA);
    (*kernel).setThresholdRelativeToSigma(paraB);
    //KdTree加速
    pcl::search::KdTree<pcl::PointXYZ>::Ptr kdtree(new pcl::search::KdTree<pcl::PointXYZ>);
    (*kdtree).setInputCloud(cloud_in);
    pcl::filters::Convolution3D <pcl::PointXYZ, pcl::PointXYZ, pcl::filters::GaussianKernel<pcl::PointXYZ, pcl::PointXYZ> > convolution;
    convolution.setKernel(*kernel);
    convolution.setInputCloud(cloud_in);
    convolution.setSearchMethod(kdtree);
    convolution.setRadiusSearch(paraC);     //radius代表点云扫面的半径，这里需要用户计算获得
    convolution.setNumberOfThreads(paraD);  //important! Set Thread number for openMP

    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_out(new pcl::PointCloud<pcl::PointXYZ>);
    convolution.convolve(*cloud_out);

    return cloud_out;
}

// 体素滤波
pcl::PointCloud<pcl::PointXYZ>::Ptr pcl_filter_voxel(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in, float leaf_size)
{
    pcl::VoxelGrid<pcl::PointXYZ> voxel_grid;
    Eigen::Vector4f leafsize{leaf_size, leaf_size, leaf_size, leaf_size};
    voxel_grid.setLeafSize(leafsize);
    voxel_grid.setMinimumPointsNumberPerVoxel(10);
    voxel_grid.setInputCloud(cloud_in);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_out (new pcl::PointCloud<pcl::PointXYZ> ()) ;
    voxel_grid.filter(*cloud_out);

    return cloud_out;
}

// CSF 布匹滤波
pcl::PointCloud<pcl::PointXYZ>::Ptr pcl_filter_csf(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in, float rigidness, float time_step)
{
        // -------------------------转换为CSF支持的数据---------------------------------
        vector<csf::Point> CSFcloud((cloud_in->size())); //定义具有cloud->size()个元素的向量
        for (size_t i = 0; i < cloud_in->points.size(); ++i)
        {
            CSFcloud[i].x = cloud_in->points[i].x;
            CSFcloud[i].y = cloud_in->points[i].y;
            CSFcloud[i].z = cloud_in->points[i].z;

        }

        CSF csf;
        csf.setPointCloud(CSFcloud);
        csf.params.bSloopSmooth = false;
        csf.params.cloth_resolution = 0.5;
        csf.params.rigidness = rigidness;
        csf.params.time_step = time_step;
        csf.params.class_threshold = 0.5;
        csf.params.interations = 500;

        pcl::Indices groundIndexes, offGroundIndexes;
        csf.do_filtering(groundIndexes, offGroundIndexes);

        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_out(new pcl::PointCloud<pcl::PointXYZ>);
        pcl::copyPointCloud(*cloud_in, offGroundIndexes, *cloud_out);

        return cloud_out;
}

// 线性特征提取
std::vector <int> powerLineSegment(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in, double max_dist, double threshould)
{

    // 建立kd-tree索引
    pcl::KdTreeFLANN<pcl::PointXYZ> kdtree;
    kdtree.setInputCloud(cloud_in);
    pcl::Indices r_indices;
    std::vector<float> neighbor_square_distance;

    std::vector<int>line_idx;

    for (size_t i = 0; i < cloud_in->points.size(); ++i)
    {
        // 如果半径max_dist邻域范围内有点，则进行计算
        if (kdtree.radiusSearch(cloud_in->points[i], max_dist, r_indices, neighbor_square_distance) > 2)
        {
            //---------------PCA计算每个点的特征值------------------------
            pcl::PCA<pcl::PointXYZ> pca;
            pca.setInputCloud(cloud_in);
            pca.setIndices(std::make_shared<const pcl::Indices>(r_indices));
            pca.getEigenValues(); // 获取特征值，特征值是按从大到小排列的
            double l1 = pca.getEigenValues()[0];
            double l2 = pca.getEigenValues()[1];
            double l3 = pca.getEigenValues()[2];
            //----------------计算每个点的线性特征------------------------
            double linear = (l1 - l2) / l1;//线性
            //----------------设置阈值提取电力线--------------------------
            if (linear > threshould)  line_idx.push_back(i);
        }
        else
        {
            //如果半径max_dist邻域范围内没有点，则跳过该点。
            continue;
        }

    }

    return line_idx;
}

