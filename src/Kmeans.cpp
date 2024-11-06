//
// Created by jayse on 05/09/2024.
//

#include <utility>
#include <map>
#include "../include/Kmeans.h"




void Kmeans::kMeansClustering(int max_epochs, int k) {

    int epoch_count = 0;
    //Randomly choose k centroids with uniform probability from the data points
    centroids = random_choice(k);
    while(epoch_count < max_epochs) {
        std::vector<Point> old_centroids = centroids;
        for (auto& data_point: data) {
            int new_cluster = min_distance_cluster(data_point);
            data_point.setCluster(new_cluster);
        }

        //Update the centroids
        std::vector<int> counters(k, 0);
        for (const auto &data_point: data) {
            int cluster_id = data_point.getCluster();
            centroids[cluster_id] += data_point;
            counters[cluster_id]++;
        }
        for (int i = 0; i < centroids.size(); ++i) {
            centroids[i] = centroids[i] / counters[i];
        }

        if(not_changed(old_centroids)) return;
        epoch_count++;

    }

}




int Kmeans::min_distance_cluster(const Point &p){
    std::vector<double> distances;
    distances.reserve(centroids.size());
    for(const auto& centroid: centroids){
        distances.push_back(centroid.distance(p));
    }
    auto min = std::min_element(distances.begin(),distances.end());
    return (int)std::distance(distances.begin(),min);
}





std::vector<Point> Kmeans::random_choice(int k) {

    int totalPoints = (int)data.size();
    if (k > totalPoints) {
        std::cerr << "Error: k cannot be greater than the total number of points." << std::endl;
        return {};
    }

    std::random_device rd;   // Obtain a random number from hardware
    std::mt19937 gen(rd());  // Seed the generator
    std::uniform_int_distribution<> dist(0, totalPoints - 1); // Range is [0, totalPoints-1]
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


Kmeans::Kmeans(const Dataset& d) {
 data = d.getDataset();

}

void Kmeans::log_results() {
    for(auto point: data){
        point.log_results();
        std::cout << std::endl;
    }

}

bool Kmeans::not_changed(std::vector<Point>& old_centroids, double tol) {
    for(size_t i = 0; i < centroids.size(); ++i){
        if(centroids[i].l2normdiff(old_centroids[i]) >= tol)
            return false;
    }
    return true;
}
