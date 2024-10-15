
#include <cstdio>
#include <iostream>
#include <chrono>
#include "../include/Dataset.h"
#include "../include/Kmeans.h"
int main()
{
    int k = 5;
    Dataset d;
    d.generateSyntheticDataset(500000,2,0,1000);
    Kmeans kmeans(d);
    auto start = std::chrono::high_resolution_clock::now();
    kmeans.kMeansClustering(1000,k);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Duration:" << duration.count() << "seconds" << std::endl;
    //kmeans.plot_clusters2d(k);
}