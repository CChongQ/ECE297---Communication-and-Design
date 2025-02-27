
#include "newmap.h"
#include "m1.h"
#include "m2.h"
#include "Helper.h"
#include "Draw_functions.h" // contains all draw_map related function
#include "Information_bar.h" // contains all function releted to interface
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h" 
#include "graphics.h"
#include "Global.h"
#include "graphics_types.h"
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

Surface theatre = load_png_from_file("libstreetmap/resources/theatre.png");
Surface parking = load_png_from_file("libstreetmap/resources/parking.png");
Surface bank = load_png_from_file("libstreetmap/resources/bank.png");
Surface hospital = load_png_from_file("libstreetmap/resources/hospital.png");
Surface pharmacy = load_png_from_file("libstreetmap/resources/pharmacy.png");
Surface bus_station = load_png_from_file("libstreetmap/resources/bus_station.png");
Surface pub = load_png_from_file("libstreetmap/resources/pub.png");
Surface fuel = load_png_from_file("libstreetmap/resources/fuel.png");
Surface cinema = load_png_from_file("libstreetmap/resources/cinema.png");
Surface fire_station = load_png_from_file("libstreetmap/resources/fire_station.png");
Surface subway_entrance = load_png_from_file("libstreetmap/resources/subway_entrance.png");
Surface fast_food = load_png_from_file("libstreetmap/resources/fast_food.png");
Surface cafe = load_png_from_file("libstreetmap/resources/cafe.png");
Surface restaurant = load_png_from_file("libstreetmap/resources/restaurant.png");

