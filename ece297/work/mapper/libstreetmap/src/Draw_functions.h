/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Draw_functions.h
 * Author: qiuchenh
 *
 * Created on February 26, 2018, 5:14 PM
 */
#include "newmap.h"
#include "m1.h"
#include "m2.h"
#include "Helper.h"
#include "interface_icon.h"
#include "StreetsDatabaseAPI.h"
#include "OSMDatabaseAPI.h" 
#include "graphics.h"
#include "Global.h"

#ifndef DRAW_FUNCTIONS_H
#define DRAW_FUNCTIONS_H
void draw_subway();
void drawscreen(void) {
    clearscreen();

    getValues();
    //set value for search bar coordinate variables     
    if (only_draw_string == true && userTypeChangeP == false) {
        set_coordinate_system(GL_SCREEN);
        set_drawing_buffer(ON_SCREEN);
        drawSearchBar();
        draw_string();
        set_coordinate_system(GL_WORLD);
        copy_off_screen_buffer_to_screen();
        set_drawing_buffer(OFF_SCREEN);
        only_draw_string = false;
    } else {

        
        draw_features();
        //draw ss
        draw_streetSegment();
        //draw POIs
        if (layer_pressed[5]) {
            draw_subway();
        }
       
        draw_POIs();
        
        set_coordinate_system(GL_SCREEN);

        drawSearchBar();

        checkBarOpen();
        if (details_opened) {
            drawDetails();
        }

        if (userTypeChangeP == true && details_opened == true) {
            drawDirection();
        }


        draw_string();

        //draw info bar 
        if (clickTwoInter.interOne != 0 && clickTwoInter.interTwo == 0) {
            set_coordinate_system(GL_WORLD);
            fromP = world_to_scrn(clickTwoInter.interOneW);
            set_coordinate_system(GL_SCREEN);
          draw_surface(load_png_from_file("red_start.png"), fromP.x - 15, fromP.y - 29);         
        }


        //when user click the search icon, highlight related intersections
        if (highlightColor == true) {
            if (userInputs.POIsName.empty() == false) {
                //highlight between pois       
                // std::cout << "userInputs.POIsName.size()  = " << userInputs.POIsName.size() << std::endl;
                if (userInputs.POIsName.size() == 1) {
                    //between inter and POI
                    highlightInterPoint(userInputs.intersWorldP[0], POI_highlightPoint[0],0);
                } else {
                    highlightInterPoint(userInputs.intersWorldP[0], POI_highlightPoint[0],0);
                    for (int i = 0; i < abs(POI_highlightPoint.size() - 1); i++) {
                        highlightInterPoint(POI_highlightPoint[i], POI_highlightPoint[i + 1],i);
                    }
                }
            } else if (clickTwoInter.twoInput == false) {
                //between intersections              
               // highlightInterPoint(userInputs.intersWorldP[0], userInputs.intersWorldP[1],0);
                for (int i = 0; i < abs(highlightPointSize - 1); i++) {
                    highlightInterPoint(userInputs.intersWorldP[i], userInputs.intersWorldP[i + 1],i);
                }

            } else if (clickTwoInter.twoInput == true) {
                set_coordinate_system(GL_WORLD);
                fromP = world_to_scrn(clickTwoInter.interOneW);
                toP = world_to_scrn(clickTwoInter.interTwoW);
                set_coordinate_system(GL_SCREEN);
                draw_surface(load_png_from_file("red_start.png"), fromP.x - 15, fromP.y - 29);
                draw_surface(load_png_from_file("blue_end.png"), toP.x - 15, toP.y - 29);
            }
        }
        if (help_pressed) {
            draw_help();
        }

        set_coordinate_system(GL_WORLD);
        copy_off_screen_buffer_to_screen();
    }


}

