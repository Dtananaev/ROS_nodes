
#ifndef RELAY_TOPIC_H
#define	RELAY_TOPIC_H


#include "ros/ros.h"
#include "ros/callback_queue.h"
#include "std_msgs/String.h"
#include <string>
#include <iostream>
 #include <sstream>
#include <fstream>
#include <ros/package.h>

#include <cstdio>
#include "topic_tools/shape_shifter.h"
#include "topic_tools/parse.h"

typedef struct{
    std::string topicName;
    bool    is_advertised;
} topic;


class relayTopic{
public:


      std::vector<ros::Subscriber> subs; 
      std::vector<std::string> topics; 
      relayTopic();
     ~relayTopic();
      void subscribe(topic g_input_topic,std::string namesp, ros::NodeHandle nh);//generic subscriber
      void callback(const ros::MessageEvent<topic_tools::ShapeShifter>& msg_event, std::string& topic);//generic callback function which also create generic publisher
    ros::Publisher getPublisher(const std::string& topic,  boost::shared_ptr<topic_tools::ShapeShifter const> const &msg );
    ros::TransportHints g_th;
     bool g_advertised = false;
   
     ros::Publisher* g_publish;
    ros::Subscriber* g_sub;
    std::string g_output_topic;
   // std::string namesp_;

private:
    std::map<std::string, ros::Publisher> mPublishers;
     ros::NodeHandle *g_node = NULL;
     ros::NodeHandle n;
     std::string namesp_;
};

#endif	/* RELAY_TOPIC_H */

