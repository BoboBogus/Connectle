# Connectle 

A C++ application that parses movie datasets to construct a bipartite graph of actors and movies. The engine enables users to discover degrees of separation between actors using multiple search implementations (**BFS**, **DFS**, and **DDFS**).

---

## Table of Contents
- [Overview](#overview)
- [Project Structure](#project-structure)
- [Features & Algorithms](#features--algorithms)
- [Building & Running](#building--running)
- [Dataset Requirements](#dataset-requirements)

---

## Overview

This project loads cast and movie data from a CSV dataset, normalizes text inputs, constructs an in-memory graph representation, and provides 3 searching algorithms to find relationships and degrees of separation between actors.

---

## Project Structure

```text
Connectle/
├── CMakeLists.txt
├── README.md
├── resources/
│   ├── movies.csv
└── src/
    ├── Game/
    │   ├── Game.cpp
    │   └── Game.h
    ├── helpers/
    │   ├── CsvParser.cpp
    │   ├── CsvParser.h
    │   ├── TextHelper.cpp
    │   └── TextHelper.h
    ├── DatasetLoader.cpp
    ├── DatasetLoader.h
    ├── MovieGraph.cpp
    ├── MovieGraph.h
    ├── Search.cpp
    ├── Search.h
    ├── main.cpp
    └── test.cpp
```

---

## Features & Algorithms

- **Bipartite Graph Representation (`MovieGraph`):** Fast lookup for actor-to-movie and movie-to-cast associations using hash maps.
- **Parsing (`CsvParser` & `Text`):** Custom string normalization and CSV list parsing.
- **Search Implementations (`namespace search`):**
  - **BFS (Breadth-First Search):** Finds the guaranteed shortest degree-of-separation path between two actors.
  - **DFS (Depth-First Search):** Explores deep graph chains via LIFO stack traversal.
  - **DDFS (Double-sided Depth-First Search):** Executes simultaneous searches from both start and target nodes to optimize path discovery.

---

## Building & Running

### Requirements
- **C++ Standard:** C++17 or higher
- **Build System:** CMake 3.12+

### Run Prebuilt
**Running the Prebuilt main.exe**
```bash
.\build\Release\main.exe
```

1. **Create and enter the build directory:**
   ```bash
   mkdir build && cd build
   ```

2. **Generate build files and compile:**
   ```bash
   cmake ..
   cmake --build .
   ```

3. **Run the executable:**
   - **Linux / macOS:**
     ```bash
     ./main
     ```
   - **Windows:**
     ```powershell
	 cd build
     .\Debug\main.exe
     ```

~ **Optional Test Load Database**
- **Linux / macOS:**
     ```bash
     ./movie_test
     ```
   - **Windows:**
     ```powershell
	 cd build
     .\Debug\movie_test.exe
     ```
---

## Dataset Requirements

Place your `movies.csv` file inside the `resources/` directory located at the root of the project before running search queries.
We provided a database from https://www.kaggle.com/datasets/sharmaabhi04/100k-movies-dataset?resource=download
