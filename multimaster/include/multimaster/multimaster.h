/*
 * File: multimaster
 * Author: Denis Tananaev
 * Date: 23.02.2017
 *
 */

#ifndef MULTIMASTER_H
#define MULTIMASTER_H

#include <multimaster/relay_manager.h>

namespace ros
{
namespace master
{
void init(const M_string& remappings);
}
}  // namespace ros

class Multimaster
{
public:
  Multimaster(){};
  virtual ~Multimaster(){};

  virtual string get_foreign_master_uri() = 0;

  virtual void establish_connection() = 0;
  virtual void switch_to_host() = 0;
  virtual void switch_to_foreign() = 0;

private:
  virtual void setupTimekeeper() = 0;
  virtual void setupParam() = 0;
};

class HFMultimaster : public Multimaster
{
public:
  HFMultimaster();
  virtual ~HFMultimaster();

  virtual string get_foreign_master_uri() override;

  virtual void establish_connection() override;
  virtual void switch_to_host() override;
  virtual void switch_to_foreign() override;

private:
  void init(ros::M_string remappings);
  virtual void setupTimekeeper() override;
  virtual void setupParam() override;

private:
  ros::M_string remappings;
  string foreign_master, host_master;
  double msgs_pub_freq;
  ros::NodeHandle nh, pnh;
  RelayTopicManager* manager;
  RelayTFManager* tf_manager;
  Timekeeper* tkeep;
};

class FHMultimaster : public Multimaster
{
public:
  FHMultimaster();
  virtual ~FHMultimaster();

  virtual string get_foreign_master_uri() override;

  virtual void establish_connection() override;
  virtual void switch_to_host() override;
  virtual void switch_to_foreign() override;

private:
  void init(ros::M_string remappings);
  virtual void setupTimekeeper() override;
  virtual void setupParam() override;

private:
  ros::M_string remappings;
  string foreign_master, host_master;
  double msgs_pub_freq;
  ros::NodeHandle nh, pnh;
  RelayTopicManager* manager;
  RelayTFManager* tf_manager;
  Timekeeper* tkeep;
};

#endif /* MULTIMASTER_H */