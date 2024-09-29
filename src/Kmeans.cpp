//
// Created by jayse on 05/09/2024.
//

#include <utility>
#include <map>
#include "../include/Kmeans.h"




void Kmeans::kMeansClustering(int max_epochs, int k) {

    // TEST: log all point coordinates


    bool object_swapping = false;
    int epoch_count = 0;
    while(epoch_count < max_epochs) {
        object_swapping = false;
        //Randomly choose k centroids with uniform probability from the data points
        centroids = random_choice(k);

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
        for (const auto &data_point: data) {
            int cluster_id = data_point.getCluster();
            centroids[cluster_id] += data_point;
            counters[cluster_id]++;
        }
        for (int i = 0; i < centroids.size(); ++i) {
            centroids[i] = centroids[i] / counters[i];
        }
        epoch_count++;
        //TODO:Check if the centroids haven't changed enough
    }


    for(auto& point: data){
        point.log_results();
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
    std::vector<std::string> colors = {"r", "g", "b", "m", "y", "c"};

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

    // Release the hold so that future plots overwrite this plot
    hold(false);

    // Show the plot
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
