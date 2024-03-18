/*
 *
 *
 *  Created on: March 30, 2016
 *      Author: Denis Tananaev
 */

#include <multimaster/relay_topic.h>

// If the input topic is latched, make the output topic latched.
bool RelayTopic::isLatchTopic(shared_ptr<const ros::M_string> const& connection_header)
{
  if (connection_header)
  {
    ros::M_string::const_iterator it = connection_header->find("latching");
    if ((it != connection_header->end()) && (it->second == "1"))
    {
      ROS_INFO("Input topic is latched; latching output topic to match");
      return true;
    }
  }

  return false;
}

ros::Publisher RelayTopic::setupPublisher(const string& topic,
                                          shared_ptr<topic_tools::ShapeShifter const> const& msg,
                                          shared_ptr<const ros::M_string> const& connection_header)
{
  if (mPublishers.find(topic) == mPublishers.end())
  {
    bool latch = isLatchTopic(connection_header);

    mPublishers[topic] = msg->advertise(relay_nh, topic, 10, latch);// advertise new topic
  }

  return mPublishers[topic];
}

void RelayTopic::callback(const ros::MessageEvent<topic_tools::ShapeShifter>& msg_event, string& topic)
{
  string publisher_name = msg_event.getPublisherName();

  shared_ptr<topic_tools::ShapeShifter const> const& msg = msg_event.getConstMessage();
  shared_ptr<const ros::M_string> const& connection_header = msg_event.getConnectionHeaderPtr();

  ros::Publisher publ = setupPublisher(cfg_->getNamespace() + "/" + topic, msg, connection_header);

  publ.publish(msg);
}

void RelayTopic::subscribe()
{
  if(cfg_ == nullptr)
  {
    ROS_ERROR("Config is not setup!");
    std::terminate();
  }
  for (const auto& topic : cfg_->getTopicsList())
  {
    ros::Subscriber subscriber =
        relay_nh.subscribe<topic_tools::ShapeShifter>(topic, 10, boost::bind(&RelayTopic::callback, this, _1, topic));

    ROS_INFO("Subscribing on %s topic %s", topic.c_str(), subscriber ? "success!" : "fail!");

    vSubscribers.push_back(subscriber);
  }
}