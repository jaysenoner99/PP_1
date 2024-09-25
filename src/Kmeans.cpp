//
// Created by jayse on 05/09/2024.
//

#include <utility>
#include <map>
#include "../include/Kmeans.h"



Kmeans::Kmeans(std::vector<Point>& data) : data(std::move(data)) {};

void Kmeans::kMeansClustering(int max_epochs, int k) {
    bool object_swapping;
    int epoch_count = 0;
    while(epoch_count < max_epochs) {
        object_swapping = false;
        //Randomly choose k centroids with uniform probability from the data points
        centroids = random_choice(k);

        //For each point, assign it to the cluster of its nearest centroid and check if the cluster of that point changed
        for (auto data_point: data) {
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
        //TODO:Check if the centroids have changed enough
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


void plotPoints(const std::vector<Point>& points) {
    std::map<int, std::vector<double>> x_coords;  // Map cluster -> x-coordinates
    std::map<int, std::vector<double>> y_coords;  // Map cluster -> y-coordinates

    // Populate maps with points based on their clusters
    for (const auto& point : points) {
        int cluster = point.getCluster();
        x_coords[cluster].push_back(point.getCoordinates()[0]);
        y_coords[cluster].push_back(point.getCoordinates()[1]);
    }

    // Plot each cluster with a different color
    for (const auto& entry : x_coords) {
        int cluster = entry.first;
        plt::scatter(x_coords[cluster], y_coords[cluster], 10);  // 10 is the marker size
    }

    plt::xlabel("X");
    plt::ylabel("Y");
    plt::title("K-Means Clustering Result");
    plt::show();
}