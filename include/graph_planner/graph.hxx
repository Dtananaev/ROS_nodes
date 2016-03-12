/* 
 * File:   graph.cpp
 * Author: Tananaev Denis
 * 
 * Created on January 12, 2016
 */

#include "graph.hpp"
#include <algorithm>

//using namespace Eigen;

template<class T>
Graph<T>::Graph() {
    initialized_ = false;
}

template<class T>
Graph<T>::~Graph() {
}

template<class T>
void Graph<T>::init() {
    clear();
    initialized_ = true;
}

template<class T>
void Graph<T>::clear() {

    edges_.reset(new EdgeSet);
    points_.reset(new PointSet);
}

template<class T>
bool Graph<T>::addPoint(PointKey k, PointPtr v) {
    auto it = points_->find(k);
    if (it != points_->end()) {
        return false;
    }

    points_->insert(std::make_pair(k, v));
    return true;
}

template<class T>
bool Graph<T>::removePoint(PointKey k) {
    auto it = points_->find(k);
    if (it ==points_->end()) {
        return false;
    }
/*
    const EdgeCollection edges = it->second->edges();
    //it->second->edges().clear();

    for (auto e : edges) {
        removeEdge(e.first);
    }
*/
    points_->erase(it);
}