void draw_map() {
   //std::cout <<"Draw map!" <<std::endl;
    /**************** load data into the data struct vectors **********************/
    //get max and min lat
    double max_lat = getIntersectionPosition(0).lat();
    double min_lat = max_lat;

    //get max and min lon
    double max_lon = getIntersectionPosition(0).lon();
    double min_lon = max_lon;

    //load intersection info
    // intersections.resize(getNumberOfIntersections());
    for (unsigned id = 0; id < getNumberOfIntersections(); id++) {
        // intersections[id].position = getIntersectionPosition(id);
        //  intersections[id].name = getIntersectionName(id);

        max_lat = std::max(max_lat, (mymap->intersectionUOM[id]).intersectionPosition1.lat()); // compares two num and finds max between the 2 and then put it into max_lat
        min_lat = std::min(min_lat, (mymap->intersectionUOM[id]).intersectionPosition1.lat());

        max_lon = std::max(max_lon, (mymap->intersectionUOM[id]).intersectionPosition1.lon());
        min_lon = std::min(min_lon, (mymap->intersectionUOM[id]).intersectionPosition1.lon());
    }
    average_lat = 0.5 * DEG_TO_RAD * (max_lat + min_lat);

    // build an Rtree for intersections(for highlight) 

    //load ss info
    ss.resize(getNumberOfStreetSegments());
    for (unsigned ssid = 0; ssid < getNumberOfStreetSegments(); ssid++) {
        //ss[ssid].stsegInfo = getStreetSegmentInfo(ssid);
        //----------------load points info--------------------//
        ss[ssid].points.push_back(getIntersectionPosition(getStreetSegmentInfo(ssid).from));


        //----------------load curve points --------------------//
        unsigned curvePointCount = getStreetSegmentInfo(ssid).curvePointCount;
        for (unsigned i = 0; i < curvePointCount; i++) {
            ss[ssid].points.push_back(getStreetSegmentCurvePoint(ssid, i));
        }

        ss[ssid].points.push_back(getIntersectionPosition(getStreetSegmentInfo(ssid).to));

        ss[ssid].name = getStreetName(getStreetSegmentInfo(ssid).streetID);
    }

    //load POI info
    POIs.resize(getNumberOfPointsOfInterest());
    for (unsigned id = 0; id < getNumberOfPointsOfInterest(); id++) {
        POIs[id].position = getPointOfInterestPosition(id);
        POIs[id].name = getPointOfInterestName(id);
        POIs[id].type = getPointOfInterestType(id);
    }

    //load feature info
    
    for (unsigned id = 0; id < getNumberOfFeatures(); id++) {
        struct FeatureInfo feature;
        feature.name = getFeatureName(id);
        feature.type = getFeatureType(id);
        //----------------load feature points --------------------//
        unsigned featuresPointCount = getFeaturePointCount(id);
        for (unsigned i = 0; i < featuresPointCount; i++) {
            feature.points.push_back(getFeaturePoint(id, i));
        }

        feature.cartPoints = new t_point[getFeaturePointCount(id)];
        for (unsigned pointID = 0; pointID < getFeaturePointCount(id); pointID++) {
            feature.cartPoints[pointID].x = xcoord(feature.points[pointID].lon());
            feature.cartPoints[pointID].y = ycoord(feature.points[pointID].lat());
        }

        feature.area = 0.0;
        if (feature.cartPoints[0].x == feature.cartPoints[getFeaturePointCount(id)-1].x &&
                feature.cartPoints[0].y == feature.cartPoints[getFeaturePointCount(id)-1].y) {
            int l = 0;
            for (unsigned k = 1; k < getFeaturePointCount(id); k++) {
                feature.area += ((feature.cartPoints[l].x + feature.cartPoints[k].x) *
                        (feature.cartPoints[l].y - feature.cartPoints[k].y));
                l = k;
            }
            feature.area = abs(feature.area / 2.0);
        }

        std::pair<std::set<struct FeatureInfo, compareArea>::iterator,bool> insertSuccess = features.insert(feature);

        if (!(insertSuccess.second)) {
            delete [] feature.cartPoints;
        }
    }

    changeMapFunc = false;
    //load map names
    mapNames.push_back("cairo_egypt.png");
    mapNames.push_back("cape-town_south-africa.png");
    mapNames.push_back("iceland.png");
    mapNames.push_back("interlaken_switzerland.png");
    mapNames.push_back("london_england.png");
    mapNames.push_back("moscow_russia.png");
    mapNames.push_back("new-delhi_india.png");
    mapNames.push_back("new-york_usa.png");
    mapNames.push_back("rio-de-janeiro_brazil.png");
    mapNames.push_back("saint-helena.png");
    mapNames.push_back("singapore.png");
    mapNames.push_back("sydney_australia.png");
    mapNames.push_back("tehran_iran.png");
    mapNames.push_back("tokyo_japan.png");
    mapNames.push_back("beijing_china.png");
    mapNames.push_back("hong-kong_china.png");
    mapNames.push_back("golden-horseshoe_canada.png");
    mapNames.push_back("hamilton_canada.png");
    mapNames.push_back("toronto_canada.png");
    
    //load layer type names
    layerTypes.push_back("emergency services.png");
    layerTypes.push_back("entertainment.png");
    layerTypes.push_back("services.png");
    layerTypes.push_back("other.png");
    layerTypes.push_back("transportation.png");
    layerTypes.push_back("subway_layer.png");
    
    //load layer pressed vector
    for (int i = 0; i < 5; i++) {
        layer_pressed.push_back(true);
    } layer_pressed.push_back(false);
    
    //load layer type pressed names
    layerTypes_pressed.push_back("emergency services pressed.png");
    layerTypes_pressed.push_back("entertainment pressed.png");
    layerTypes_pressed.push_back("services pressed.png");
    layerTypes_pressed.push_back("other pressed.png");
    layerTypes_pressed.push_back("transportation pressed.png");
    layerTypes_pressed.push_back("subway_layer pressed.png");
//    /**************** load poi icon areas for act_mouse_move func. **********************/
//    t_point poiCenter;
//    for (unsigned i = 0; i < POIs.size(); i++) {
//        float tempx = (POIs[i].position.lon());
//        float tempy = (POIs[i].position.lat());
//
//        poiCenter.x = tempx;
//        poiCenter.y = tempy;
//
//        t_point poiCenter2 = world_to_scrn(poiCenter);
//
//        float left = poiCenter2.x - 0.000001;
//        float bottom = poiCenter2.y + 0.000001;
//        float right = poiCenter2.x + 0.000001;
//        float top = poiCenter2.y - 0.000001;
//
//        t_bound_box poiIcon(left, bottom, right, top);
//
//        poiIcons.push_back(poiIcon);
//    }
    /**************** set visible world values **********************/
    left = xcoord(min_lon);
    bottom = ycoord(min_lat);
    right = xcoord(max_lon);
    top = ycoord(max_lat);

    /**************** initialize display **********************/
    //area is the current area in the window
    area = (right - left)*(top - bottom);
    initial_coords = t_bound_box(left, bottom, right, top);
    std::cout << "About to start graphics.\n";

    if (!changedMap) mapTitle = "toronto_canada";

    // Set the name of the window (in UTF-8), with white background.
    init_graphics(mapTitle, t_color(237, 237, 237)); // you could pass a t_color RGB triplet instead


    set_drawing_buffer(OFF_SCREEN);
    /****Enable input**/
    set_keypress_input(true);
    set_mouse_move_input(true);
    line_entering_demo = true;
    
    // We'll first do double buffered drawing, where we draw to an off-screen
    // buffer and then quickly copy it to the screen to avoid flashes as we 
    // rapidly change the graphics when panning or zooming
    set_drawing_buffer(OFF_SCREEN);
    set_visible_world(initial_coords);
    // create_button("Search Location", "From: ", NULL);
//    create_button("Window", "Find", highlight_street_intersections);
    //create_button("Window", "Change Map", change_map);
    
    event_loop(act_on_mouse_click, act_on_mouse_move, act_on_keyboard, drawscreen);
    close_graphics();
    delete_map();
    std::cout << "Graphics closed down.\n";

}

void delete_map() {
    //clear vectors and sets
    pois.clear();
    inputString_s1.clear();
    inputString_s2.clear();
    click_pts.clear();
    inter_id_twoStreet.clear();
    ss.clear();
    POIs.clear();
    for (std::set<FeatureInfo, compareArea>::iterator it = features.begin(); it != features.end(); it++) {
        delete [] (*it).cartPoints;
    }
    features.clear();
    
    //clean strings
    inputString_s1.clear();
    outputString_s1.clear();
    inputString_s2.clear();
    outputString_s2.clear();
}

void change_map(std::string mapName_input) {
    changedMap = true;
    
    close_graphics();
    close_map();
    mapTitle = mapName_input;

    bool load_success = load_map("/cad2/ece297s/public/maps/" + mapName_input + ".streets.bin");
    delete_map();
    draw_map();
}
