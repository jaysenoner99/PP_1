//
// Created by jayse on 05/09/2024.
//


#ifndef PP_1_KMEANS_H
#define PP_1_KMEANS_H

#include <vector>
#include "Point.h"
#include "Dataset.h"
#include <random>
#include <matplot/matplot.h>


class Kmeans {

    public:
        explicit Kmeans(const Dataset& d);
        std::vector<Point> centroids;
        std::vector<Point> data;
        void kMeansClustering(int epochs, int k);
        void plot_clusters2d(int k);
        void log_results();

    private:
        std::vector<Point> random_choice(int k);
        int min_distance_cluster(const Point& p);
};


#endif //PP_1_KMEANS_H
