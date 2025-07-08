//
// Created by Jay Senoner on 10/10/24.
//
#include "../include/ParallelKmeans.h"

ParallelKmeans::ParallelKmeans(const Dataset &d) { data = d.getDataset(); }
// Version 0
// void ParallelKmeans::parallelkMeansClustering(int max_epochs, int k,
//                                               int threads) {
//   // int max_threads = omp_get_max_threads();
//   int chunk = ceil(static_cast<double>(data.size()) / threads);
//   centroids = random_choice(k);
//   int point_size = static_cast<int>(centroids[0].getCoordinates().size());
//   std::vector<Point> updated_centroids(k, Point(point_size));
//   std::vector<int> cluster_counters(k, 0);
//
//   for (int i = 0; i < max_epochs; ++i) {
//     updated_centroids = std::vector<Point>(k, Point(point_size));
//     cluster_counters = std::vector<int>(k, 0);
//
// #pragma omp parallel num_threads(threads) default(none) \
//     shared(chunk, k, updated_centroids, cluster_counters, point_size)
//     {
//       std::vector<int> private_counts(k, 0);
//       std::vector<Point> private_centroids(k, Point(point_size));
// #pragma omp for nowait schedule(static, chunk)
//       for (auto &point : data) {
//         int new_cluster = min_distance_cluster(point);
//         point.setCluster(new_cluster);
//         private_centroids[new_cluster] += point;
//         private_counts[new_cluster]++;
//       }
// #pragma omp critical
//       {
//         for (int p = 0; p < k; ++p) {
//           updated_centroids[p] += private_centroids[p];
//           cluster_counters[p] += private_counts[p];
//         }
//       }
//     }
//     for (int j = 0; j < k; ++j) {
//       if (cluster_counters[j] > 0)
//         updated_centroids[j] = updated_centroids[j] / cluster_counters[j];
//     }
//
//     if (not_changed(updated_centroids))
//       return;
//     centroids = updated_centroids;
//   }
// }
//
// VERSION 1:
// void ParallelKmeans::parallelkMeansClustering(int max_epochs, int k,
//                                               int threads) {
//   // int max_threads = omp_get_max_threads();
//   int chunk = ceil(static_cast<double>(data.size()) / threads);
//   centroids = random_choice(k);
//   int point_size = static_cast<int>(centroids[0].getCoordinates().size());
//
//   // These will be the final, merged results for each epoch
//   std::vector<Point> final_centroids(k, Point());
//   std::vector<int> final_counters(k, 0);
//
//   for (int i = 0; i < max_epochs; ++i) {
//     // Reset final sums for the current epoch
//     final_centroids = std::vector<Point>(k, Point());
//     final_counters = std::vector<int>(k, 0);
//
//     // This is the key change: create a shared array to hold each thread's
//     // private results. Each thread will write to its own slot, so there is
//     // race condition.
//     std::vector<std::vector<Point>> private_centroids_per_thread(
//         threads, std::vector<Point>(k, Point()));
//     std::vector<std::vector<int>> private_counts_per_thread(
//         threads, std::vector<int>(k, 0));
//
// #pragma omp parallel num_threads(threads) default(none) \
//     shared(chunk, k, point_size, private_centroids_per_thread, \
//                private_counts_per_thread)
//     {
//       int thread_id = omp_get_thread_num();
//
//       // Each thread works on its own copy, no need for critical sections
//       std::vector<Point> &my_private_centroids =
//           private_centroids_per_thread[thread_id];
//       std::vector<int> &my_private_counts =
//           private_counts_per_thread[thread_id];
//
// #pragma omp for schedule(static, \
//                              chunk) // 'nowait' is removed as we need all
//                                     // threads to finish before the next step
//       for (auto &point : data) {
//         int new_cluster = min_distance_cluster(point);
//         point.setCluster(new_cluster);
//         my_private_centroids[new_cluster] += point;
//         my_private_counts[new_cluster]++;
//       }
//     } // ---- End of parallel region ----
//
//     // Now, merge the results from all threads. This is done in serial,
//     // but it's very fast and contention-free.
//     for (int t = 0; t < threads; ++t) {
//       for (int j = 0; j < k; ++j) {
//         final_centroids[j] += private_centroids_per_thread[t][j];
//         final_counters[j] += private_counts_per_thread[t][j];
//       }
//     }
//
//     // Update the centroids based on the merged results
//     for (int j = 0; j < k; ++j) {
//       if (final_counters[j] > 0)
//         final_centroids[j] = final_centroids[j] / final_counters[j];
//     }
//
//     if (not_changed(final_centroids))
//       return;
//     centroids = final_centroids;
//   }
// }
// In ParallelKmeans.cpp

// VERSION 2
// void ParallelKmeans::parallelkMeansClustering(int max_epochs, int k,
//                                               int threads) {
//   if (data.empty())
//     return;
//
//   // Initialize centroids once
//   centroids = random_choice(k);
//
//   const int n = data.size();
//   const int dim = centroids[0].getCoordinates().size();
//
//   // --- FIX 1: Allocate all temporary storage ONCE, outside the main loop
//   --- std::vector<Point> new_centroids(k, Point()); std::vector<int>
//   cluster_counters(k, 0);
//
//   for (int i = 0; i < max_epochs; ++i) {
//     // --- FIX 1 (cont.): Reset storage instead of re-allocating ---
//     // Use std::fill for efficient reset
//     for (auto &p : new_centroids) {
//       p.reset();
//     }
//     std::fill(cluster_counters.begin(), cluster_counters.end(), 0);
//
// #pragma omp parallel num_threads(threads)
//     {
//       // --- FIX 2: Use truly thread-private storage to avoid false sharing
//       ---
//       // These are created once per thread for the duration of the parallel
//       // region.
//       std::vector<Point> private_centroids(k, Point());
//       std::vector<int> private_counts(k, 0);
//
// // --- FIX 3: Use a standard, index-based for loop for OpenMP ---
// #pragma omp for schedule(static) nowait
//       for (size_t j = 0; j < n; ++j) {
//         int cluster_id = min_distance_cluster(data[j]);
//         data[j].setCluster(
//             cluster_id); // This is a potential race condition if Point
//             objects
//                          // are shared, but data is a copy.
//         private_centroids[cluster_id] += data[j];
//         private_counts[cluster_id]++;
//       }
//
// // --- FIX 2 (cont.): Efficient reduction using a critical section ---
// // Each thread enters this section only ONCE, so contention is minimal.
// #pragma omp critical
//       {
//         for (int c = 0; c < k; ++c) {
//           new_centroids[c] += private_centroids[c];
//           cluster_counters[c] += private_counts[c];
//         }
//       }
//     } // --- End of parallel region ---
//
// // Final update step (can be parallelized too, as it's independent per
// centroid) #pragma omp parallel for num_threads(threads)
//     for (int j = 0; j < k; ++j) {
//       if (cluster_counters[j] > 0) {
//         new_centroids[j] = new_centroids[j] / cluster_counters[j];
//       }
//     }
//
//     // Check for convergence and update main centroids
//     if (not_changed(new_centroids)) {
//       centroids = new_centroids;
//       return;
//     }
//     centroids = new_centroids;
//   }
// }

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

// You must also update the signature for min_distance_cluster
// This version is now "pure" - its result only depends on its inputs.
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
