#include "DatasetLoader.h"
#include "Game/Game.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    const std::string datasetPath = argc > 1 ? argv[1] : "resources/movies.csv";
    MovieGraph graph;

    std::cout << "Loading movie dataset from " << datasetPath << "...\n";
    if (!DatasetLoader::loadDataset(datasetPath, graph))
    {
        std::cerr << "Failed to load the movie dataset.\n";
        return 1;
    }

    std::cout << "Loaded " << graph.actorCount() << " actors and "
              << graph.movieCount() << " movies.\n";
    runMainMenu(graph);
    return 0;
}