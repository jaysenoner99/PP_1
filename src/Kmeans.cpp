//
// Created by jayse on 05/09/2024.
//

#include <utility>

#include "../include/Kmeans.h"

Kmeans::Kmeans(std::vector<Point>& data) : data(std::move(data)) {};

void Kmeans::kMeansClustering(int epochs, int k) {

    centroids = random_choice(k);

    for(auto data_point : data){
        data_point.setCluster(compute_min_distance(data_point));
    }

    //Update the centroids
    std::vector<int> counters(k,0);
    for(const auto& data_point: data){
        int cluster_id = data_point.getCluster();
        centroids[cluster_id] += data_point;
        counters[cluster_id]++;
    }
    for(int i = 0; i < centroids.size(); ++i)
    {
        centroids[i] = centroids[i]/counters[i];
    }

}


double Kmeans::compute_min_distance(const Point& p){
    std::vector<double> distances;
    distances.reserve(centroids.size());
    for(const auto& centroid: centroids){
        distances.push_back(centroid.distance(p));
    }
    auto min = std::min_element(distances.begin(),distances.end());
    return std::distance(distances.begin(),min);
}





std::vector<Point> Kmeans::random_choice(int k) {

    int totalPoints = data.size();
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