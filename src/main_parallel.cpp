//
// Created by Jay Senoner on 10/10/24.
//

#ifdef _OPENMP
#include <omp.h> // for OpenMP library functions
#endif
#include "../include/Dataset.h"
#include "../include/Kmeans.h"
#include "../include/ParallelKmeans.h"
#include <chrono>
#include <filesystem>
#include <iostream>
#include <set>
#include <string>
// Speedup = t_s / t_p

void print_progress(int current, int total, int bar_width = 50) {
  float progress = static_cast<float>(current) / total;
  int pos = static_cast<int>(bar_width * progress);

  std::cout << "[";
  for (int i = 0; i < bar_width; ++i) {
    if (i < pos)
      std::cout << "=";
    else if (i == pos)
      std::cout << ">";
    else
      std::cout << " ";
  }
  std::cout << "] " << int(progress * 100.0) << "%\r";
  std::cout.flush();
}
void run_and_save_final_result(int n, int k, int max_epochs, int threads) {
  std::cout << "Generating final clustered output for plotting..." << std::endl;

  // 1. Create the pristine dataset.
  Dataset data(n, 2, 0, 1000);

  // 2. Create a ParallelKmeans object.
  ParallelKmeans kmeans_solver(data);

  // 3. Run the clustering algorithm to completion.
  kmeans_solver.parallelkMeansClustering(max_epochs, k, threads);

  // 4. Use the new getter to retrieve the final, solved data.
  const std::vector<Point> &final_data = kmeans_solver.getClusteredData();
  std::vector<Point> final_data_copy = final_data;
  Dataset clustered_data(final_data_copy);

  clustered_data.saveToCsv("../plots/clustered_data.csv");

  Kmeans kmeans_seq_solver(data);

  kmeans_seq_solver.kMeansClustering(max_epochs, k);

  const std::vector<Point> &final_data_seq =
      kmeans_seq_solver.getClusteredData();
  std::vector<Point> final_data_seq_copy = final_data_seq;
  Dataset clustered_data_seq(final_data_seq_copy);

  clustered_data_seq.saveToCsv("../plots/clustered_data_sequential.csv");
}

std::vector<double> run_test(int n, int k, int max_epochs,
                             int repetitions = 20) {
  Dataset data(n, 2, 0, 1000);

  double current_time = 0;
  double sequential_time = 0;
  int max_threads = omp_get_max_threads();
  std::vector<double> times(max_threads - 1, 0);

  std::cout << "Running sequential k-means (" << repetitions << " runs):\n";
  for (int i = 0; i < repetitions; ++i) {
    print_progress(i + 1, repetitions);

    Kmeans sequential(data);
    current_time = omp_get_wtime();
    sequential.kMeansClustering(max_epochs, k);
    sequential_time += omp_get_wtime() - current_time;
  }
  sequential_time /= repetitions;
  std::cout << std::endl;

  for (int threads = 2; threads <= omp_get_max_threads(); ++threads) {
    double parallel_time = 0;
    std::cout << "Running parallel k-means with " << threads << " threads ("
              << repetitions << " runs):\n";
    for (int i = 0; i < repetitions; ++i) {
      print_progress(i + 1, repetitions);
      ParallelKmeans parallel(data);
      current_time = omp_get_wtime();
      parallel.parallelkMeansClustering(max_epochs, k, threads);
      parallel_time += omp_get_wtime() - current_time;
    }
    parallel_time /= repetitions;
    times[threads - 2] = sequential_time / parallel_time;
    std::cout << std::endl;
  }

  std::cout << "Completed run_test with n=" << n
            << ", result size: " << times.size() << std::endl;
  return times;
}

std::vector<std::vector<double>> run_multiple_tests(int max_n, int k,
                                                    int max_epochs,
                                                    int repetitions,
                                                    std::string filename) {
  std::vector<std::vector<double>> speedups;
  int max_threads = omp_get_max_threads();

  for (int n = 1000; n <= max_n; n = n + 9 * n) {
    std::cout << "Testing for " + std::to_string(n) + " elements" << std::endl;
    std::vector<double> times = run_test(n, k, max_epochs, repetitions);
    speedups.push_back(times);
  }

  using namespace matplot;
  std::filesystem::create_directory("plots");

  // Thread counts: x-axis values
  std::vector<double> thread_counts;
  for (int t = 2; t <= max_threads; ++t) {
    thread_counts.push_back(static_cast<double>(t));
  }
  hold(on);
  // Plot each speedup curve
  for (const auto &times : speedups) {
    plot(thread_counts, times)->line_width(3.0);
  }
  hold(off);
  // Set legend labels
  std::vector<std::string> labels;
  for (int n = 1000; n <= max_n; n = n + 9 * n) {
    labels.push_back("n=" + std::to_string(n));
  }

  // Legend placed outside to the right
  legend(labels);

  // Add grid
  grid(on);

  // Axis labels and title
  xlabel("Number of threads (P)");
  ylabel("Speedup (tₛ / tₚ)");
  title("Speedup, k=" + std::to_string(k));

  // Save the plot
  matplot::save(filename + std::to_string(k) + ".png");

  return speedups;
}
void save_speedups_to_csv(const std::vector<std::vector<double>> &speedups,
                          const std::string &filename) {
  std::ofstream file(filename);

  if (!file.is_open()) {
    std::cerr << "Error: Unable to open file " << filename << " for writing.\n";
    return;
  }

  // Header: n,P=2,...,P=max_threads
  file << "n";
  int max_threads = omp_get_max_threads();
  for (int i = 2; i <= max_threads; ++i) {
    file << ",P=" << i;
  }
  file << "\n";

  int n = 1000;
  for (const auto &line : speedups) {
    file << n;
    for (size_t i = 0; i < line.size(); ++i) {
      file << "," << line[i];
    }
    file << "\n";
    n = n + 9 * n;
  }

  file.close();
  std::cout << "Speedup data saved to: " << filename << std::endl;
}
int main() {
  /*
      std::cout << "running parallel k means" << std::endl;
      int k = 7;
      Dataset d;
      d.generateSyntheticDataset(50000,2,0,1000);
      ParallelKmeans kmeans(d);
      int max_threads = omp_get_max_threads();
      auto start = std::chrono::high_resolution_clock::now();
      kmeans.parallelkMeansClustering(1000,k,max_threads);
      auto end = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::seconds>(end -
     start); std::cout << "Duration:" << duration.count() << "seconds" <<
     std::endl; kmeans.plot_clusters2d(k);
  */
  std::cout << "Maximum number of threads for this machine:" +
                   std::to_string(omp_get_max_threads())
            << std::endl;
  int repetitions = 50;
  int max_n = 1000000;
  int k = 20;
  int max_epochs = 150;
  std::string filename = "../plots/speedups_v1_k=";
  std::vector<std::vector<double>> speedups =
      run_multiple_tests(max_n, k, max_epochs, repetitions, filename);
  save_speedups_to_csv(speedups, filename + std::to_string(k) + ".csv");
}
