#ifndef RELAY_CONFIG_H
#define RELAY_CONFIG_H

#include <string>
#include <vector>
#include <map>

#include <ros/callback_queue.h>
#include <ros/ros.h>

#include <topic_tools/parse.h>
#include <topic_tools/shape_shifter.h>

#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>

#include <std_msgs/Empty.h>

using std::string;
using std::vector;
using std::move;

class TfTransform
{
public:
  TfTransform(string& from, string& to) : from(from), to(to){};
  TfTransform(TfTransform&& other) : from(move(other.from)), to(move(other.to)){};
  virtual ~TfTransform(){};

public:
  string from;
  string to;
};

class RelayTopicConfig
{
public:
  RelayTopicConfig(){};
  virtual ~RelayTopicConfig(){};

  virtual const vector<string>& getListTopicToSub() = 0;
  virtual const vector<string>& getListTopicToPub() = 0;

private:
  virtual void setupTopicsList() = 0;
  virtual void addTopicPrefix(const string& prefix) = 0;
  virtual void stripTopicPrefix(const string& prefix) = 0;
  virtual void addTopicSuffix(const string& suffix) = 0;
  virtual void stripTopicSuffix(const string& suffix) = 0;
};

class HostRelayTopicConfig : public RelayTopicConfig
{
public:
  HostRelayTopicConfig();
  HostRelayTopicConfig(ros::NodeHandle& multimaster_nh);
  virtual ~HostRelayTopicConfig(){};

  virtual const vector<string>& getListTopicToSub() override;
  virtual const vector<string>& getListTopicToPub() override;

private:
  virtual void setupTopicsList() override;
  virtual void addTopicPrefix(const string& prefix) override;
  virtual void stripTopicPrefix(const string& prefix) override;
  virtual void addTopicSuffix(const string& suffix) override;
  virtual void stripTopicSuffix(const string& suffix) override;

private:
  vector<string> topic_list_to_subscribe;
  vector<string> topic_list_to_publish;
  ros::NodeHandle config_pnh;
};

class ForeignRelayTopicConfig : public RelayTopicConfig
{
public:
  ForeignRelayTopicConfig();
  ForeignRelayTopicConfig(ros::NodeHandle& multimaster_nh);
  virtual ~ForeignRelayTopicConfig(){};

  virtual const vector<string>& getListTopicToSub() override;
  virtual const vector<string>& getListTopicToPub() override;

private:
  virtual void setupTopicsList() override;
  virtual void addTopicPrefix(const string& prefix) override;
  virtual void stripTopicPrefix(const string& prefix) override;
  virtual void addTopicSuffix(const string& suffix) override;
  virtual void stripTopicSuffix(const string& suffix) override;

private:
  vector<string> topic_list_to_subscribe;
  vector<string> topic_list_to_publish;
  ros::NodeHandle config_pnh;
};

class RelayTFConfig
{
public:
  RelayTFConfig(){};
  virtual ~RelayTFConfig(){};

  virtual const vector<TfTransform>& getTFList() = 0;

private:
  virtual void setupTFList() = 0;
};

class HostRelayTFConfig : public RelayTFConfig
{
public:
  HostRelayTFConfig();
  HostRelayTFConfig(ros::NodeHandle& multimaster_nh);
  virtual ~HostRelayTFConfig(){};

  virtual const vector<TfTransform>& getTFList() override;

private:
  virtual void setupTFList() override;

private:
  vector<TfTransform> tf_list;
  ros::NodeHandle config_pnh;
};

class ForeignRelayTFConfig : public RelayTFConfig
{
public:
  ForeignRelayTFConfig();
  ForeignRelayTFConfig(ros::NodeHandle& multimaster_nh);
  virtual ~ForeignRelayTFConfig(){};

  virtual const vector<TfTransform>& getTFList() override;

private:
  virtual void setupTFList() override;

private:
  vector<TfTransform> tf_list;
  ros::NodeHandle config_pnh;
};

#endif /* CONFIG_RELAY_H */