//
// Created by jayse on 05/09/2024.
//

#ifndef PP_1_POINT_H
#define PP_1_POINT_H

#include <vector>
#include <iostream>
class Point {

    private:
        std::vector<double> coordinates;
        int cluster{};

public:
        Point(double x, double y);
        Point(int size);
        explicit Point(const std::vector<double>&  coords);
        double distance(const Point& other) const;
        const std::vector<double>& getCoordinates() const;
        void setCoordinates(const std::vector<double>& coords);
        void logPointCoordinates();
        void log_results();
        void setCluster(int cluster);
        int getCluster() const;
        bool operator==(const Point &rhs) const;
        bool operator!=(const Point &rhs) const;
        Point operator+(const Point &rhs) const;
        Point operator-(const Point &rhs) const;
        Point& operator+=(const Point &rhs);
        Point operator/(int div);
        double l2normdiff(const Point& other) const;
};



#endif //PP_1_POINT_H


