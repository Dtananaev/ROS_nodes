#ifndef RELAY_MANAGER_H
#define RELAY_MANAGER_H

#include <multimaster/relay_tf.h>
#include <multimaster/relay_topic.h>
#include <thread>

class RelayTopicManager
{
public:
  RelayTopicManager(){};
  virtual ~RelayTopicManager(){};

  virtual void setupConfig() = 0;
  virtual void setupConfig(ros::NodeHandle& multimaster_nh) = 0;
  virtual void connectToMaster() = 0;
  virtual void spin(double rate) = 0;
};

class HostRelayTopicManager : public RelayTopicManager
{
public:
  HostRelayTopicManager(){};
  virtual ~HostRelayTopicManager();

  virtual void setupConfig() override;
  virtual void setupConfig(ros::NodeHandle& multimaster_nh) override;
  virtual void connectToMaster() override;
  virtual void spin(double rate) override;

private:
  RelayTopicConfig* topic_cfg;
  RelayTopic* topic_connection;
};

class ForeignRelayTopicManager : public RelayTopicManager
{
public:
  ForeignRelayTopicManager(){};
  virtual ~ForeignRelayTopicManager();

  virtual void setupConfig() override;
  virtual void setupConfig(ros::NodeHandle& multimaster_nh) override;
  virtual void connectToMaster() override;
  virtual void spin(double rate) override;

private:
  RelayTopicConfig* topic_cfg;
  RelayTopic* topic_connection;
};

class TimeTFManager;

class RelayTFManager
{
public:
  RelayTFManager(){};
  virtual ~RelayTFManager(){};

  virtual void setupConfig() = 0;
  virtual void setupConfig(ros::NodeHandle& multimaster_nh) = 0;
  virtual void connectToMaster() = 0;
  virtual void spin(double rate, ros::Duration time) = 0;
};

class HostRelayTFManager : public RelayTFManager
{
public:
  HostRelayTFManager(){};
  virtual ~HostRelayTFManager();

  virtual void setupConfig() override;
  virtual void setupConfig(ros::NodeHandle& multimaster_nh) override;
  virtual void connectToMaster() override;
  virtual void spin(double rate, ros::Duration time) override;

private:
  RelayTFConfig* tf_cfg;
  RelayTF* tf_connection;
};

class ForeignRelayTFManager : public RelayTFManager
{
public:
  ForeignRelayTFManager(){};
  virtual ~ForeignRelayTFManager();

  virtual void setupConfig() override;
  virtual void setupConfig(ros::NodeHandle& multimaster_nh) override;
  virtual void connectToMaster() override;
  virtual void spin(double rate, ros::Duration time) override;

private:
  RelayTFConfig* tf_cfg;
  RelayTF* tf_connection;
};

class Timekeeper
{
public:
  Timekeeper(){};
  ~Timekeeper(){};

  void setHostTime();
  void setForeignTime();
  ros::Duration difference();

private:
  ros::Time host_time;
  ros::Time foreign_time;
};

#endif /* RELAY_MANAGER_H */