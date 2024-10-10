//
// Created by Jay Senoner on 10/10/24.
//
#include "../include/ParallelKmeans.h"

ParallelKmeans::ParallelKmeans(const Dataset &d) {
    data = d.getDataset();
}

void ParallelKmeans::parallelkMeansClustering(int max_epochs, int k) {
    bool object_swapping = false;
    int epoch_count = 0;
    //Randomly choose k centroids with uniform probability from the data points
    centroids = random_choice(k);
    while(epoch_count < max_epochs) {
        object_swapping = false;
        //For each point, assign it to the cluster of its nearest centroid and check if the cluster of that point changed
        for (auto& data_point: data) {
            int new_cluster = min_distance_cluster(data_point);
            if(new_cluster != data_point.getCluster()){
                object_swapping = true;
            }
            data_point.setCluster(new_cluster);
        }
        //Before updating the centorids, check if at least one object has changed cluster.
        // If no object has changed its cluster, terminate the execution of the algorithm.
        if(!object_swapping) break;

        //Update the centroids
        std::vector<int> counters(k, 0);
#pragma omp parallel for
        for (const auto &data_point: data) {
            int cluster_id = data_point.getCluster();
            centroids[cluster_id] += data_point;
            counters[cluster_id]++;
        }
#pragma omp parallel for
        for (int i = 0; i < centroids.size(); ++i) {
            centroids[i] = centroids[i] / counters[i];
        }
        epoch_count++;
        //TODO: Maybe another stopping criterion could be added, like checking if centroids have changed enough after an iteration
    }
}

std::vector<Point> ParallelKmeans::random_choice(int k) {

    int totalPoints = (int)data.size();
    if (k > totalPoints) {
        std::cerr << "Error: k cannot be greater than the total number of points." << std::endl;
        return {};
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, totalPoints - 1);
    std::vector<Point> selectedPoints;
    std::vector<bool> chosen(totalPoints, false);
#pragma omp parallel for
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

int ParallelKmeans::min_distance_cluster(const Point &p) {
    std::vector<double> distances(centroids.size());
#pragma omp parallel for // Parallelize this loop
    for (int i = 0; i < centroids.size(); ++i) {
        distances[i] = centroids[i].distance(p);
    }
    auto min = std::min_element(distances.begin(), distances.end());
    return (int)std::distance(distances.begin(), min);
}

void ParallelKmeans::plot_clusters2d(int k) {
    using namespace matplot;

    // Enable holding to plot multiple clusters in one figure
    hold(true);

    // Define a set of distinct colors to use for clusters
    //std::vector<std::string> colors = {"r", "g", "b", "m", "y", "c"};
    std::vector<std::string> colors = {
            "r", "g", "b", "m", "y", "c", "k","w"
    };
    // Prepare a vector of x, y coordinates for each cluster


    for (int cluster_id = 0; cluster_id < k; ++cluster_id) {
        std::vector<double> x_coords, y_coords;

        // Collect all points for the current cluster
        for (const auto& point : data) {
            if (point.getCluster() == cluster_id) {
                const auto& coordinates = point.getCoordinates();
                if (coordinates.size() != 2) {
                    throw std::invalid_argument("All points must have 2 dimensions (x, y).");
                }
                // Add the (x, y) coordinates to the vectors
                x_coords.push_back(coordinates[0]);
                y_coords.push_back(coordinates[1]);
            }
        }

        // Plot the points for the current cluster using a unique color
        scatter(x_coords, y_coords)->marker_size(5).marker_color(colors[cluster_id % colors.size()]);
    }

    hold(false);

    show();
}
