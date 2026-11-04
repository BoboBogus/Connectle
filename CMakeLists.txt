# made by Jayden Valera
cmake_minimum_required(VERSION 3.12)
project(MovieSearchProject)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(movie_test
    src/test.cpp
    src/MovieGraph.cpp
    src/helpers/CsvParser.cpp
    src/DatasetLoader.cpp
    src/helpers/TextHelper.cpp
)

add_executable(main
    src/main.cpp
    src/Game/Game.cpp
    src/MovieGraph.cpp
    src/helpers/CsvParser.cpp
    src/DatasetLoader.cpp
    src/helpers/TextHelper.cpp
    src/Search.cpp
)
target_include_directories(movie_test PRIVATE src)
target_include_directories(movie_test PRIVATE src/helpers)
target_include_directories(main PRIVATE src src/helpers)

file(COPY resources/movies.csv DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources)