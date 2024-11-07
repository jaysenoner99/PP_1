//
// Created by Jay Senoner on 10/10/24.
//

#ifdef _OPENMP
#include <omp.h> // for OpenMP library functions
#endif
#include <iostream>
#include<chrono>
#include "../include/Dataset.h"
#include "../include/ParallelKmeans.h"
#include "../include/Kmeans.h"
#include <set>
#include <string>
//Speedup = t_s / t_p

std::vector<double> run_test(int n, int k, int max_epochs, int repetitions=20){
    Dataset data(n,2,0,1000);
    Kmeans sequential(data);
    ParallelKmeans parallel(data);
    double current_time = 0;
    double sequential_time = 0;
    double parallel_time = 0;
    std::vector<double> times(omp_get_max_threads(),0);
    for(int i=0; i < repetitions;++i){
        current_time = omp_get_wtime();
        sequential.kMeansClustering(max_epochs,k);
        sequential_time = sequential_time + omp_get_wtime() - current_time;
    }
    sequential_time = sequential_time / repetitions;

    for (int threads = 1; threads < omp_get_max_threads(); ++threads) {
        for(int i=0; i < repetitions;++i) {
            current_time = omp_get_wtime();
            parallel.parallelkMeansClustering(max_epochs, k, threads);
            parallel_time = parallel_time + omp_get_wtime() - current_time;
            //Compute speed up for each execution with different thread numbers
        }
        parallel_time = parallel_time / repetitions;
        times[threads] = sequential_time / parallel_time;
    }
    return times;
}


std::set<std::vector<double>> run_multiple_tests(int max_n,int k, int max_epochs){
    std::set<std::vector<double>> speedups;
    for(int n=100; n < max_n; n = n + 9*n){
       std::vector<double> times = run_test(n,k,max_epochs);
       speedups.insert(times);
       std::cout << "Testing for " + std::to_string(n) + "elements" << std::endl;
    }
    using namespace matplot;
    //Ensures that the plots directory exists inside the project directory
    std::filesystem::create_directory("plots");
    auto plt = plot(speedups);
    for(const auto& line: plt){
       line -> line_width(3.0);
    }
    std::vector<std::string> labels;
    for(int n=100; n < max_n; n = n + 9*n){
        labels.push_back("n=" + std::to_string(n));
    }
    // Set labels for each line
    matplot::legend(labels);

    // Set x and y axis labels
    xlabel("P");
    ylabel("Speedup(t_s/t_P)");

    // Set plot title
    title("Speedup, k=" + std::to_string(k));
    matplot::save("../plots/speedupk" + std::to_string(k) + ".png");
    show();
}
int main(){
/*
    std::cout << "running parallel k means" << std::endl;
    int k = 7;
    Dataset d;
    d.generateSyntheticDataset(50000,2,0,1000);
    ParallelKmeans kmeans(d);
    int max_threads = omp_get_max_threads();
    auto start = std::chrono::high_resolution_clock::now();
    kmeans.parallelkMeansClustering(1000,k,max_threads);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Duration:" << duration.count() << "seconds" << std::endl;
    kmeans.plot_clusters2d(k);
*/
    std::cout <<"Maximum number of threads for this machine:" + std::to_string(omp_get_max_threads()) << std::endl;
    std::set<std::vector<double>> speedups = run_multiple_tests(100000,5,10000);
}