void highlightInterPoint(t_point point_from, t_point point_to,  int start) {
    set_coordinate_system(GL_WORLD);
  //  std::cout << "point_from x = " << point_from.x << std::endl;
   // std::cout << "point_to x = " << point_to.x<<std::endl;
    fromP = world_to_scrn(point_from);
    toP = world_to_scrn(point_to);
    set_coordinate_system(GL_SCREEN);


    if (start == 0) {     
        draw_surface(load_png_from_file("red_start.png"), fromP.x - 15, fromP.y - 29);
       // draw_surface(load_png_from_file("blue_end.png"), toP.x - 15, toP.y - 29);

    } 
    
    if(start ==  abs(highlightPointSize - 2)){
        draw_surface(load_png_from_file("blue_end.png"), toP.x - 15, toP.y - 29);
    }else {  
        int which_pin = start % 5;
       if ( which_pin == 0)
          draw_surface(load_png_from_file("1.png"), toP.x - 15, toP.y - 29);
        else if (which_pin == 1)
          draw_surface(load_png_from_file("2.png"), toP.x - 15, toP.y - 29);
        else if (which_pin == 2)
          draw_surface(load_png_from_file("3.png"), toP.x - 15, toP.y - 29);
        else if (which_pin == 3)
          draw_surface(load_png_from_file("4.png"), toP.x - 15, toP.y - 29);
    }     
}

void draw_help() {
    setcolor(67, 68, 69);
    fillrect(helpScreen);
    if (helpTab1_pressed) {
        draw_surface(load_png_from_file("help_search-bar.png"), helpScreen.left(), helpScreen.top());
    } else if (helpTab2_pressed) {
        draw_surface(load_png_from_file("help_click.png"), helpScreen.left(), helpScreen.top());
    } else if (helpTab3_pressed) {
        draw_surface(load_png_from_file("help_tabs.png"), helpScreen.left(), helpScreen.top());
    }

    //draw white small tabs for help window
    draw_surface(load_png_from_file("tab.png"), helpTab1.left(), helpTab1.top());
    draw_surface(load_png_from_file("tab.png"), helpTab2.left(), helpTab2.top());
    draw_surface(load_png_from_file("tab.png"), helpTab3.left(), helpTab3.top());
}

//load icons for each POI and draw them

void draw_POIs() {
    for (unsigned i = 0; i < POIs.size(); i++) {
        double x = xcoord(POIs[i].position.lon());
        double y = ycoord(POIs[i].position.lat());
        float height = 0.000001;
        //draw preloaded icons in different zoom level
        if (LOD_area_test(area * 0.0008)) {
            setcolor(t_color(255, 0, 255));

            if (POIs[i].type == "hospital" && layer_pressed[0]) {
                draw_surface(load_png_from_file("hospital.png"), x, y);
            } else if (POIs[i].type == "pharmacy" && layer_pressed[0]) {
                draw_surface(load_png_from_file("pharmacy.png"), x, y);
            } else if (POIs[i].type == "fire_station" && layer_pressed[0]) {
                draw_surface(load_png_from_file("fire_station.png"), x, y);
            } else if (POIs[i].type == "theatre" && layer_pressed[1]) {
                draw_surface(load_png_from_file("theatre.png"), x, y);
            } else if (POIs[i].type == "pub" && layer_pressed[1]) {
                draw_surface(load_png_from_file("pub.png"), x, y);
            } else if (POIs[i].type == "cinema" && layer_pressed[1]) {
                draw_surface(load_png_from_file("cinema.png"), x, y);
            } else if (POIs[i].type == "bank" && layer_pressed[2]) {
                draw_surface(load_png_from_file("bank.png"), x, y);
            } else if (POIs[i].type == "bus_station" && layer_pressed[4]) {
                draw_surface(load_png_from_file("bus_station.png"), x, y);
            } else if (POIs[i].type == "fuel" && layer_pressed[4]) {
                draw_surface(load_png_from_file("fuel.png"), x, y);
            } else if (POIs[i].type == "subway_entrance" && layer_pressed[4]) {
                draw_surface(load_png_from_file("subway_entrance.png"), x, y);
            } else if (POIs[i].type == "parking" && layer_pressed[4]) {
                draw_surface(load_png_from_file("parking.png"), x, y);
            }
        }
        if (LOD_area_test(area * 0.0003)) {
            if (POIs[i].type == "fast_food" && layer_pressed[1]) {
                draw_surface(load_png_from_file("fast_food.png"), x, y);
            } else if (POIs[i].type == "cafe" && layer_pressed[1]) {
                draw_surface(load_png_from_file("cafe.png"), x, y);
            } else if (POIs[i].type == "restaurant" && layer_pressed[1]) {
                draw_surface(load_png_from_file("restaurant.png"), x, y);
            }
            if (LOD_area_test(area * 0.0003)) {
                setfontsize(8);
                setcolor(t_color(139, 131, 120));
                drawtext(x, y + height, POIs[i].name);
            }
        }
    }
}
bool test = true;

