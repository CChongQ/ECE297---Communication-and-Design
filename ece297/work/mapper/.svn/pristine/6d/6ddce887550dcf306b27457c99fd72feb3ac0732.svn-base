#include <algorithm>
#include <set>
#include <unittest++/UnitTest++.h>
#include "m1.h"
#include "unit_test_util.h"
#include "StreetsDatabaseAPI.h"

//using ece297test::relative_error;
using namespace std;

SUITE(test_find_street_ids_from_name_toronto_canada_) {
    struct MapFixture {
        MapFixture() {
            //Load the map
            load_map("/cad2/ece297s/public/maps/toronto_canada.streets.bin");
        }

        ~MapFixture() {
            //Clean-up
            close_map();
        }
    };

    TEST_FIXTURE(MapFixture, corner_cases) {
        std::vector <unsigned> expected;
        std::vector<unsigned> actual;
        
        //standard test
        actual = find_street_ids_from_name("Highway 401 Eastbound Collectors");
        expected = {1};
        CHECK_EQUAL(expected, actual);
        
        //instance of 3 same name
        actual = find_street_ids_from_name("Queen's Park");
        expected = {124, 125, 3052};
        CHECK_EQUAL(expected, actual);
        
        //long street name
        actual = find_street_ids_from_name("Ramp to Highway 401 Eastbound & Don Valley Parkway");
        expected = {59};
        CHECK_EQUAL(expected, actual);
        
        //instance of 12 same name and special '&' character
        actual = find_street_ids_from_name("Kiss & Ride");
        expected = {20804, 20835, 20836, 20837, 20838, 20839, 20840, 20841, 20842, 20843, 20844, 20845};
        CHECK_EQUAL(expected, actual);
        
        //instance of getting last element
        actual = find_street_ids_from_name("Louvain Avenue");
        expected = {21221};
        CHECK_EQUAL(expected, actual);
    }
    
    //testing for time constraint
    TEST_FIXTURE(MapFixture, time) {
        std::vector <unsigned> expected;
        std::vector <unsigned> actual;
        UNITTEST_TIME_CONSTRAINT(250);
        
        
        for (auto j = 0; j < 20000; j++) {
            actual = find_street_ids_from_name(getStreetName(j));
             expected.push_back(j);
        }
        for (auto j = 0; j < 20000; j++) {
            actual = find_street_ids_from_name(getStreetName(j));
             expected.push_back(j);
        }
        for (auto j = 0; j < 20000; j++) {
            actual = find_street_ids_from_name(getStreetName(j));
             expected.push_back(j);
        }
        for (auto j = 0; j < 20000; j++) {
            actual = find_street_ids_from_name(getStreetName(j));
             expected.push_back(j);
        }
        for (auto j = 0; j < 20000; j++) {
            actual = find_street_ids_from_name(getStreetName(j));
             expected.push_back(j);
        }
    }
}
