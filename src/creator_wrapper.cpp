/* 
 * File:   creator_wrapper.cpp
 * Author: Vladislav Tananaev
 * 
 * Created on February 17, 2016, 8:29 AM
 */

#include <vector>
#include <map>
#include <memory>

#include <ros/publisher.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <interactive_markers/interactive_marker_server.h>
#include "graph_planner/creator_wrapper.h"

CreatorWrapper::CreatorWrapper() : nh_() {

    graph_pub_ = nh_.advertise<graph_planner::GraphStructure>("/graph", 0, true); //last option "true" is enables "latching" on a connection. When a connection is latched, the last message published is saved and automatically sent to any future subscribers that connect.
    graph_viz_pub_ = nh_.advertise<visualization_msgs::MarkerArray>("/graph_viz", 1);
    
    point_cmd_sub_ = nh_.subscribe <graph_planner::PointCmd> ("/point_cmd", 1, &CreatorWrapper::pointCmdCallback, this);
   // edge_cmd_sub_ = nh_.subscribe<graph_planner::EdgeCmd>("/edge_cmd", 1, &CreatorWrapper::edgeCmdCallback, this);

    marker_server_ = new interactive_markers::InteractiveMarkerServer("point_marker");
}

CreatorWrapper::~CreatorWrapper() {
}

void CreatorWrapper::init() {

    clear();

/*
    name_ = "stamina"; 
    folder_path_ = ros::package::getPath("navigation_rail"); 

    bool loaded = loadGraphFromFile(folder_path_, name_);
    if (loaded ) {

        pubGraph();

        //add interactive marker for nodes
        for (auto it = vertices_->begin(); it != vertices_->end(); it++) {
            addInteractiveMarker(it->first);
        }

    }
*/
  
}

void CreatorWrapper::pointCmdCallback(const graph_planner::PointCmd::ConstPtr& cmd) {
    std::cerr << "Got new cmd for point" << std::endl;

    // add the point to the graph
    if (cmd->cmd == PointCMD::new_point) {
        std::cerr << "Add new Point" << std::endl;

         int32_t point_key_id = addPoint(
                cmd->pointInfo.x,
                cmd->pointInfo.y,
                cmd->pointInfo.name);
        //interactive marker
        addInteractiveMarker(point_key_id);
    }


    // remove selected point
    if (cmd->cmd == PointCMD::del_point) { // change cmd value
        std::cerr << "Remove selected Point" << std::endl;
        removePoint(cmd->pointInfo.key_id);
        marker_server_->erase(std::to_string(cmd->pointInfo.key_id));
        marker_server_->applyChanges();
    }
/*
    // update values
    if (cmd->cmd == VertexCMD::update_waypoint) { // change cmd value
        std::cerr << "Update selected Vertex" << std::endl;
        vertexUpdateFromMsg(cmd->vertexInfo);
        //interactive marker
        geometry_msgs::Pose pose;
        pose.position.x = cmd->vertexInfo.x;
        pose.position.y = cmd->vertexInfo.y;
        pose.position.z = cmd->vertexInfo.factor / 10.0; //why divide on 10?
        pose.orientation = tf::createQuaternionMsgFromYaw(cmd->vertexInfo.theta);
        marker_server_->setPose(std::to_string(cmd->vertexInfo.key_id), pose);
        visualization_msgs::InteractiveMarker marker;
        marker_server_->get(std::to_string(cmd->vertexInfo.key_id), marker);
        marker.description = cmd->vertexInfo.name;
        marker_server_->insert(marker);
        marker_server_->applyChanges();
    }
*/
    pubGraph();
    pubRvizGraph();
}
/*
void RailWrapper::edgeCmdCallback(const navigation_rail::EdgeCmd::ConstPtr& cmd) {

    // add edge to the graph
    if (cmd->cmd == EdgeCMD::add_edge) {
        std::cerr << "Add new Edge" << std::endl;
        double weight = 1.0;
        std::cerr << "Motion: " << (int8_t) cmd->edgeInfo.motion << std::endl;
        addEdge(cmd->edgeInfo.from_vertex,
                cmd->edgeInfo.to_vertex,
                weight,
                cmd->edgeInfo.motion
                );
    }

    if (cmd->cmd == EdgeCMD::update_edge) {

        edgeUpateFromMsg(cmd->edgeInfo);
    }
    if (cmd->cmd == EdgeCMD::del_edge) {

        removeEdge(cmd->edgeInfo.key_id);
    }

    if (cmd->cmd == EdgeCMD::save_graph) {
        std::cerr << "Save graph" << std::endl;
        saveGraphToFile(folder_path_, name_);
    }

    pubGraph();
    pubRvizGraph();
}
*/
void CreatorWrapper::pubGraph() {

    graph_planner::GraphStructure msg;

    for (auto it = points_->begin(); it != points_->end(); it++) {
        graph_planner::Point point;
        point2msg(it->second.get(), it->first, &point);
        msg.pointList.push_back(point);

    }
/*
    for (auto it = edges_->begin(); it != edges_->end(); it++) {
        navigation_rail::Edge edge;
        edge2msg(it->second.get(), &edge);
        msg.edgeList.push_back(edge);
    }
*/
    graph_pub_.publish(msg);
}


void CreatorWrapper::point2msg(Point* in, int32_t key_id, graph_planner::Point* out) {
    GraphPoint* v_ptr = dynamic_cast<GraphPoint*> (in);
    out->key_id = key_id;
    out->x = v_ptr->x_;
    out->y = v_ptr->y_;
    out->name = v_ptr->name_;
}

void CreatorWrapper::pointUpdateFromMsg(graph_planner::Point v) {
    auto it = points_->find(v.key_id);
    if (it != points_->end()) {

        auto v_ptr = std::dynamic_pointer_cast<GraphPoint> (it->second);
        v_ptr->x_ = v.x;
        v_ptr->y_ = v.y;
        v_ptr->name_ = v.name;
    }
}
/*
void RailWrapper::edge2msg(Edge* in, navigation_rail::Edge* out) {
    out->from_vertex = in->from()->id();
    out->to_vertex = in->to()->id();
    out->key_id = in->id();
    RailEdge* e_ptr = dynamic_cast<RailEdge*> (in);
    out->motion = e_ptr->motion_;
}

void RailWrapper::edgeUpateFromMsg(navigation_rail::Edge e) {

    auto it = edges_->find(e.key_id);
    if (it != edges_->end()) {
        RailEdge* e_ptr = dynamic_cast<RailEdge*> (it->second.get());
        e_ptr->motion_ = e.motion;

        if (e_ptr->from()->id() != e.from_vertex) {
            edgeChangeFrom(e.key_id, e.from_vertex);
        }

        if (e_ptr->to()->id() != e.to_vertex) {
            edgeChangeTo(e.key_id, e.to_vertex);
        }
    }

}
*/


void CreatorWrapper::pubRvizGraph() {

    visualization_msgs::MarkerArray ma;
    visualization_msgs::Marker marker;

    marker.header.frame_id = "/map";
    marker.id = 0;
    marker.type = visualization_msgs::Marker::SPHERE;
    marker.action = visualization_msgs::Marker::ADD;
    marker.lifetime = ros::Duration(2);
    marker.pose.position.x = 0;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;
    marker.scale.x = 0.12;
    marker.scale.y = 0.12;
    marker.scale.z = 0.12;
    marker.color.a = 1.0;
    marker.color.r = 1.0;
    marker.color.g = 0.0;

    for (auto it = points_->begin(); it != points_->end(); it++) {
        marker.id++;
        marker.type = visualization_msgs::Marker::SPHERE;
        auto v = point2GraphPoint(it->second);

        marker.pose.position.x = v->x_;
        marker.pose.position.y = v->y_;
        marker.pose.position.z = 0;
        ma.markers.push_back(marker);
        marker.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
        marker.id++;
        marker.text = v->name_;
        marker.pose.position.x = v->x_ + v->x_ / 200;
        marker.pose.position.y = v->y_ + v->y_ / 200;
        marker.pose.position.z = 0;
        marker.scale.z = 0.25;
        ma.markers.push_back(marker);
    }

/*
    marker.type = visualization_msgs::Marker::ARROW;
    marker.pose.position.x = 0;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0;



    for (auto it = edges_->begin(); it != edges_->end(); it++) {
        marker.points.clear();
        marker.id++;
        marker.scale.x = 0.05;
        marker.scale.y = 0.15;
        marker.scale.z = 0.1;
        marker.color.a = 0.4;
        marker.color.r = 0.2;
        marker.color.g = 0.2;
        marker.color.b = 1.0;
        auto e = edge2RailEdge(it->second);
        geometry_msgs::Point p;
        auto from = vertex2RailVertex(e->from());
        auto to = vertex2RailVertex(e->to());
        p.x = from->x_;
        p.y = from->y_;
        p.z = 0;
        marker.points.push_back(p);
        p.x = to->x_;
         p.y = to->y_;
        p.z = 0;
        marker.points.push_back(p);
        ma.markers.push_back(marker);
        //add motion direction
        marker.points.clear();
        marker.id++;
        marker.scale.x = 0.07;
        marker.scale.y = 0.25;
        marker.scale.z = 0.10;
        marker.color.a = 1.0;
        marker.color.r = 0.0;
        marker.color.g = 0.0;
        marker.color.b = 1.0;
        //middle point in edge

        p.x = (from->x_ + to->x_) / 2;
        p.y = (from->y_ + to->y_) / 2;
        p.z = 0;
        marker.points.push_back(p);
        if (e->motion_ == motion::forward) {
            p.x = (p.x + to->x_) / 2;
            p.y = (p.y + to->y_) / 2;
            p.z = 0;
            marker.points.push_back(p);

        } else if (e->motion_ == motion::backward) {
            p.x = (p.x + from->x_) / 2;
            p.y = (p.y + from->y_) / 2;
            p.z = 0;
            marker.points.push_back(p);
        } else if (e->motion_ == motion::forward_backward) {
            geometry_msgs::Point t;
            //first arrow forward
            t.x = (p.x + to->x_) / 2;
            t.y = (p.y + to->y_) / 2;
            t.z = 0;
            marker.points.push_back(t);
            ma.markers.push_back(marker);
            marker.points.clear();
            marker.id++;
            //second arrow backward
            marker.points.push_back(p);
            t.x = (p.x + from->x_) / 2;
            t.y = (p.y + from->y_) / 2;
            t.z = 0;
            marker.points.push_back(t);
        }
        ma.markers.push_back(marker);
    }
*/
    graph_viz_pub_.publish(ma);
}

/*
void RailWrapper::pubRvizPath() {

    if (graphPath_ == nullptr) {
        return;
    }
    //todo later graph as real path
    visualization_msgs::MarkerArray ma;
    visualization_msgs::Marker marker;

    marker.header.frame_id = "/map";
    marker.id = 0;
    marker.type = visualization_msgs::Marker::SPHERE;
    marker.action = visualization_msgs::Marker::ADD;
    marker.lifetime = ros::Duration(2);
    marker.pose.position.x = 0;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;
    marker.scale.x = 0.1;
    marker.scale.y = 0.1;
    marker.scale.z = 0.1;
    marker.color.a = 1.0;
    marker.color.r = 0.0;
    marker.color.g = 1.0;

    for (int i = 0; i < graphPath_->vertex_ptrs_.size(); i++) {
        marker.id++;
        auto it = vertices_->find(graphPath_->vertex_ptrs_[i]->id());
        auto v = vertex2RailVertex(it->second);
        marker.pose.position.x = v->x_;
        marker.pose.position.y = v->y_;
        marker.pose.position.z = 0;
        ma.markers.push_back(marker);
    }

    marker.type = visualization_msgs::Marker::ARROW;
    marker.pose.position.x = 0;
    marker.pose.position.y = 0;
    marker.pose.position.z = 0;
    marker.scale.x = 0.08;
    marker.scale.y = 0.15;
    marker.scale.z = 0.2;
    marker.color.a = 0.6;
    marker.color.r = 0.5;
    marker.color.g = 0.0;
    marker.color.b = 0.5;

    for (int i = 0; i < graphPath_->edge_ptrs_.size(); i++) {
        marker.points.clear();
        marker.id++;
        auto it = edges_->find(graphPath_->edge_ptrs_[i]->id());
        auto e = edge2RailEdge(it->second);
        geometry_msgs::Point p;
        auto v = vertex2RailVertex(e->from());
        p.x = v->x_;
        p.y = v->y_;
        p.z = 0;
        marker.points.push_back(p);
        v = vertex2RailVertex(e->to());
        p.x = v->x_;
        p.y = v->y_;
        p.z = 0;
        marker.points.push_back(p);
        ma.markers.push_back(marker);
    }

    graph_path_viz_pub_.publish(ma);
}
*/