t_point on_poi_icon(float x, float y) {
    LatLon newpoint(x, y);
    poi_id = find_closest_point_of_interest(newpoint);

    //get world coord of closest POI
    float x2 = xcoord(getPointOfInterestPosition(poi_id).lon());
    float y2 = ycoord(getPointOfInterestPosition(poi_id).lat());

    t_point worldCoord(x2, y2);

    t_point screenCoord = world_to_scrn(worldCoord);

    return screenCoord;
}

//to be implemented for m4 where hovering over POI icons would display information

void act_on_mouse_move(float x, float y) {

    //    t_point screenCoord = on_poi_icon(x, y);

    user_mouse_world.x = x;
    user_mouse_world.y = y;
    user_mouse_screen = world_to_scrn(user_mouse_world);

    //    if (abs(screenCoord.x - user_mouse_screen.x) < 0.000001 && abs(screenCoord.y - user_mouse_screen.y) < 0.000001) {
    //    }
}

void draw_features() {
    for (auto it = features.begin(); it != features.end(); ++it) {
        t_color color = getFeatureColor((*it).type);
        // check if shape is open ended (stream)
        auto index = (*it).points.size() - 1;
        if (((*it).points[0].lat() != (*it).points[index].lat()) && ((*it).points[0].lon() != (*it).points[index].lon())) {
            if (LOD_area_test(0.05 * area)) {
                unsigned pointIDprev = 0;
                for (unsigned pointID = 1; pointID < (*it).points.size(); pointID++) {
                    setlinewidth(1);
                    //setcolor(t_color(135,206,250));
                    setcolor(color.red, color.green, color.blue);
                    drawline((*it).cartPoints[pointIDprev].x, (*it).cartPoints[pointIDprev].y,
                            (*it).cartPoints[pointID].x, (*it).cartPoints[pointID].y);
                    pointIDprev = pointID;
                }
            }
        } else {
            //t_point *points;

            if (getFeatureType((*it).type) == "Building") {
                //setcolor(t_color(152,251,152));
                if (LOD_area_test(area * 0.005)) {
                    setcolor(color.red, color.green, color.blue);
                    fillpoly((*it).cartPoints, (*it).points.size());
                    if (test) {
                        test = false;
                        // std::cout << (*it).points.size() << std::endl;
                        //   for (unsigned i = 0; i < (*it).points.size(); ++i) {
                        //       std::cout << (*it).cartPoints[i].x << ", " << (*it).cartPoints[i].y << std::endl;
                        //   }
                    }
                }

            } else if (getFeatureType((*it).type) == "Greenspace" || getFeatureType((*it).type) == "Park"
                    || getFeatureType((*it).type) == "Golfcourse"
                    || getFeatureType((*it).type) == "River"
                    || getFeatureType((*it).type) == "Beach"
                    || getFeatureType((*it).type) == "Shoreline"
                    || getFeatureType((*it).type) == "Island") {
                if (LOD_area_test(area * 5)) {
                    setcolor(color.red, color.green, color.blue);
                    fillpoly((*it).cartPoints, (*it).points.size());
                }
            } else {
                setcolor(color.red, color.green, color.blue);
                fillpoly((*it).cartPoints, (*it).points.size());

            }
        }
    }
}


