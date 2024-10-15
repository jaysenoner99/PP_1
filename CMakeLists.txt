cmake_minimum_required(VERSION 3.22)
project(PP_1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Xpreprocessor -fopenmp -lomp")

find_package(Matplot++ REQUIRED)
include_directories(/usr/local/opt/libomp/include)
link_directories(/usr/local/opt/libomp/lib)

# Define common source files (without main.cpp)
set(COMMON_SOURCE_FILES src/Kmeans.cpp src/Point.cpp src/Dataset.cpp src/ParallelKmeans.cpp include/Kmeans.h include/Point.h include/Dataset.h include/ParallelKmeans.h)

# Executable for PP_1 (using main.cpp)
add_executable(PP_1 src/main.cpp ${COMMON_SOURCE_FILES})
target_link_libraries(PP_1 PRIVATE Matplot++::matplot)

# Executable for PP_1_parallel (using main_parallel.cpp)
add_executable(PP_1_parallel src/main_parallel.cpp ${COMMON_SOURCE_FILES})
target_link_libraries(PP_1_parallel PRIVATE Matplot++::matplot)




