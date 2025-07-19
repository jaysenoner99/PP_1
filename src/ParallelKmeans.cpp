//
// Created by Jay Senoner on 10/10/24.
//
#include "../include/ParallelKmeans.h"

ParallelKmeans::ParallelKmeans(const Dataset &d) { data = d.getDataset(); }


void ParallelKmeans::parallelkMeansClustering(int max_epochs, int k,
                                              int threads) {
  if (data.empty())
    return;

  centroids = random_choice(k);

  const int n = data.size();
  std::vector<Point> new_centroids(k);
  std::vector<int> cluster_counters(k, 0);

  for (int epoch = 0; epoch < max_epochs; ++epoch) {
    for (auto &p : new_centroids)
      p.reset();
    std::fill(cluster_counters.begin(), cluster_counters.end(), 0);

#pragma omp parallel num_threads(threads)
    {
      const std::vector<Point> local_centroids = centroids;

      std::vector<Point> private_centroids_sum(k);
      std::vector<int> private_counts(k, 0);

#pragma omp for schedule(static) nowait
      for (size_t j = 0; j < n; ++j) {
        // Pass the thread's FAST local copy to min_distance_cluster
        int cluster_id = min_distance_cluster(data[j], local_centroids);
        data[j].setCluster(cluster_id);
        private_centroids_sum[cluster_id] += data[j];
        private_counts[cluster_id]++;
      }

#pragma omp critical
      {
        for (int c = 0; c < k; ++c) {
          new_centroids[c] += private_centroids_sum[c];
          cluster_counters[c] += private_counts[c];
        }
      }
    } // --- End of parallel region ---

#pragma omp parallel for num_threads(threads)
    for (int j = 0; j < k; ++j) {
      if (cluster_counters[j] > 0) {
        new_centroids[j] = new_centroids[j] / cluster_counters[j];
      }
    }

    if (not_changed(new_centroids)) {
      centroids = new_centroids;
      return;
    }
    centroids = new_centroids;
  }
}


int ParallelKmeans::min_distance_cluster(
    const Point &p, const std::vector<Point> &current_centroids) {
  double min_dist = std::numeric_limits<double>::max();
  int best_cluster = -1;
  for (int i = 0; i < current_centroids.size(); ++i) {
    double dist = current_centroids[i].distance(p);
    if (dist < min_dist) {
      min_dist = dist;
      best_cluster = i;
    }
  }
  return best_cluster;
}
std::vector<Point> ParallelKmeans::random_choice(int k) {

  int totalPoints = (int)data.size();
  if (k > totalPoints) {
    std::cerr << "Error: k cannot be greater than the total number of points."
              << std::endl;
    return {};
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, totalPoints - 1);
  std::vector<Point> selectedPoints;
  std::vector<bool> chosen(totalPoints, false);
  for (int i = 0; i < k; ++i) {
    int randomIndex;
    do {
      randomIndex = dist(gen);
    } while (chosen[randomIndex]);

    chosen[randomIndex] = true;
    selectedPoints.push_back(data[randomIndex]);
  }

  return selectedPoints;
}

void ParallelKmeans::plot_clusters2d(int k, std::string filename) {
  using namespace matplot;
  // Enable holding to plot multiple clusters in one figure
  hold(true);

  // Define a set of distinct colors to use for clusters
  // std::vector<std::string> colors = {"r", "g", "b", "m", "y", "c"};
  std::vector<std::string> colors = {"r", "g", "b", "m", "y", "c", "k", "w"};
  // Prepare a vector of x, y coordinates for each cluster

  for (int cluster_id = 0; cluster_id < k; ++cluster_id) {
    std::vector<double> x_coords, y_coords;

    // Collect all points for the current cluster
    for (const auto &point : data) {
      if (point.getCluster() == cluster_id) {
        const auto &coordinates = point.getCoordinates();
        if (coordinates.size() != 2) {
          throw std::invalid_argument(
              "All points must have 2 dimensions (x, y).");
        }
        // Add the (x, y) coordinates to the vectors
        x_coords.push_back(coordinates[0]);
        y_coords.push_back(coordinates[1]);
      }
    }

    // Plot the points for the current cluster using a unique color
    scatter(x_coords, y_coords)
        ->marker_size(5)
        .marker_color(colors[cluster_id % colors.size()]);
  }

  hold(false);

  matplot::save(filename + "cluster_plot" + std::to_string(k) + ".png");
}

bool ParallelKmeans::not_changed(std::vector<Point> &old_centroids,
                                 double tol) {
  for (size_t i = 0; i < centroids.size(); ++i) {
    if (centroids[i].distance(old_centroids[i]) >= tol)
      return false;
  }
  return true;
}

ParallelKmeans::ParallelKmeans(std::vector<Point> &data) { this->data = data; }
