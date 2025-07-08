
cmake_minimum_required(VERSION 3.22)
project(PP_1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# OpenMP support
find_package(OpenMP REQUIRED)

# Matplot++ (installed via AUR)
find_package(Matplot++ REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
# Common source files
set(COMMON_SOURCE_FILES
    src/Kmeans.cpp src/Point.cpp src/Dataset.cpp src/ParallelKmeans.cpp
    include/Kmeans.h include/Point.h include/Dataset.h include/ParallelKmeans.h
)

# Executable for main version
add_executable(PP_1 src/main.cpp ${COMMON_SOURCE_FILES})
target_link_libraries(PP_1 PRIVATE Matplot++::matplot OpenMP::OpenMP_CXX stdc++fs)

# Executable for parallel version
add_executable(PP_1_parallel src/main_parallel.cpp ${COMMON_SOURCE_FILES})
target_link_libraries(PP_1_parallel PRIVATE Matplot++::matplot OpenMP::OpenMP_CXX stdc++fs)
