// Created by jayse on 05/09/2024.
//

#ifndef PP_1_DATALOADER_H
#define PP_1_DATALOADER_H

#include "Point.h"
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

class Dataset {
public:
  explicit Dataset(std::vector<Point> &data);
  Dataset();
  Dataset(int num_data, int dim_data, double minValue, double maxValue);
  bool loadDataFromCsv(const std::string &path);
  void generateSyntheticDataset(int num_data, int dim_data, double minValue,
                                double maxValue);
  void logDataset();
  const std::vector<Point> &getDataset() const;

  void saveToCsv(const std::string &filename) const;

private:
  std::vector<Point> dataset;
};

#endif // PP_1_DATALOADER_H
