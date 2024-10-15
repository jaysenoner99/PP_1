//
// Created by Jay Senoner on 10/10/24.
//

#ifdef _OPENMP
#include <omp.h> // for OpenMP library functions
#endif
#include <iostream>
#include<chrono>
#include "../include/Dataset.h"
#include "../include/ParallelKmeans.h"

int main(){

    std::cout << "running parallel k means" << std::endl;
    int k = 7;
    Dataset d;
    d.generateSyntheticDataset(50000,2,0,1000);
    ParallelKmeans kmeans(d);
    auto start = std::chrono::high_resolution_clock::now();
    kmeans.parallelkMeansClustering(1000,k);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Duration:" << duration.count() << "seconds" << std::endl;
    kmeans.plot_clusters2d(k);

}