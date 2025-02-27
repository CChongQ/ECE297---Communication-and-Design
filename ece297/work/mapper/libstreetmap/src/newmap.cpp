#include "m1.h"
#include "newmap.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h"
#include <map>
#include <tuple>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <unordered_map>
#include <vector>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <limits>


newmap::newmap() {
    streetToSegID.resize(getNumberOfStreets());
    getssIndexfromintersectionIndex.resize(getNumberOfIntersections());

    unsigned i;
    LatLon sth;//LatLon(position) of intersections 
    LatLon sth2;//LatLon(position) of point of interests

    for (i = 0; i < getNumberOfStreets(); i++) {
        std::string street_name = getStreetName(i);
        streetUOM[street_name].push_back(i);
        streeNAME.push_back(street_name);
    }
    
    for (i = 0; i < getNumberOfStreetSegments(); i++) {
        StreetSegmentInfo temp_info = getStreetSegmentInfo(i);
        
        unsigned totalCount = temp_info.curvePointCount;
        LatLon point1 = getIntersectionPosition(temp_info.from);
        LatLon point2 = getIntersectionPosition(temp_info.to);
        double distance = 0;

        if (totalCount == 0) {
            // this street segment doesn't have curve point
            distance = find_distance_between_two_points(point1, point2);
        }
        else {
            // the street have curve points 
            //basic idea: distance = start point to 1st curve point + 1st curve point to (totalCount-1)th curve point + (totalCount-1)th curve point to the end point     
            for (unsigned j = 0; j <= (totalCount - 1); j++) { // curve points index:0 to n-1
                LatLon curvePoint = getStreetSegmentCurvePoint(i, j); //position of ith curve point
                if (j == 0) {
                    //start point to 1st curve point
                    distance = distance + find_distance_between_two_points(point1, curvePoint);
                }
                else {
                    //1st curve point to (totalCount-1)th curve point
                    LatLon tempPoint = getStreetSegmentCurvePoint(i, j - 1); //position of (i-1)th curve point
                    distance = distance + find_distance_between_two_points(tempPoint, curvePoint);
                }
            }
            //(totalCount-1)th curve point to the end point
            LatLon curvePoint = getStreetSegmentCurvePoint(i, totalCount - 1);
            distance = distance + find_distance_between_two_points(curvePoint, point2);
        }

        float speed_limit = (temp_info.speedLimit) * pow(10, 3) / 3600; // unit:km/h -> m/s  
        double time = distance / speed_limit;

        travel_time.push_back(time);
        segmentLength.push_back(distance);

        streetSegmentUOM.push_back(getStreetSegmentInfo(i));
        streetToSegID[temp_info.streetID].push_back(i);
        
        ssidtoOSMID[i] = temp_info.wayOSMID;
   
        
        
    }
    
    for (i = 0; i < getNumberOfIntersections(); i++) {
        //transfer LatLon to a and b
        sth = getIntersectionPosition(i);
        double a = sth.lat();
        double b = sth.lon();

        intersectionUOM.push_back(geteintersectionInfo(i));
        countstreetsegment.push_back(getIntersectionStreetSegmentCount(i));

        for (unsigned int count = 0; count < getIntersectionStreetSegmentCount(i); count++) {
            getssIndexfromintersectionIndex[i].push_back(getIntersectionStreetSegment(i, count));
        }
        //construct RTree for the last function
        intersectionRTree.insert(std::make_pair(apoint(b, a), i));
    }
    
    //get location information and ids of all point of interests, insert thenm into an Rtree
    for (i = 0; i < getNumberOfPointsOfInterest(); i++) {
        sth2 = getPointOfInterestPosition(i);
        double a = sth2.lat();
        double b = sth2.lon();
        pointofInterestRTree.insert(std::make_pair(apoint(b, a), i)); 
      
    } 
    for(i = 0; i< getNumberOfWays(); i++){
        osmidToindex[getWayByIndex(i)->id()] = i;
        
    }
    
    unsigned num_OSMWay= getNumberOfWays();
    for(i = 0; i < num_OSMWay; i++){
        OSMID way_index = getWayByIndex(i)->id();
        OSM_Way[way_index] = i;
    }
    
    unsigned num_OSMNode = getNumberOfNodes();
    for(i = 0; i < num_OSMNode; i++){
        OSMID node_index = getNodeByIndex(i)->id();
        OSM_Node[node_index] = i;
    }
    
    
    
}

newmap::~newmap() {
    streetUOM.clear();
    streetSegmentUOM.clear();
    intersectionUOM.clear();
    segmentLength.clear();
    travel_time.clear();
}

intersectionInfo geteintersectionInfo(IntersectionIndex intersectionIdx) {
    intersectionInfo myintersectionInfo = {getIntersectionName(intersectionIdx), getIntersectionPosition(intersectionIdx), getIntersectionOSMNodeID(intersectionIdx)};
    return myintersectionInfo;
    }