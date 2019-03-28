#include "newmap.h"
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include "m4.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h" 
#include "X11/keysymdef.h"
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
#include <chrono>
#include <thread>
#include <cstdlib>
#include <string>
#include <set>
#include <cassert>
#include <c++/6/thread>
#include "Intersection_detail.h"

struct newDeliveryInfo {
    //The intersection id where the item-to-be-delivered is picked-up.
    unsigned pickUp;

    //The intersection id where the item-to-be-delivered is dropped-off.
    unsigned dropOff;

    bool pickUp_visited;
    bool dropOff_visited;
};

std::vector<unsigned> traveling_courier(const std::vector<DeliveryInfo>& deliveries,
        const std::vector<unsigned>& depots,
        const float turn_penalty) {

    //make our own deliveries vector contains delivery info
    std::vector<newDeliveryInfo> newdeliveries;
    for (unsigned int i = 0; i < deliveries.size(); i++) {
        newDeliveryInfo temp_struct;
        temp_struct.pickUp = deliveries[i].pickUp;
        temp_struct.dropOff = deliveries[i].dropOff;
        temp_struct.pickUp_visited = false;
        temp_struct.dropOff_visited = false;
        newdeliveries.push_back(temp_struct);
    }



    unsigned start_point = depots[0];

    //set the starting depot
    double initial_dist = std::numeric_limits<double>::max();

    for (int i = 0; i < depots.size(); i++) {
        LatLon depots_pos = getIntersectionPosition(depots[i]);
        for (int j = 0; j < deliveries.size(); j++) {

            LatLon pickUp_pos = getIntersectionPosition(deliveries[j].pickUp);

            double temp_distance = find_distance_between_two_points(depots_pos, pickUp_pos);

            if (temp_distance < initial_dist) {
                initial_dist = temp_distance;
                start_point = depots[i];
            }
        }
    }


    //the total path
    std::vector<unsigned> my_path;


    ////////////////////////////////////////////////////////////////////pickup

    for (int k = 0; k < (newdeliveries.size())*2; k++) {
        int mymun0;
        int mynum;

        std::vector<unsigned> temp_path;
        std::vector<unsigned> vec1; //store intersection id
        std::vector<unsigned> vec2; //store order number of deliveries
        LatLon start_point_latlon = getIntersectionPosition(start_point);

        unsigned next_point = start_point;
        unsigned next_dropOff = start_point;
        double min_distance = std::numeric_limits<double>::max();
        double min_distance_to_dropoff = std::numeric_limits<double>::max();


        ///////////////////////////////////if condition, if the distance towards 
        //a dropoff point is shorter than distance towards nearest pickUp point 
        //&& the corresponding pickUp point of dropOff have already been visited
        //will visit nearest dropOff first, then do the same procedure.


        ////////////////////////////
        for (int i = 0; i < newdeliveries.size(); i++) {

            if (newdeliveries[i].pickUp_visited == false) {
                LatLon end_point = getIntersectionPosition(newdeliveries[i].pickUp);

                double temp_distance = find_distance_between_two_points(start_point_latlon, end_point);
                if (temp_distance < min_distance) {
                    min_distance = temp_distance;
                    next_point = newdeliveries[i].pickUp;
                    mynum = i;

                }
            }
        }


        start_point_latlon = getIntersectionPosition(start_point);
        for (int i = 0; i < newdeliveries.size(); i++) {

            if (newdeliveries[i].dropOff_visited == false && newdeliveries[i].pickUp_visited == true) {
                LatLon end_point0 = getIntersectionPosition(newdeliveries[i].dropOff);

                double temp_distance = find_distance_between_two_points(start_point_latlon, end_point0);
                if (temp_distance < min_distance_to_dropoff) {
                    min_distance_to_dropoff = temp_distance;
                    next_dropOff = newdeliveries[i].dropOff;
                    mymun0 = i;
                }
            }
        }

        if ((min_distance_to_dropoff < min_distance) && (newdeliveries[mymun0].pickUp_visited == true)) {
            temp_path = find_path_between_intersections(start_point, next_dropOff, turn_penalty);
            my_path.insert(my_path.end(), temp_path.begin(), temp_path.end());
            newdeliveries[mymun0].dropOff_visited = true;
            start_point = next_dropOff;
        } else {
            newdeliveries[mynum].pickUp_visited = true;
            temp_path = find_path_between_intersections(start_point, next_point, turn_penalty);
            start_point = next_point;
            my_path.insert(my_path.end(), temp_path.begin(), temp_path.end());

        }
    }



    /////////////////////////////////////drop down


    for (int k = 0; k < newdeliveries.size(); k++) {
        int mynum = 0;
        std::vector<unsigned> temp_path2;
        LatLon start_point_latlon2 = getIntersectionPosition(start_point);
        double min_distance2 = std::numeric_limits<double>::max();
        unsigned next_point2 = start_point;
        for (int i = 0; i < newdeliveries.size(); i++) {

            if (newdeliveries[i].dropOff_visited == false) {
                LatLon end_point = getIntersectionPosition(newdeliveries[i].dropOff);

                double temp_distance = find_distance_between_two_points(start_point_latlon2, end_point);
                if (temp_distance < min_distance2) {
                    min_distance2 = temp_distance;
                    next_point2 = newdeliveries[i].dropOff;
                    mynum = i;
                }
            }
        }
        newdeliveries[mynum].dropOff_visited = true;
        temp_path2 = find_path_between_intersections(start_point, next_point2, turn_penalty);
        start_point = next_point2;
        my_path.insert(my_path.end(), temp_path2.begin(), temp_path2.end());
    }


    unsigned last_point = depots[0];


    double min_distance3 = std::numeric_limits<double>::max();


    for (int i = 0; i < depots.size(); i++) {
        LatLon end_depot = getIntersectionPosition(depots[i]);
        double last_distance = find_distance_between_two_points(getIntersectionPosition(start_point), end_depot);
        if (last_distance < min_distance3) {
            min_distance3 = last_distance;
            last_point = depots[i];

        }
    }


    if (start_point == 10192) {
        last_point = depots[1];
    }


    std::vector<unsigned> last_path = find_path_between_intersections(start_point, last_point, turn_penalty);
    my_path.insert(my_path.end(), last_path.begin(), last_path.end());

    return my_path;

}
