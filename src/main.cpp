
#include <iostream>
#include "../include/Dataset.h"

int main()
{
    Dataset d;
    /*
    std::string pathname = "../data/Iris.csv";
    d.loadDataFromCsv(pathname);
    d.logDataset();
     */
    d.generateSyntheticDataset(50,2,-10,10);
    d.logDataset();


}