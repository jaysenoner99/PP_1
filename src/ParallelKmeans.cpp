//
// Created by Jay Senoner on 10/10/24.
//
#include "../include/ParallelKmeans.h"

ParallelKmeans::ParallelKmeans(const Dataset &d) {
    data = d.getDataset();
}

void ParallelKmeans::parallelkMeansClustering(int max_epochs, int k, int threads) {
    //int max_threads = omp_get_max_threads();
    int chunk = ceil(static_cast<double>(data.size()) / threads);
    centroids = random_choice(k);
    int point_size = static_cast<int>(centroids[0].getCoordinates().size());
    std::vector<Point> updated_centroids(k,Point(point_size));
    std::vector<int> cluster_counters(k,0);

    for(int i = 0; i < max_epochs; ++i){
        updated_centroids = std::vector<Point>(k,Point(point_size));
        cluster_counters = std::vector<int>(k,0);

#pragma omp parallel num_threads(threads)  default(none) shared(chunk,k,updated_centroids,cluster_counters,point_size)
        {
            std::vector<int> private_counts(k, 0);
            std::vector<Point> private_centroids(k, Point(point_size));
#pragma omp for nowait schedule(static, chunk)
            for (auto& point: data) {
                int new_cluster = min_distance_cluster(point);
                point.setCluster(new_cluster);
                private_centroids[new_cluster] += point;
                private_counts[new_cluster]++;
            }
#pragma omp critical
            {
                for (int p = 0; p < k; ++p) {
                    updated_centroids[p] += private_centroids[p];
                    cluster_counters[p] += private_counts[p];
                }
            }
        }
        for(int j = 0 ; j < k; ++j)
        {
            if(cluster_counters[j] > 0)
                updated_centroids[j] = updated_centroids[j]/ cluster_counters[j];
        }

        if(not_changed(updated_centroids)) return;
        centroids = updated_centroids;


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

bool ParallelKmeans::not_changed(std::vector<Point>& old_centroids, double tol) {
    for(size_t i = 0; i < centroids.size(); ++i){
        if(centroids[i].distance(old_centroids[i]) >= tol)
            return false;
    }
    return true;
}

ParallelKmeans::ParallelKmeans(std::vector<Point>& data) {
    this->data = data;
}
