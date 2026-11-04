//made By Jayden Valera
#pragma once
#include <string>
#include "MovieGraph.h"

class DatasetLoader {
public:
    // Loads dataset from CSV file and populates the provided MovieGraph.
    // Returns true if successful, false otherwise.
    static bool loadDataset(const std::string& filename, MovieGraph& graph);
};