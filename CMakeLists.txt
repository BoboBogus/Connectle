cmake_minimum_required(VERSION 3.12)
project(MovieSearchProject)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add your executable and all source files
add_executable(movie_test
    src/test.cpp
    src/MovieGraph.cpp
    src/CsvParser.cpp
    src/DatasetLoader.cpp
    src/TextHelper.cpp
)
target_include_directories(movie_test PRIVATE src)