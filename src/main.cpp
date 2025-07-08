
#include "../include/Dataset.h"
#include "../include/Kmeans.h"
#include "../include/ParallelKmeans.h"
#include <chrono>
#include <cstdio>
#include <iostream>
int main() {
  int k = 5;
  Dataset d;
  int threads = 16;
  d.generateSyntheticDataset(50000, 2, 0, 1000);
  ParallelKmeans kmeans(d);
  auto start = std::chrono::high_resolution_clock::now();
  kmeans.parallelkMeansClustering(1000, k, threads);
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
  std::cout << "Duration:" << duration.count() << "seconds" << std::endl;
  kmeans.plot_clusters2d(k, "");
}
