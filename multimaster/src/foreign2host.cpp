/*
 *
 *
 *  Created on: Februar 23.02.2017
 *      Author: Denis Tananaev
 */

#include <multimaster/multimaster.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "foreign2host");  // init ROS

  Multimaster* mmaster = new FHMultimaster();

  mmaster->switch_to_foreign();  // remap to the foreign master

  // check if foreign master work
  if (ros::master::check() == false)
  {
    ROS_ERROR("DISCONNECTED FROM THE ROS_MASTER_URI := %s", mmaster->get_foreign_master_uri().c_str());
  }
  else
  {
    ROS_INFO("CONNECTED TO THE ROS_MASTER_URI := %s", mmaster->get_foreign_master_uri().c_str());
    mmaster->establish_connection();
  }

  delete mmaster;

  return 0;
}