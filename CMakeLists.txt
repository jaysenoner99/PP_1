cmake_minimum_required(VERSION 3.22)
project(PP_1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Xpreprocessor -fopenmp -lomp")


find_package(Matplot++ REQUIRED)
include_directories(/usr/local/opt/libomp/include)
link_directories(/usr/local/opt/libomp/lib)
set(SOURCE_FILES src/main.cpp src/Kmeans.cpp include/Kmeans.h include/Point.h include/Dataset.h src/Point.cpp src/Dataset.cpp
        include/ParallelKmeans.h
        src/ParallelKmeans.cpp)
add_executable(PP_1 ${SOURCE_FILES})


add_executable(PP_1_parallel src/main_parallel.cpp)
target_link_libraries(PP_1 PRIVATE Matplot++::matplot)
target_link_libraries(PP_1_parallel PRIVATE Matplot++::matplot)