//Use tag from OSMDatabaseAPI to classify streets
//then draw them in different color and size

void draw_streetSegment() {

  
    
    
    
    for (unsigned i = 0; i < ss.size(); i++) {
        for (unsigned pointID = 0; pointID < ss[i].points.size() - 1; pointID++) {
            bool unknown = true;
            //----------draw street segment initialization--------------//
            double fromx = xcoord(ss[i].points[pointID].lon());
            double fromy = ycoord(ss[i].points[pointID].lat());
            double tox = xcoord(ss[i].points[pointID + 1].lon());
            double toy = ycoord(ss[i].points[pointID + 1].lat());

            t_point pt1;
            t_point pt2;
            pt1.x = fromx;
            pt1.y = fromy;
            pt2.x = tox;
            pt2.y = toy;

            //--------------draw names initialization--------------------//
            setfontsize(10);
            double x, y, degreeAngle;
            x = tox - fromx;
            y = toy - fromy;

            if (ss[i].name != "<unknown>") {
                degreeAngle = atan(y / x) * 180 / PI;
                unknown = false;
            }
            //--------------draw oneway initialization--------------------//
            double degreeAngleOneWay = atan2(y, x) * 180 / PI;
            //----------------draw (zoom implemented)------------------------//
            //mymap->osmidToindex[mymap->ssidtoOSMID[i]]//osmWay index
            //create a pointer point to OSMWays
            //get OSMWay index by street segment index
            const OSMWay* test = getWayByIndex(mymap->osmidToindex[mymap->ssidtoOSMID[i]]);
            for (unsigned k = 0; k < getTagCount(test); k++) {

                std::string key, value;
                std::tie(key, value) = getTagPair(test, k);
                if (key == "highway") {

                    if (value == "motorway" || value == "trunk" || value == "motorway_link" || value == "trunk_link" || value == "primary" || value == "primary_link") {
                        if (LOD_area_test(area * 5)) { //zoomed in area compared to total area
                            setcolor(t_color(255, 255, 107));
                            resetColor(i);
                            if (8 * 0.000001 / (get_visible_world().area()) <= 2) {
                                setlinewidth(8 * 0.000001 / (get_visible_world().area()));
                                drawline(fromx, fromy, tox, toy);
                            } else if (8 * 0.000001 / (get_visible_world().area()) <= 110) {
                                setlinewidth(8 * 0.0000001 / (get_visible_world().area()));

                                drawline(fromx, fromy, tox, toy);
                            } else if (8 * 0.000001 / (get_visible_world().area()) <= 314.18) {
                                setlinewidth(8 * 0.00000007 / (get_visible_world().area()));

                                drawline(fromx, fromy, tox, toy);
                            } else if (8 * 0.000001 / (get_visible_world().area()) > 314.18) {
                                setlinewidth(16);

                                drawline(fromx, fromy, tox, toy);
                            }
                        }


                        drawSS(unknown, degreeAngle, degreeAngleOneWay, pt1, pt2, i);
                    } else if (value == "secondary" || value == "secondary_link") { //sub-highways
                        setcolor(t_color(255, 255, 178));
                        resetColor(i);
                        if (LOD_area_test(area * 0.5)) {
                            if (1.5 * 0.000001 / (get_visible_world().area()) <= 10) {
                                setlinewidth(1.5 * 0.000001 / (get_visible_world().area()));
                                drawline(fromx, fromy, tox, toy);
                            } else if (1.5 * 0.000001 / (get_visible_world().area()) > 10) {
                                setlinewidth(10);

                                drawline(fromx, fromy, tox, toy);
                            }
                        }
                        drawSS(unknown, degreeAngle, degreeAngleOneWay, pt1, pt2, i);
                    } else if (value == "tertiary" || value == "tertiary_link" || value == "unclassified" || value == "residential" || value == "service") { //white roads
                        if (LOD_area_test(area * 0.05)) {
                            setcolor(t_color(255, 255, 255));
                            resetColor(i);
                            if (1 * 0.000001 / (get_visible_world().area()) <= 10) {
                                setlinewidth(1 * 0.000001 / (get_visible_world().area()));
                                drawline(fromx, fromy, tox, toy);
                            } else if (1 * 0.000001 / (get_visible_world().area()) > 10) {
                                setlinewidth(10);

                                drawline(fromx, fromy, tox, toy);
                            }
                        }
                        drawSS(unknown, degreeAngle, degreeAngleOneWay, pt1, pt2, i);
                    } else if (value == "footway" || value == "bridleway" || value == "path" || value == "steps") {
                        if (LOD_area_test(area * 0.05)) {
                            setcolor(t_color(117, 117, 117));
                            resetColor(i);
                            if (1 * 0.000001 / (get_visible_world().area()) <= 6) {
                                setlinewidth(1 * 0.000001 / (get_visible_world().area()));

                                drawline(fromx, fromy, tox, toy);
                            } else if (1 * 0.000001 / (get_visible_world().area()) > 6) {
                                setlinewidth(6);

                                drawline(fromx, fromy, tox, toy);
                            }
                        }
                        drawSS(unknown, degreeAngle, degreeAngleOneWay, pt1, pt2, i);
                    }
                }
            }
        }
    }
}
void line_Graphing(std::vector<t_point> points_Collection){
    unsigned vector_size = points_Collection.size();
    for(unsigned i = 0; i < vector_size-1; i++){
        if (LOD_area_test(area)) {
        drawline(points_Collection[i],points_Collection[i+1]);
        }
    }
    
    
    
    
    
}
void draw_subway() {
    color_types color_indicies[] = {
        GREEN,
        ORANGE,
        PURPLE,
        BLUE,
        //FIREBRICK
        
    };
    unsigned num_subwaylines = mymap->subway_Coordinate.size();
    for(unsigned i = 0; i < num_subwaylines; i++){
        int count = 21;
        setcolor(color_indicies[i%count]);
        setlinestyle(SOLID,ROUND);
        unsigned num_ways = mymap->subway_Coordinate[i].size();
        for(unsigned j = 0; j < num_ways; j++){
            setlinewidth(3);
            line_Graphing((mymap->subway_Coordinate[i])[j]);
            
        }
        
    }
    setlinestyle(SOLID,ROUND);
}