void CreatorWrapper::addInteractiveMarker(PointKey id) {

    std::shared_ptr<GraphPoint> point_ptr = getPoint(id);

    // add interactive marker to marker server
    // create an interactive marker for our server
    visualization_msgs::InteractiveMarker int_marker;
    int_marker.header.frame_id = "map";
    int_marker.name = std::to_string(id);
    marker_name_2_pointkey_[std::to_string(id)] = id;
    int_marker.description = point_ptr->name_;
    int_marker.pose.position.x = point_ptr->x_;
    int_marker.pose.position.y = point_ptr->y_;
   // int_marker.pose.position.z = 0.15;
    //int_marker.pose.orientation = tf::createQuaternionMsgFromYaw(vertex_ptr->theta_);

/*
    // create a grey box marker
    visualization_msgs::Marker box_marker;
    box_marker.type = visualization_msgs::Marker::ARROW;
    box_marker.scale.x = 0.4;
    box_marker.scale.y = 0.05;
    box_marker.scale.z = 0.05;
    box_marker.color.r = 0.9;
    box_marker.color.g = 0.0;
    box_marker.color.b = 0.0;
    box_marker.color.a = 1.0;


    // create a non-interactive control which contains the box
    visualization_msgs::InteractiveMarkerControl box_control;
    box_control.always_visible = true;
    box_control.markers.push_back(box_marker);

    // add the control to the interactive marker
    int_marker.controls.push_back(box_control);
*/
    // create a control which will move the box
    // this control does not contain any markers,
    // which will cause RViz to insert two arrows
    visualization_msgs::InteractiveMarkerControl control;


    control.orientation.w = 1;
    control.orientation.x = 1;
    control.orientation.y = 0;
    control.orientation.z = 0;
 
    control.name = "move_x";
    control.interaction_mode = visualization_msgs::InteractiveMarkerControl::MOVE_AXIS;

    control.orientation.w = 1;
    control.orientation.x = 0;
    control.orientation.y = 1;
    control.orientation.z = 0;
    control.name = "rotate_z";
    control.interaction_mode = visualization_msgs::InteractiveMarkerControl::ROTATE_AXIS;
    //int_marker.controls.push_back(control);
    control.name = "move_z";
    control.interaction_mode = visualization_msgs::InteractiveMarkerControl::MOVE_AXIS;
    //int_marker.controls.push_back(control);

    control.orientation.w = 1;
    control.orientation.x = 0;
    control.orientation.y = 0;
    control.orientation.z = 1;
    control.name = "move_y";
    control.interaction_mode = visualization_msgs::InteractiveMarkerControl::MOVE_AXIS;


    control.orientation.w = 1;
    control.orientation.x = 0;
    control.orientation.y = 1;
    control.orientation.z = 0;
    control.interaction_mode = visualization_msgs::InteractiveMarkerControl::MOVE_ROTATE;
    int_marker.controls.push_back(control);
   // control.interaction_mode = visualization_msgs::InteractiveMarkerControl::MOVE_AXIS;
    //int_marker.controls.push_back(control);



    // add the interactive marker to our collection &
    // tell the server to call processFeedback() when feedback arrives for it
    marker_server_->insert(int_marker, boost::bind(&CreatorWrapper::processMarkerFedback, this, _1));

    // 'commit' changes and send to all clients
    marker_server_->applyChanges();


}


void CreatorWrapper::processMarkerFedback(const visualization_msgs::InteractiveMarkerFeedbackConstPtr& feedback) {

    unsigned int id = marker_name_2_pointkey_[feedback->marker_name];

    auto it = points_->find(id);
    if (it != points_->end()) {

        auto v_ptr = std::dynamic_pointer_cast<GraphPoint> (it->second);
        v_ptr->x_ = feedback->pose.position.x;
        v_ptr->y_ = feedback->pose.position.y;
    }
    pubGraph();
    pubRvizGraph();
}



void CreatorWrapper::spinOnes() {

    //pubRvizPath(); // changes this function so that it only pulished the current path
   // pubRvizQuery();

}




