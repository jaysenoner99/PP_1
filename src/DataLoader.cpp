//
// Created by jayse on 05/09/2024.
//
#include "../include/DataLoader.h"

DataLoader::DataLoader(std::vector<Point> &data) : dataset(data) {}

DataLoader::DataLoader() = default;

bool DataLoader::loadDataFromCsv(const std::string& path) {

    std::ifstream file(path);

    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
         std::vector<double> row;
         std::stringstream ss(line);
         std::string value;

         while (std::getline(ss, value, ',')) {
             try {
                 row.push_back(std::stod(value));
             } catch (const std::invalid_argument& e) {
                 std::cerr << "Invalid number format: " << value << std::endl;
             }
         }


         dataset.emplace_back(row);
     }

     // Close the file
     file.close();
     return true;
}



// Function to generate a synthetic dataset
void DataLoader::generateSyntheticDataset(int numExamples, int dimension, double minValue, double maxValue) {
    // Random number generation setup
    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 gen(rd()); // Seed the generator
    std::uniform_real_distribution<> dis(minValue, maxValue); // Define the range

    for (int i = 0; i < numExamples; ++i) {
        std::vector<double> coords(dimension);
        for (int j = 0; j < dimension; ++j) {
            coords[j] = dis(gen);
        }
        dataset.emplace_back(coords);
    }
}