void resetColor(unsigned id) {

    for (std::vector<unsigned>::iterator it = allHighlightSS.begin(); it != allHighlightSS.end(); it++) {
        // check whether current ss is to be highlight
        unsigned temp = *it;
        if (temp == id) {
            setcolor(t_color(54, 97, 225));
            break;
        }
    }
}

//use boundbox to display names of street & arrows for one-way street

void drawSS(bool unknown, double degreeAngle, double degreeAngleOneWay, t_point& pt1, t_point& pt2, unsigned i) {
    t_point text_pos((pt1.x + pt2.x) / 2, (pt1.y + pt2.y) / 2);
    if (!unknown) {
        setcolor(t_color(0, 0, 0, 170));


        t_bound_box bound(pt1, pt2);

        settextrotation(degreeAngle);

        drawtext(text_pos.x, text_pos.y, ss[i].name, abs(pt1.x - pt2.x), abs(pt1.y - pt2.y));

        settextrotation(0);
    }

    if ((mymap->streetSegmentUOM[i]).oneWay) {
        setcolor(t_color(0, 0, 0, 60));
        settextrotation(degreeAngleOneWay);
        drawtext(text_pos.x, text_pos.y, "->", abs(pt1.x - pt2.x), abs(pt1.y - pt2.y));
        settextrotation(0);
    }
}

#endif /* DRAW_FUNCTIONS_H */

