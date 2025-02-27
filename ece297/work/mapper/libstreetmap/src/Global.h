/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Global.h
 * Author: chosoo1
 *
 * Created on February 21, 2018, 12:56 PM
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include "m1.h"
#include "newmap.h"
#include "m2.h"
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


//-----from newmap------//
extern newmap *mymap; //make mymap a global variable to access mymap in different files

//-----from m2--------//
/*************Function prototypes*************/
void draw_streetSegment();
void change_map(std::string mapName_input);
void delete_map(); //clear variables and contents from current map
void draw_features();
void draw_POIs();
void drawSS(bool unknown, double degreeAngle, double degreeAngleOneWay, t_point& pt1, t_point& pt2, unsigned i);
void drawSearchBar();
void drawscreen(void);
void hightlight_intersection(unsigned inter_id);
void highlight_street_intersections();
void draw_highlight_intersections();
t_point coordinate_convert_inter(unsigned inter_id_intersection);
void Zoom_in_area();
std::string getFeatureType(FeatureType type);
t_point get_world_point(unsigned id);
void act_on_keyboard(char c, int keysym);
t_color getFeatureColor(FeatureType type);
void act_on_mouse_click(float x, float y, t_event_buttonPressed event);
void act_on_mouse_move(float x, float y);
void draw_string();

// unit conversion
float xcoord(double loncoord);
float ycoord(double latcoord);
float x_to_lon(float x);
float y_to_lat(float y);
void getValues();



/************************** Global variables **************************/
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
static bool line_entering_demo = false;
extern newmap *mymap;
std::string mapName;
t_bound_box initial_coords;
std::vector <std::string> pois;
double average_lat; //globally used for unit conversions
static std::vector<t_point> click_pts; // Stores the position
/*used to know which area was clicked, and what variable input string needs to go into
coordinate information of screen*/
double click_point;
double left;
double bottom;
double right;
double top;
double area;
unsigned inter_id; // intersection id of the closest intersection with a given point
POIIndex poi_id;
std::vector<unsigned> inter_id_twoStreet; //intersection id(s) of two streets
std::string mapTitle;
bool changedMap = false;
t_point for_search_zoom;
/************Position information************/
t_point on_poi_icon(float x, float y);

//points where user clicked on, used for check 
t_point user_clicked_world;
t_point user_clicked_screen;
//points where the user's mouse is
t_point user_mouse_world;
t_point user_mouse_screen;
//position of the nearest intersection found on map, in screen coordinate
t_point click_screen;
t_point temp_zoom;
//position of the nearest poi found on map, in screen coordinate
t_point click_screen_poi;
//position of a specific intersection on map, in screen coordinate
t_point inter_point_screen;

/******size information about information bar in screen coordinate******/
float bottom_l_y; //bottom left y
float top_r_x; // top right x
float screen_width;
float screen_height;
float screen_ratio;
//new
float bottom_l_x_left;
float bottom_l_x_right;
float bar_height_bottom;
float bar_height_top;
float bar_length;
float changeMap_icon_top_left_y;
float changeMap_icon_gap;
bool changeMapFunc;
float changeMapID;
float layer_icon_top_left_y;
float layer_icon_gap;

//change map name vector
std::vector<std::string> mapNames;
void print_changeMap_icons(int i, std::string name);
std::vector<t_bound_box> map_icon_boundaries;

std::vector<std::string> layerTypes;
void print_layer_icons(int i, std::string name);
std::vector<t_bound_box> layer_icon_boundaries;
std::vector<std::string> layerTypes_pressed;

float zoomWidth = 0.0001;

/***Enable variables to indicate that what will be print on screen***/
bool draw_enable;
bool user_type = false;
bool user_click_map;
bool user_enter_string_twice = false;
bool draw_inter_streets = false;
bool only_draw_string = false;
//which search bar was clicked
bool inS1 = false;
bool inS2 = false;
bool special = false;
//which tabs need to be opened
bool layer_opened = false;
bool changeMap_opened = false;
bool details_opened = false;

std::vector<bool> layer_pressed;

/*******User input strings*******/
std::vector<char> inputString_s1;
std::vector<char> inputString_s2;
std::string outputString_s1;
std::string outputString_s2;

/**************** data struct **********************/
struct ssInfo {
    std::vector <LatLon> points;
    std::string name;
};

struct POIInfo {
    std::string name;
    std::string type;
    LatLon position;
};

struct FeatureInfo {
    std::string name;
    FeatureType type;
    std::vector <LatLon> points;
    t_point* cartPoints;
    double area;
};

struct compareArea {

    bool operator()(const struct FeatureInfo & lhs, const struct FeatureInfo & rhs) {
        return (lhs.area > rhs.area);
    }
};

struct interIcon {
    float left;
    float bottom;
    float right;
    float top;
};




