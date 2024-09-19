//
// Created by jayse on 05/09/2024.
//
#include <vector>
#include "Point.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#ifndef PP_1_DATALOADER_H
#define PP_1_DATALOADER_H


class DataLoader {
    public:
        explicit DataLoader(std::vector<Point> &data);
        DataLoader();
        bool loadDataFromCsv(const std::string& path);
        void generateSyntheticDataset(int num_data, int dim_data, double minValue, double maxValue);
        void logDataset();
    private:
        std::vector<Point> dataset;

};



#endif //PP_1_DATALOADER_H
