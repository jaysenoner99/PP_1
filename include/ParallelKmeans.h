//
// Created by Jay Senoner on 10/10/24.
//

#ifndef PP_1_PARALLELKMEANS_H
#define PP_1_PARALLELKMEANS_H

#include "Dataset.h"
#include "Point.h"
#include <matplot/matplot.h>
#include <omp.h>
#include <random>
#include <vector>
class ParallelKmeans {

public:
  explicit ParallelKmeans(const Dataset &d);
  explicit ParallelKmeans(std::vector<Point> &data);
  std::vector<Point> centroids;
  std::vector<Point> data;
  void parallelkMeansClustering(int epochs, int k, int threads);
  void plot_clusters2d(int k, std::string filename);

  const std::vector<Point> &getClusteredData() const { return this->data; }

private:
  std::vector<Point> random_choice(int k);
  bool not_changed(std::vector<Point> &old_centroids, double tol = 1e-5);

  int min_distance_cluster(const Point &p,
                           const std::vector<Point> &current_centroids);
};
#endif // PP_1_PARALLELKMEANS_H
