#ifndef PCL_FUNCTION_H
#define PCL_FUNCTION_H

#endif // PCL_FUNCTION_H

#include <iostream>
#include <vector>
#include <time.h>
#include <QString>

//-----------------------------pcl------------------------------------
/**********************************pcl格式*****************************/
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/common/pca.h>
#include <pcl/common/eigen.h>
#include <pcl/common/centroid.h>
#include <pcl/console/time.h>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/search/kdtree.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <boost/thread/thread.hpp>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/radius_outlier_removal.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <boost/thread/thread.hpp>
#include <pcl/visualization/common/common.h>

#include <pcl/filters/passthrough.h>  // 直通滤波
#include <pcl/filters/convolution_3d.h>  // 高斯滤波
#include <pcl/filters/voxel_grid.h>  // 体素滤波
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/extract_indices.h>
#include <CSF.h>
#include <pcl/sample_consensus/ransac.h>
#include <pcl/sample_consensus/sac_model_line.h>


/**********************************pcl显示***************************/
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/visualization/histogram_visualizer.h>
#include <pcl/visualization/pcl_plotter.h>

/**********************************pcl滤波***************************/
#include <laswriter.hpp>
#include <lasreader.hpp>

//------------------------------------------filter--------------------------------
pcl::PointCloud<pcl::PointXYZ>::Ptr pcl_filter_passthrough(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in, float min,float max, QString axis, bool is_save);
pcl::PointCloud<pcl::PointXYZ>::Ptr pcl_filter_guass(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in, float paraA, float paraB, float paraC, float paraD);
pcl::PointCloud<pcl::PointXYZ>::Ptr pcl_filter_voxel(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in, float leaf_size);
pcl::PointCloud<pcl::PointXYZ>::Ptr pcl_filter_csf(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in, float rigidness, float time_step);
std::vector <int> powerLineSegment(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in, double max_dist, double threshould);
