#include <thread>
#include <mutex>
#include <multimaster/relay_tf.h>

RelayTF::~RelayTF()
{
  delete buffer_;
  delete listener_;
  delete broadcaster_;
}

void RelayTF::setListener(tf2_ros::Buffer* buffer, tf2_ros::TransformListener* listener)
{
  buffer_ = buffer;
  listener_ = listener;
}

void RelayTF::setBroadcaster(tf2_ros::TransformBroadcaster* broadcaster)
{
  broadcaster_ = broadcaster;
}

void RelayTF::listen(double rate, ros::Duration time)
{
  // ros::Duration(5).sleep();
  ros::Rate loop_rate(rate);
  while (ros::ok())
  {
    try
    {
      for (const auto& tf_name : cfg_->getTFList())
      {
        geometry_msgs::TransformStamped transformStamped;
        transformStamped = buffer_->lookupTransform(tf_name.from, tf_name.to, ros::Time::now() - time);
        transformStamped.header.stamp = ros::Time::now();
        broadcaster_->sendTransform(transformStamped);
      }
    }
    catch (tf::TransformException ex)
    {
      ROS_ERROR("%s", ex.what());
    }
    loop_rate.sleep();
  }
}