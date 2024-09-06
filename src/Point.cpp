//
// Created by jayse on 05/09/2024.
//

#include "../include/Point.h"
#include <cmath>
#include <utility>


Point::Point(std::vector<double> coords) : coordinates(std::move(coords)){}


Point::Point(double x, double y) {
    coordinates[0] = x;
    coordinates[1] = y;
}



double Point::distance(const Point &other) const {
    double sum = 0.0;
    for (size_t i = 0; i < coordinates.size(); ++i) {
        sum += std::pow(coordinates[i] - other.coordinates[i], 2);
    }
    return std::sqrt(sum);
}

const std::vector<double> &Point::getCoordinates() const {
    return coordinates;
}

void Point::setCoordinates(const std::vector<double> &coords) {
    coordinates = coords;
}






