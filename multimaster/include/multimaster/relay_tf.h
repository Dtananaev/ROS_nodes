#ifndef RELAY_TF_H
#define RELAY_TF_H

#include <multimaster/relay_config.h>

using boost::shared_ptr;
using std::map;

class RelayTF
{
public:
  RelayTF(RelayTFConfig* cfg) : relay_nh_(), cfg_(cfg){};
  ~RelayTF();

  void listen(double rate, ros::Duration time);
  void setListener(tf2_ros::Buffer* buffer, tf2_ros::TransformListener* listener);
  void setBroadcaster(tf2_ros::TransformBroadcaster* broadcaster);

private:
  tf2_ros::Buffer* buffer_;
  tf2_ros::TransformListener* listener_;
  tf2_ros::TransformBroadcaster* broadcaster_;
  RelayTFConfig* cfg_;
  ros::NodeHandle relay_nh_;
};

#endif /* RELAY_TF_H */