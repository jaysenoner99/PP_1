//
// Created by jayse on 05/09/2024.
//

#include <vector>
#include "Point.h"
#ifndef PP_1_KMEANS_H
#define PP_1_KMEANS_H


class Kmeans {

    public:
        Kmeans();
        void kMeansClustering(int epochs, int k, std::vector<Point>& dataset);

};


#endif //PP_1_KMEANS_H
