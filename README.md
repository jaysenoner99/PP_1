# Parallel KMeans Clustering in C++ with OpenMP

This repository contains a high-performance C++ implementation of the KMeans clustering algorithm, parallelized using the OpenMP API for shared-memory systems. The project includes both a cache-optimized sequential version for baseline comparison and a highly-tuned parallel version.

The focus of this work is not just on parallelization, but on a holistic optimization approach that considers the interplay between the algorithm, memory access patterns, cache hierarchy, and the physical architecture of the CPU.

## Features

- **Efficient Parallel Implementation:** Uses OpenMP to parallelize the most computationally intensive phases of the KMeans algorithm.
- **Cache-Aware Design:** Employs thread-local copies of centroids to maximize cache hits and minimize high-latency main memory traffic during the assignment step.
- **Optimized Sequential Baseline:** The sequential version is also cache-optimized, ensuring a fair "apples-to-apples" comparison for accurate speedup measurement.
- **Manual Reduction Pattern:** Utilizes a highly efficient reduction pattern with thread-private storage and a minimal critical section to resolve update-step contention without performance bottlenecks.
- **Performance Benchmarking Suite:** Includes a robust testing framework to measure and plot speedup across various problem sizes (`n`), cluster counts (`k`), and thread counts (`P`).
- **CMake Build System:** Provides a clean, cross-platform build process using CMake.

## Project Structure

```
/
├── CMakeLists.txt  
├── README.md  
├── include/  
│   ├── Kmeans.h  
│   ├── ParallelKmeans.h  
│   ├── Point.h  
│   └── Dataset.h  
├── src/  
│   ├── Kmeans.cpp  
│   ├── ParallelKmeans.cpp  
│   ├── main.cpp  
│   ├── Dataset.cpp  
│   ├── Point.cpp  
│   └── main_parallel.cpp  
└── plots/  
    └── (Generated plots and CSVs will appear here)
```

## Getting Started

### Prerequisites

To build and run this project, you will need the following installed on your system:

- **A C++17 Compiler:** Such as `g++` or `clang++`.
- **CMake:** Version 3.10 or higher.
- **OpenMP Support:** Your compiler must support OpenMP. This is standard on most modern compilers and enabled by the `-fopenmp` flag in the `CMakeLists.txt`.


### Building the Project

The project uses a standard out-of-source CMake build process.

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/jsenoner99/PP_1.git
    cd PP_1
    ```

2.  **Create a build directory:**
    ```bash
    mkdir build
    cd build
    ```

3.  **Configure with CMake:**
    Run cmake from the `build` directory. This command will also fetch and prepare the `matplot++` dependency.
    ```bash
    # This will configure the project and generate the Makefile
    cmake ..
    ```

4.  **Compile the code:**
    ```bash
    # This will build the executable
    make
    ```
    An executable (e.g., `PP_1_parallel`) will be created in the `build` directory.

## Running the Code

After successfully building the project, you can run the main executable from the `build` directory.

```bash
cd build
./PP_1_parallel
```
## Usage

By default, the program will execute the full test suite (`run_multiple_tests`):

- It will run benchmarks for various dataset sizes (`n`) and cluster counts (`k`).
- It will measure the average execution time for the sequential version and the parallel version across all available threads (from 2 to the maximum on your machine).
- It will calculate the speedup and generate plots, saving them to the `plots/` directory in the project's root folder.
- It will also generate CSV files (`clustered_data.csv` and `clustered_data_sequential.csv`) with the final cluster assignments from a sample run for visual validation.

The terminal will print progress bars and status updates throughout the execution.

---

## Performance Results

The implementation was tested on an **Intel Core i7-10700K CPU** (8 Cores, 16 Threads). The results demonstrate excellent scalability that is directly correlated with the hardware architecture.

- A maximum speedup of approximately **8.5×** was achieved.
- The performance scales nearly linearly up to 8 threads, corresponding to the number of physical cores.
- Beyond 8 threads, a distinct "knee" in the performance curve shows the more modest gains from Hyper-Threading, as expected for a compute-intensive workload.

The efficiency was shown to increase with larger problem sizes (`n`) and higher cluster counts (`k`), confirming the effectiveness of the parallel design for large-scale problems.

A full technical report written in the English language is available at the root directory of the project under the name `report.pdf`. Below follows the abstract of such report:

> This report presents a comparative study between sequential and OpenMP-parallelized implementations of the k-means clustering algorithm. The aim is to assess the impact of parallelization on performance, particularly in terms of execution time and scalability, while preserving clustering accuracy. The parallel version employs OpenMP to accelerate the most computationally intensive phases of the algorithm: the assignment of data points to clusters and the update of cluster centroids. Results show that the OpenMP-parallel implementation achieves significant performance improvements over the sequential version on multi-core systems, especially for larger datasets, without compromising clustering quality. The findings underline the practical benefits and limitations of using shared-memory parallelism in iterative machine learning algorithms.*
