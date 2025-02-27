#ifndef NEWMAP_H
#define NEWMAP_H

#include <string>
#include <vector>
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
#include "graphics.h"
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<double,2,bg::cs::spherical_equatorial<bg::degree>> apoint; // since the unit of LatLon is degree, use spherical coordinate system in the following code 
typedef std::pair<apoint, unsigned> apair; //make position and ids (of intersections or point of interests) a pair

//struct to hold all intersection information
struct intersectionInfo {
    std::string intersectionName1;
    LatLon intersectionPosition1;
    OSMID intersectionOSMID1;
};

//return a intersection struct based on input intersection index
intersectionInfo geteintersectionInfo(IntersectionIndex intersectionIdx);


//create the class of the map data structure
class newmap {
public:
    newmap();
    ~newmap();

    
    // (unordered map) that stores the OSM way index for each OSM id for every street segment before you start looking for the tags.
    std::unordered_map<StreetSegmentIndex,OSMID> ssidtoOSMID; //get OSMID of street segments by street segment index
    std::unordered_map<OSMID, unsigned> osmidToindex;//get OSMWay index by OSMID
    std::vector<std::vector<std::vector<t_point>>> subway_Coordinate;
    std::unordered_map<OSMID, unsigned>OSM_Way;
    std::unordered_map<OSMID, unsigned>OSM_Node;
    std::unordered_map<std::string, std::vector<unsigned int>> streetUOM; //store the name of street as key, the index of street as value
    std::vector<std::string> streeNAME;
    std::vector<StreetSegmentInfo> streetSegmentUOM;//vector to hold street segment info, use street segment index as key
    std::vector<intersectionInfo> intersectionUOM;//vector to hold intersection info, use intersection index as key
    std::vector<double> segmentLength;//vector to hold length of street segment
    std::vector<double> travel_time;//vector to hold travelling time
    std::vector<std::vector<unsigned int>> streetToSegID;//vector to hold street segment id from given street id
    std::vector<unsigned int> countstreetsegment;//vector to hold number of streetsegment of an intersection
    std::vector<std::vector<unsigned int>> getssIndexfromintersectionIndex;//vector to hold street segment index from given intersection index

    bgi::rtree<apair, bgi::quadratic < 16 >> intersectionRTree;//create a Rtree for intersection
    bgi::rtree<apair, bgi::quadratic < 16 >> pointofInterestRTree; //create a Rtree for point of interests
    
};

#endif /* NEWMAP_H */

