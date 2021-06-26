#ifndef _NDT_MAPPING_
#define _NDT_MAPPING_

#include <nav_msgs/Odometry.h>
#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/PointCloud2.h>
#include <std_msgs/Float32.h>

#include <tf/transform_broadcaster.h>
#include <tf/transform_datatypes.h>

#include <pcl/filters/voxel_grid.h>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/registration/ndt.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl_ros/transforms.h>

#include <ndt_mapping/data_struct.h>

// odometry・IMU必須、通常実装のNDT使用
class NDTMapping
{
  using PointType = pcl::PointXYZI;

public:
  NDTMapping();
  ~NDTMapping() = default;

private:
  ros::NodeHandle nh_{};
  ros::NodeHandle pnh_{"~"};

  ros::Subscriber points_subscriber_;
  ros::Subscriber odom_subscriber_;
  ros::Subscriber imu_subscriber_;
  ros::Publisher ndt_map_publisher_;
  ros::Publisher ndt_pose_publisher_;
  ros::Publisher transform_probability_publisher_;

  Pose ndt_pose_;
  Pose previous_pose_;

  ros::Time current_scan_time_;
  ros::Time previous_scan_time_;

  //tf::TransformBroadcaster br_;
  tf2_ros::TransformBroadcaster br_;

  pcl::PointCloud<PointType> map_;
  pcl::NormalDistributionsTransform<PointType, PointType> ndt_;

  bool initial_scan_loaded_{true};
  bool is_first_map_{true};

  // rosparam
  double min_scan_range_;
  double max_scan_range_;
  double min_add_scan_shift_;

  // voxel grid filter
  double leaf_size_;

  // NDT config rosparam
  double trans_eps_;
  double step_size_;
  double ndt_res_;
  int max_iter_;

  sensor_msgs::Imu imu_;
  nav_msgs::Odometry odom_;

  // rosparam
  double tf_x_;
  double tf_y_;
  double tf_z_;
  double tf_roll_;
  double tf_pitch_;
  double tf_yaw_;
  Eigen::Matrix4f tf_btol_, tf_ltob_;  // base_link to sensor_link

  pcl::VoxelGrid<PointType> voxel_grid_filter_;

  void imuCorrect();

  void pointsCallback(const sensor_msgs::PointCloud2::ConstPtr & points);
  void odomCallback(const nav_msgs::Odometry::ConstPtr & msg);
  void imuCallback(const sensor_msgs::Imu::ConstPtr & msg);
};

#endif
