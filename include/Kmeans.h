//
// Created by jayse on 05/09/2024.
//

#ifndef PP_1_KMEANS_H
#define PP_1_KMEANS_H

#include "Dataset.h"
#include "Point.h"
#include <matplot/matplot.h>
#include <random>
#include <vector>

class Kmeans {

public:
  explicit Kmeans(const Dataset &d);
  std::vector<Point> centroids;
  std::vector<Point> data;
  void kMeansClustering(int epochs, int k);
  void plot_clusters2d(int k);
  void log_results();
  const std::vector<Point> &getClusteredData() const { return this->data; }

private:
  std::vector<Point> random_choice(int k);
  int min_distance_cluster(const Point &p);
  bool not_changed(const std::vector<Point> &old_centroids, double tol = 1e-5);
  int min_distance_cluster(const Point &p,
                           const std::vector<Point> &current_centroids) const;
};

#endif // PP_1_KMEANS_H
