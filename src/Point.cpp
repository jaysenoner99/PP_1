//
// Created by jayse on 05/09/2024.
//

#include "../include/Point.h"
#include <cmath>
#include <utility>


Point::Point(const std::vector<double>& coords) : coordinates(coords), cluster(-1), minDist(__DBL_MAX__){}


Point::Point(double x, double y) {
    coordinates[0] = x;
    coordinates[1] = y;
    cluster = -1;
    minDist = __DBL_MAX__;
}

double Point::distance(const Point &other) const {
    double sum = 0.0;
    for (size_t i = 0; i < coordinates.size(); ++i) {
        sum += std::pow(coordinates[i] - other.coordinates[i], 2);
    }
    return std::sqrt(sum);
}

void Point::logPointCoordinates(){
    for(auto i: coordinates){
        std::cout << i ;
    }
}



const std::vector<double> &Point::getCoordinates() const {
    return coordinates;
}

void Point::setCoordinates(const std::vector<double> &coords) {
    coordinates = coords;
}






