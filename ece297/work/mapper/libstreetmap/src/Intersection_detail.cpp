/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Nodes.cpp
 * Author: qinzhen4
 * 
 * Created on March 16, 2018, 10:39 PM
 */

#include "Intersection_detail.h"
#include "StreetsDatabaseAPI.h"
#include "m2.h"
#include "m1.h"


Intersection_detail::Intersection_detail(){
    visited=false;
    total_time_spend=std::numeric_limits<double>::max();
    previousIntersectionID=getNumberOfIntersections();
}

Intersection_detail::~Intersection_detail() {
}


