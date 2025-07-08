//
// Created by jayse on 05/09/2024.
//

#include "../include/Kmeans.h"
#include <map>
#include <utility>
void Kmeans::kMeansClustering(int max_epochs, int k) {
  if (data.empty())
    return;

  centroids = random_choice(k);

  for (int epoch = 0; epoch < max_epochs; ++epoch) {
    const std::vector<Point> local_centroids = centroids;

    for (auto &data_point : data) {
      int new_cluster = min_distance_cluster(data_point, local_centroids);
      data_point.setCluster(new_cluster);
    }

    std::vector<Point> new_centroids(k);
    std::vector<int> counters(k, 0);

    for (const auto &data_point : data) {
      int cluster_id = data_point.getCluster();
      if (cluster_id != -1) {
        new_centroids[cluster_id] += data_point;
        counters[cluster_id]++;
      }
    }

    for (int i = 0; i < k; ++i) {
      if (counters[i] > 0) {
        new_centroids[i] = new_centroids[i] / counters[i];
      }
    }

    if (not_changed(new_centroids)) {
      centroids = new_centroids;
      return;
    }

    centroids = new_centroids;
  }
}

// --- CHANGE 3: Rewritten, optimized min_distance_cluster function ---
// It now takes 'current_centroids' as an argument and is more efficient.
int Kmeans::min_distance_cluster(
    const Point &p, const std::vector<Point> &current_centroids) const {
  double min_dist = std::numeric_limits<double>::max();
  int best_cluster = -1;

  for (size_t i = 0; i < current_centroids.size(); ++i) {
    double dist = current_centroids[i].distance(p);
    if (dist < min_dist) {
      min_dist = dist;
      best_cluster = static_cast<int>(i);
    }
  }
  return best_cluster;
}
std::vector<Point> Kmeans::random_choice(int k) {

  int totalPoints = (int)data.size();
  if (k > totalPoints) {
    std::cerr << "Error: k cannot be greater than the total number of points."
              << std::endl;
    return {};
  }

  std::random_device rd;  // Obtain a random number from hardware
  std::mt19937 gen(rd()); // Seed the generator
  std::uniform_int_distribution<> dist(0, totalPoints -
                                              1); // Range is [0, totalPoints-1]
  std::vector<Point> selectedPoints;

  std::vector<bool> chosen(totalPoints, false);

  // Randomly select k points
  for (int i = 0; i < k; ++i) {
    int randomIndex;
    do {
      randomIndex = dist(gen); // Generate a random index
    } while (chosen[randomIndex]); // Ensure it's not a duplicate

    // Mark the index as chosen
    chosen[randomIndex] = true;

    // Add the randomly chosen point to the selected points vector
    selectedPoints.push_back(data[randomIndex]);
  }

  return selectedPoints;
}

void Kmeans::plot_clusters2d(int k) {
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

  show();
}

Kmeans::Kmeans(const Dataset &d) { data = d.getDataset(); }

void Kmeans::log_results() {
  for (auto point : data) {
    point.log_results();
    std::cout << std::endl;
  }
}
// Also update not_changed to be const correct
bool Kmeans::not_changed(const std::vector<Point> &new_centroids, double tol) {
  if (centroids.size() != new_centroids.size())
    return false; // Safety check
  for (size_t i = 0; i < centroids.size(); ++i) {
    if (centroids[i].l2normdiff(new_centroids[i]) >= tol)
      return false;
  }
  return true;
}