struct twoInterID {
    std::vector<unsigned> intersID;
    std::vector<std::string> inputIntersections;
    std::vector<std::string> POIsName;
    std::vector<t_point> intersWorldP;
    
    unsigned interOne;
    unsigned interTwo;
   // std::string POI;
    bool twoInput = false;
    t_point interOneW;
    t_point interTwoW;
    int which_is_POI = 0;

    void reset() {
        interOne = 0;
        interTwo = 0;
        twoInput = false;
        interOneW.x = 0;
        interOneW.y = 0;
        interTwoW.x = 0;
        interTwoW.y = 0;
       // which_is_POI = 0;
     //   POI.clear();
        
        intersID.clear();
        inputIntersections.clear();
        POIsName.clear();
        intersWorldP.clear();
    }

};

struct twoSS {
    t_point before;
    unsigned before_SID;
    t_point after;
    unsigned after_SID;
    std::string direction;
};
std::vector<ssInfo> ss;
std::vector<POIInfo> POIs;
std::set<struct FeatureInfo, compareArea> features;
twoInterID clickTwoInter;
//twoInterID foundInterID;
twoInterID userInputs;

//x and y //direction vector and ss id
typedef std::pair<twoSS, int> vectorP;
typedef std::vector<vectorP> oneInterToInter;
std::vector<oneInterToInter> multiplePath; // vector for where need to change direction
std::vector< oneInterToInter > outInfo;

std::vector<t_point> POI_highlightPoint;
std::vector<std::vector<unsigned>> foundPath; //id of ss for all paths
std::vector<std::vector<unsigned>> foundPath_POI;
std::vector<unsigned> allHighlightSS;

std::vector<t_bound_box> allCross;


unsigned lastPOI_closestInter;
int highlightPointSize = 0;
//std::vector<vectorP> newPathList; //vector contains all list


/*****************************Milestone 3***************************/

/****Function prototype ********/
void checkBarOpen();
void drawHightlightPath();
void resetColor(unsigned id);
void for_highlight_ZOOM();
void checkPOIInput();
void drawHighlight_POI_path();
void checkClickInter(LatLon& newPoint);
void computeVectorPath(std::vector<unsigned>& inputIDs);
void drawDirection();
void drawDetails();
void printINFO(std::vector<vectorP> &trueList,bool multiple);
void draw_help();
void drawPathByPath(std::string from_name , std::string to_name, oneInterToInter& currentP,int pathID);
void highlightInterPoint(t_point point_from, t_point point_to, int start);
/******Icons*****/
t_bound_box wholeScreen;
t_bound_box helpScreen;

t_bound_box helpTab1;
t_bound_box helpTab2;
t_bound_box helpTab3;

t_bound_box searchIcon;
t_bound_box leftArrow;
t_bound_box rightArrow;
t_bound_box listInter;
 t_bound_box crossArea;
t_bound_box zoomInButton;
t_bound_box zoomOutButton;
t_bound_box questionMark;
t_bound_box leftsearchBar;
t_bound_box rightSearchBar;

t_bound_box POIselect;
t_bound_box changeMapButton;
t_bound_box Details;
t_bound_box layerTab;
t_bound_box changeMapTab;
t_bound_box detailsTab;
t_bound_box findPath;
t_bound_box opened_layerTab;
t_bound_box opened_changeMapTab;
t_bound_box opened_detailsTab;
t_bound_box changePage_right;
t_bound_box changePage_left;



t_bound_box changetoPOI;
t_bound_box changeToPOI_text;
t_bound_box changeToInter;
std::vector<t_bound_box> poiIcons;



//std::vector<unsigned> foundPath_POI;
/****Enable different windows****/
bool inter_to_POI = false;
bool interIconClick = true;
bool searIconClick = false;
bool disappear_text_one = false;
bool disappear_text_two = false;
bool disappear_POI = false;
bool inPOI = false;
bool help_pressed = false;
bool helpTab1_pressed = true;
bool helpTab2_pressed = false;
bool helpTab3_pressed = false;

bool inter_icon_open = false;

bool highlightColor = false;

bool userChangePage = false;
bool userTypeChangeP = false;
bool userClickOnMap = false;
int curretWindow = 0;
bool findPathClick = false;
bool userInputWrong = false;

const float png_size = 35;
const float changeIcon_size = 18;
const float helpScreen_size = 500;
const float helpTab_size = 17;
const float mapIcon_size = png_size * 2;
const float layerIcon_size = png_size * 2;
const float detailsTabLength_size = png_size * 5.4286; //tab length is 5.4286 time larger than png size
const float changeMapTabLength_size = png_size * 3.343; //tab length is 3.343 time larger than png size

/*****Variables******/
t_point for_highlight_zoom;
t_point fromP;
t_point toP;
std::vector<char> inputString_POI;
std::string outputString_POI;



#endif /* GLOBAL_H */
