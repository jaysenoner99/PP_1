
#include <iostream>
#include "../include/Dataset.h"
#include "../include/Kmeans.h"

int main()
{
    int k = 5;
    Dataset d;
    d.generateSyntheticDataset(50000,2,0,1000);
    Kmeans kmeans(d);
    kmeans.kMeansClustering(1000,k);
    kmeans.plot_clusters2d(k);

}