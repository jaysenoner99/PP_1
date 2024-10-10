//
// Created by jayse on 05/09/2024.
//

#include "../include/Point.h"
#include <cmath>
#include <utility>


Point::Point(const std::vector<double>& coords) : coordinates(coords), cluster(-1){}


Point::Point(double x, double y) {
    coordinates[0] = x;
    coordinates[1] = y;
    cluster = -1;
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
        std::cout << i << " ";
    }
}
void Point::log_results() {
    for(auto i: coordinates){
        std::cout << i << " ";
    }
    std::cout << cluster << std::endl;
}


const std::vector<double> &Point::getCoordinates() const {
    return coordinates;
}

void Point::setCoordinates(const std::vector<double> &coords) {
    coordinates = coords;
}

void Point::setCluster(int cluster) {
    this->cluster = cluster;
}

int Point::getCluster() const {
    return cluster;
}

bool Point::operator==(const Point &rhs) const {
    return coordinates == rhs.coordinates;
}

bool Point::operator!=(const Point &rhs) const {
    return !(rhs == *this);
}

Point Point::operator+(const Point &rhs) const {
    if (coordinates.size() != rhs.coordinates.size()) {
        throw std::invalid_argument("Points must have the same number of dimensions");
    }

    std::vector<double> result(coordinates.size());
    for (std::size_t i = 0; i < coordinates.size(); ++i) {
        result[i] = coordinates[i] + rhs.coordinates[i];
    }

    return Point(result);
}

Point& Point::operator+=(const Point& other) {
    if (coordinates.size() != other.coordinates.size()) {
        throw std::invalid_argument("Points must have the same number of dimensions");
    }

    for (std::size_t i = 0; i < coordinates.size(); ++i) {
        coordinates[i] += other.coordinates[i];
    }

    return *this;  // Return the current object (modified)
}

Point Point::operator/(const int div){
    std::vector<double> result(coordinates.size());
    for(std::size_t i = 0; i < coordinates.size(); ++i){
        result[i] = coordinates[i] / div;
    }
    return Point(result);
}





