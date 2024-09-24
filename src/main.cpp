
#include <iostream>
#include "../include/Dataset.h"
#include "../include/Kmeans.h"

int main()
{
    Dataset d;
    d.generateSyntheticDataset(50,2,-10,10);
    std::vector<Point> data_points = d.getDataset();



}