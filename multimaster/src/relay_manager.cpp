#include <multimaster/relay_manager.h>

HostRelayTopicManager::~HostRelayTopicManager()
{
  delete topic_cfg;
  delete topic_connection;
}

ForeignRelayTopicManager::~ForeignRelayTopicManager()
{
  delete topic_cfg;
  delete topic_connection;
}

HostRelayTFManager::~HostRelayTFManager()
{
  delete tf_cfg;
  delete tf_connection;
}

ForeignRelayTFManager::~ForeignRelayTFManager()
{
  delete tf_cfg;
  delete tf_connection;
}

void HostRelayTopicManager::setupConfig()
{
  topic_cfg = new HostRelayTopicConfig();
}

void HostRelayTopicManager::setupConfig(ros::NodeHandle& multimaster_nh)
{
  topic_cfg = new HostRelayTopicConfig(multimaster_nh);
}

void ForeignRelayTopicManager::setupConfig()
{
  topic_cfg = new ForeignRelayTopicConfig();
}

void ForeignRelayTopicManager::setupConfig(ros::NodeHandle& multimaster_nh)
{
  topic_cfg = new ForeignRelayTopicConfig(multimaster_nh);
}

void HostRelayTFManager::setupConfig()
{
  tf_cfg = new HostRelayTFConfig();
}

void HostRelayTFManager::setupConfig(ros::NodeHandle& multimaster_nh)
{
  tf_cfg = new HostRelayTFConfig(multimaster_nh);
}

void ForeignRelayTFManager::setupConfig()
{
  tf_cfg = new ForeignRelayTFConfig();
}

void ForeignRelayTFManager::setupConfig(ros::NodeHandle& multimaster_nh)
{
  tf_cfg = new ForeignRelayTFConfig(multimaster_nh);
}

void HostRelayTopicManager::connectToMaster()
{
  topic_connection = new RelayTopic(topic_cfg);
  topic_connection->subscribe();
}

void ForeignRelayTopicManager::connectToMaster()
{
  topic_connection = new RelayTopic(topic_cfg);
  topic_connection->subscribe();
}

void HostRelayTFManager::connectToMaster()
{
  tf_connection = new RelayTF(tf_cfg);
  tf2_ros::Buffer* buffer = new tf2_ros::Buffer(ros::Duration(5.0));
  tf2_ros::TransformListener* listener = new tf2_ros::TransformListener(*buffer);
  tf_connection->setListener(buffer, listener);
}

void ForeignRelayTFManager::connectToMaster()
{
  tf_connection = new RelayTF(tf_cfg);
  tf2_ros::Buffer* buffer = new tf2_ros::Buffer(ros::Duration(5.0));
  tf2_ros::TransformListener* listener = new tf2_ros::TransformListener(*buffer);
  tf_connection->setListener(buffer, listener);
}

void HostRelayTopicManager::spin(double rate)
{
  ros::Rate loop_rate(rate);
  ros::Duration(0.5).sleep();
  while (ros::ok() && ros::master::check() == true)
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
}

void ForeignRelayTopicManager::spin(double rate)
{
  ros::Rate loop_rate(rate);
  ros::Duration(0.5).sleep();
  while (ros::ok() && ros::master::check() == true)
  {
    ros::spinOnce();
    loop_rate.sleep();
  }
}

void HostRelayTFManager::spin(double rate, ros::Duration diff)
{
  tf_connection->setBroadcaster(new tf2_ros::TransformBroadcaster);
  std::thread t([rate, diff, this] { this->tf_connection->listen(rate, diff); });
  t.detach();
}

void ForeignRelayTFManager::spin(double rate, ros::Duration diff)
{
  tf_connection->setBroadcaster(new tf2_ros::TransformBroadcaster);
  std::thread t([rate, diff, this] { this->tf_connection->listen(rate, diff); });
  t.detach();
}

void Timekeeper::setHostTime()
{
  host_time = ros::Time::now();
}

void Timekeeper::setForeignTime()
{
  foreign_time = ros::Time::now();
}

ros::Duration Timekeeper::difference()
{
  return ros::Duration(foreign_time - host_time);
}