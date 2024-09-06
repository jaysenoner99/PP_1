//
// Created by jayse on 05/09/2024.
//

#ifndef PP_1_POINT_H
#define PP_1_POINT_H

#include <vector>

class Point {

    private:
        std::vector<double> coordinates;

    public:
        Point(double x, double y);
        explicit Point(std::vector<double>  coords);
        double distance(const Point& other) const;
        const std::vector<double>& getCoordinates() const;
        void setCoordinates(const std::vector<double>& coords);

};



#endif //PP_1_POINT_H


