//
// Created by jayse on 05/09/2024.
//


#ifndef PP_1_KMEANS_H
#define PP_1_KMEANS_H

#include <vector>
#include "Point.h"
#include "Dataset.h"
#include <random>


class Kmeans {

    public:
        std::vector<Point> centroids;
        std::vector<Point> data;
        explicit Kmeans(std::vector<Point>& data);
        void kMeansClustering(int epochs, int k);

    private:
        std::vector<Point> random_choice(int k);
        double compute_min_distance(const Point& p);
};


#endif //PP_1_KMEANS_H
