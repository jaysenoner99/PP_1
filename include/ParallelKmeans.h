//
// Created by Jay Senoner on 10/10/24.
//

#ifndef PP_1_PARALLELKMEANS_H
#define PP_1_PARALLELKMEANS_H



#include <vector>
#include "Point.h"
#include "Dataset.h"
#include <random>
#include <matplot/matplot.h>
#include <omp.h>

class ParallelKmeans {

public:
    explicit ParallelKmeans(const Dataset& d);
    std::vector<Point> centroids;
    std::vector<Point> data;
    void parallelkMeansClustering(int epochs, int k);
    void plot_clusters2d(int k);

private:
    std::vector<Point> random_choice(int k);
    int min_distance_cluster(const Point& p);

};
#endif //PP_1_PARALLELKMEANS_H