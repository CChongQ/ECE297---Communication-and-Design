/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   interface_icon.h
 * Author: qiuchenh
 *
 * Created on March 16, 2018, 11:34 PM
 */
#include "graphics.h"
#include "Global.h"
#include "graphics_types.h"
#include <vector>

#ifndef INTERFACE_ICON_H
#define INTERFACE_ICON_H

void getValues() {
    // screen size values
    top_r_x = get_visible_screen().top_right().x; //??
    bottom_l_y = get_visible_screen().bottom_left().y;
    screen_width = get_visible_screen().get_width();
    screen_height = get_visible_screen().get_height();
    bottom_l_x_left = top_r_x * 0.02;
    bottom_l_x_right = top_r_x * 0.3;
    bar_height_bottom = bottom_l_y * 0.05;
    bar_height_top = bottom_l_y * 0.01;
    bar_length = top_r_x * 0.25;
    
    screen_ratio = screen_height / screen_width;

    //---------------------------------icons-----------------------------------------//
    
    t_bound_box temp14(top_r_x * 0.01,
            bar_height_bottom,
            top_r_x * 0.01 + changeIcon_size, 
            bar_height_top);
    changetoPOI = temp14;
    
    t_bound_box temp16(top_r_x * 0.01,
            bar_height_bottom,
            top_r_x * 0.01 + changeIcon_size, 
            bar_height_top);
    changeToInter = temp16;
    
    t_bound_box temp0(top_r_x * 0.01 + changeIcon_size, 
            bar_height_bottom, 
            top_r_x * 0.01 + changeIcon_size + bar_length, 
            bar_height_top);
    leftsearchBar = temp0;

    t_bound_box temp1(bottom_l_x_right, 
            bar_height_bottom, 
            bottom_l_x_right + bar_length, 
            bar_height_top);
    rightSearchBar = temp1;
    
    t_bound_box temp15(top_r_x * 0.01 + changeIcon_size, 
            bar_height_bottom, 
            top_r_x * 0.01 + changeIcon_size + bar_length, 
            bar_height_top);
    changeToPOI_text = temp15;  
    
    t_bound_box temp2(rightSearchBar.right() + top_r_x*0.01, 
            bar_height_bottom, 
            rightSearchBar.right() + top_r_x*0.01 + png_size, 
            bar_height_bottom * 0.3);
    searchIcon = temp2;

    t_bound_box temp3(searchIcon.right() + top_r_x*0.01, 
            bar_height_bottom, 
            searchIcon.right() + top_r_x*0.01 + png_size, 
            bar_height_bottom * 0.3);
    findPath = temp3;

    t_bound_box temp4(findPath.right() + top_r_x*0.01, 
            bar_height_bottom, 
            findPath.right() + top_r_x*0.01 + png_size, 
            bar_height_bottom * 0.3);
    listInter = temp4;
    
    t_bound_box temp5(listInter.left(), listInter.bottom() + 27,
            listInter.left()+20 ,listInter.bottom());
    crossArea = temp5;
    
/*t_bound_box temp5(rightArrow.right() + top_r_x*0.01, 
            bar_height_bottom, 
            rightArrow.right() + top_r_x*0.01 + png_size, 
            bar_height_bottom * 0.3);
    findPath = temp5;

    t_bound_box temp9(findPath.right() + top_r_x*0.01, 
            bar_height_bottom, 
            findPath.right() + top_r_x*0.01 + png_size, 
            bar_height_bottom * 0.3);
    listInter = temp9;*/
    

    t_bound_box temp6(top_r_x*0.99 - png_size, 
            bar_height_bottom, 
            top_r_x*0.99, 
            bar_height_bottom * 0.3);
    zoomInButton = temp6;

    t_bound_box temp7(top_r_x*0.99 - png_size, 
            zoomInButton.bottom() + png_size + bottom_l_y*0.01, 
            top_r_x*0.99, 
            zoomInButton.bottom() + bottom_l_y*0.01);
    zoomOutButton = temp7;

    t_bound_box temp8(top_r_x*0.99 - png_size, 
            zoomOutButton.bottom() + png_size + bottom_l_y*0.01, 
            top_r_x*0.99, 
            zoomOutButton.bottom() + bottom_l_y*0.01);
    questionMark = temp8;

    //---------------------------------tabs-----------------------------------------//
    //closed tabs

    t_bound_box temp12(top_r_x * 0.99 - png_size,
            bottom_l_y*0.99,
            top_r_x * 0.99,
            bottom_l_y*0.99 - detailsTabLength_size);
    detailsTab = temp12;
    
    t_bound_box temp11(top_r_x * 0.99 - png_size,
            bottom_l_y * 0.98 - abs(detailsTab.bottom() - detailsTab.top()),
            top_r_x * 0.99,
            bottom_l_y * 0.98 - abs(detailsTab.bottom() - detailsTab.top()) - changeMapTabLength_size);
    changeMapTab = temp11;

    t_bound_box temp10(top_r_x * 0.99 - png_size,
            bottom_l_y * 0.97 - abs(detailsTab.bottom() - detailsTab.top()) - abs(changeMapTab.bottom() - changeMapTab.top()),
            top_r_x * 0.99,
            bottom_l_y * 0.97 - abs(detailsTab.bottom() - detailsTab.top()) - abs(changeMapTab.bottom() - changeMapTab.top()) - png_size * 3.343); //tab length is 3.34 times the png_size
    layerTab = temp10;

    //opened tabs    
    t_bound_box temp17(top_r_x * 0.5, 
            bottom_l_y*0.99,
            top_r_x * 0.5 + png_size,
            bottom_l_y * 0.99 - detailsTabLength_size);
    opened_detailsTab = temp17;

    t_bound_box temp18(top_r_x * 0.01,
            bottom_l_y * 0.98 - abs(detailsTab.bottom() - detailsTab.top()),
            top_r_x * 0.01 + png_size,
            bottom_l_y * 0.98 - abs(detailsTab.bottom() - detailsTab.top()) - changeMapTabLength_size);
    opened_changeMapTab = temp18;

    changeMap_icon_gap = ((top_r_x*0.99 - opened_changeMapTab.top_right().x) - mapIcon_size*19)/20;
    if (changeMap_icon_gap < 0) changeMap_icon_gap = 0;
    
    //change map icon y value for the top stays constant at one pan view
    changeMap_icon_top_left_y = 
            (opened_changeMapTab.top_right().y + abs(opened_changeMapTab.top_right().y - opened_changeMapTab.bottom_left().y)/2) //top of changemap box
            - (mapIcon_size)/2;
    
    t_bound_box temp19(top_r_x *0.99 - layerIcon_size*6  - (top_r_x)*7/100 - png_size, //1.multiply png_size by the number of logos 2.gaps between logos 3.size of the tab
            bottom_l_y * 0.97 - abs(detailsTab.bottom() - detailsTab.top()) - abs(changeMapTab.bottom() - changeMapTab.top()),
            (top_r_x *0.99 - layerIcon_size*6  - (top_r_x)*7/100),
            bottom_l_y * 0.97 - abs(detailsTab.bottom() - detailsTab.top()) - abs(changeMapTab.bottom() - changeMapTab.top()) - png_size * 3.343);
    opened_layerTab = temp19;
    
    layer_icon_gap = ((top_r_x*0.99 - opened_layerTab.top_right().x) - layerIcon_size*6)/7;
    if(layer_icon_gap < 0) layer_icon_gap = 0;
    
    layer_icon_top_left_y = 
            (opened_layerTab.top_right().y + abs(opened_layerTab.top_right().y - opened_layerTab.bottom_left().y)/2)
            - (layerIcon_size)/2; //map and layer icons are the same size
    
    map_icon_boundaries.clear();
    
    for (int i = 0; i < 19; i++) {
        t_bound_box temp(opened_changeMapTab.right() + mapIcon_size*i + changeMap_icon_gap * (i + 1),
                changeMap_icon_top_left_y + mapIcon_size,
                opened_changeMapTab.right() + mapIcon_size*i + changeMap_icon_gap * (i + 1) + mapIcon_size,
                changeMap_icon_top_left_y);
        map_icon_boundaries.push_back(temp);
    }
    
    t_bound_box temp20(0, bottom_l_y, top_r_x, 0);
    wholeScreen = temp20;
    
    //layers tab    
    layer_icon_boundaries.clear();
    
    for (int i = 0; i < 6; i++) {
        t_bound_box temp(opened_layerTab.right() + layerIcon_size*i + layer_icon_gap * (i + 1),
                layer_icon_top_left_y + layerIcon_size,
                opened_layerTab.right() + layerIcon_size*i + layer_icon_gap * (i + 1) + layerIcon_size,
                layer_icon_top_left_y);
        layer_icon_boundaries.push_back(temp);
    }
    
    //-----------------------------help-----------------------------------------//
    
    t_bound_box temp21(top_r_x*0.98 - png_size - helpScreen_size, 
            bottom_l_y * 0.97 - abs(detailsTab.bottom() - detailsTab.top()) - abs(changeMapTab.bottom() - changeMapTab.top()) - png_size * 3.343 - png_size*0.5,
            top_r_x*0.98 - png_size,
            bar_height_bottom * 0.3 + png_size * 2.5);
    helpScreen = temp21;
    
    t_bound_box temp22(helpScreen.left() + (abs(helpScreen.right() - helpScreen.left())/12)*3,
            bottom_l_y * 0.97 - abs(detailsTab.bottom() - detailsTab.top()) - abs(changeMapTab.bottom() - changeMapTab.top()) - png_size * 3.343 - png_size*1,
            (helpScreen.left() + (abs(helpScreen.right() - helpScreen.left())/12)*3) + helpTab_size,
            (bottom_l_y * 0.97 - abs(detailsTab.bottom() - detailsTab.top()) - abs(changeMapTab.bottom() - changeMapTab.top()) - png_size * 3.343 - png_size*1) - 17);
    helpTab1 = temp22;
    
    t_bound_box temp23(helpScreen.left() + (abs(helpScreen.right() - helpScreen.left())/12)*6,
            bottom_l_y * 0.97 - abs(detailsTab.bottom() - detailsTab.top()) - abs(changeMapTab.bottom() - changeMapTab.top()) - png_size * 3.343 - png_size*1,
            (helpScreen.left() + (abs(helpScreen.right() - helpScreen.left())/12)*6) + helpTab_size,
            (bottom_l_y * 0.97 - abs(detailsTab.bottom() - detailsTab.top()) - abs(changeMapTab.bottom() - changeMapTab.top()) - png_size * 3.343 - png_size*1) - 17);
    helpTab2 = temp23;
    
    t_bound_box temp24(helpScreen.left() + (abs(helpScreen.right() - helpScreen.left())/12)*9,
            bottom_l_y * 0.97 - abs(detailsTab.bottom() - detailsTab.top()) - abs(changeMapTab.bottom() - changeMapTab.top()) - png_size * 3.343 - png_size*1,
            (helpScreen.left() + (abs(helpScreen.right() - helpScreen.left())/12)*9) + helpTab_size,
            (bottom_l_y * 0.97 - abs(detailsTab.bottom() - detailsTab.top()) - abs(changeMapTab.bottom() - changeMapTab.top()) - png_size * 3.343 - png_size*1) - 17);
    helpTab3 = temp24;
    
    
    t_bound_box temp25(detailsTab.left()-70,detailsTab.bottom(),detailsTab.left()-45,detailsTab.bottom()-30);
    changePage_left = temp25;
    
      
    t_bound_box temp26(detailsTab.left()-30,detailsTab.bottom(),detailsTab.left()-5,detailsTab.bottom()-30);
    changePage_right = temp26;
    
}

#endif /* INTERFACE_ICON_H */

