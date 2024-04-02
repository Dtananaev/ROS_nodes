/*
 * 
 *  License: BSD 2-Clause "Simplified" License
 *  Created on: Februar 23.02.2017
 *      Author: Denis Tananaev
 */

#include "multimaster/multimaster.h"


int main(int argc, char **argv){
    ros::M_string     remappings;

    //init ROS    
    ros::init(argc, argv,"main");
          

     ros::NodeHandle nh;  
    ros::Rate loop_rate_main(200);//check the connection to the master every 200 mc
     float foreign_master_works= false;//set by default that foreign master is turned off


    multimaster mmaster;
   if(mmaster.getParam()==false){
        return 0;
    }


    if(mmaster.getHostTopicsList()==false){
        return 0;
    }

    //remap to the foreign master 
    remappings["__master"] = mmaster.foreign_master_uri();
    ros::master::init(remappings);
   //ros::NodeHandle nn; 

    //first check
if (ros::master::check()==false){
    ROS_ERROR_STREAM("DISCONNECTED FROM THE ROS_MASTER_URI:= "<< mmaster.foreign_master_uri());
}

    while(ros::ok()){
        //check that master is working
        if(ros::master::check()==true && foreign_master_works==false){
            foreign_master_works=true;   
            ROS_INFO_STREAM("CONNECTED TO THE ROS_MASTER_URI:= "<<mmaster.foreign_master_uri());      
               
            mmaster.host2foreign(remappings);     
           
         } else if(ros::master::check()==false && foreign_master_works==true){
                foreign_master_works=false;
                ROS_ERROR_STREAM("DISCONNECTED FROM THE ROS_MASTER_URI:= "<<mmaster.foreign_master_uri());                    
               
           }
   loop_rate_main.sleep();
    }

    return 0;
}
