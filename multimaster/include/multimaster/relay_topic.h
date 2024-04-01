#ifndef RELAY_TOPIC_H
#define RELAY_TOPIC_H

#include <multimaster/relay_config.h>

using boost::shared_ptr;
using std::map;

class RelayTopic
{
public:
  RelayTopic(RelayTopicConfig* cfg) : relay_nh(), cfg_(cfg){};
  ~RelayTopic(){};

  void subscribe();

private:
  // for internal use only
  bool isLatchTopic(shared_ptr<const ros::M_string> const& connection_header);
  void callback(const ros::MessageEvent<topic_tools::ShapeShifter>& msg_event, string& topic);
  ros::Publisher setupPublisher(const string& topic, shared_ptr<topic_tools::ShapeShifter const> const& msg,
                                shared_ptr<const ros::M_string> const& connection_header);

private:
  map<string, ros::Publisher> mPublishers;
  vector<ros::Subscriber> vSubscribers;  // hold subscribe
  ros::NodeHandle relay_nh;
  RelayTopicConfig* cfg_;
};

#endif /* RELAY_TOPIC_H */