//
// Created by jayse on 05/09/2024.
//


#ifndef PP_1_DATALOADER_H
#define PP_1_DATALOADER_H

#include <vector>
#include "Point.h"
#include "Kmeans.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>

class Dataset {
    public:
        explicit Dataset(std::vector<Point> &data);
        Dataset();
        Dataset(int num_data, int dim_data, double minValue, double maxValue);
        bool loadDataFromCsv(const std::string& path);
        void generateSyntheticDataset(int num_data, int dim_data, double minValue, double maxValue);
        void logDataset();

        std::vector<Point> getDataset() const;

private:
        std::vector<Point> dataset;

};



#endif //PP_1_DATALOADER_H
