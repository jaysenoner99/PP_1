
#include <iostream>
#include "../include/DataLoader.h"

int main()
{
    DataLoader d;
    std::string pathname = "data/Iris.csv";
    d.loadDataFromCsv(pathname);



}