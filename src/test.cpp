//Temp testing folder by Jayden Valera
#include <iostream>
#include "MovieGraph.h"
#include "DatasetLoader.h"
#include "TextHelper.h"

int main() {
    MovieGraph graph;
    std::string filename = "../resources/movies.csv";

    std::cout << "----------------------------------------\n";
    std::cout << "Loading dataset from " << filename << "...\n";
    std::cout << "----------------------------------------\n";

    if (!DatasetLoader::loadDataset(filename, graph)) {
        std::cerr << "Failed to load dataset\n";
        return 1;
    }

    std::cout << "Successfully loaded dataset\n";
    std::cout << " - Total Unique Actors: " << graph.actorCount() << "\n";
    std::cout << " - Total Unique Movies: " << graph.movieCount() << "\n\n";

    // Test looking up an actor (John Goodman from your sample CSV)
    std::string testActor = "JohnGoodman";
    std::cout << "Testing Actor Lookup: [" << testActor << "]\n";
    if (graph.hasActor(testActor)) {
        std::cout << "  -> Found! Movies they starred in:\n";
        for (const auto& movie : graph.moviesOf(testActor)) {
            std::cout << "     * " << movie << "\n";
        }
    } else {
        std::cout << "  -> Actor not found in graph.\n";
    }

    // Test looking up a movie (Coyote Ugly)
    std::string testMovie = "Coyote Ugly";
    std::cout << "\nTesting Movie Lookup: [" << testMovie << "]\n";
    if (graph.hasMovie(testMovie)) {
        std::cout << "  -> Found! Cast list:\n";
        for (const auto& actor : graph.castOf(testMovie)) {
            std::cout << "     * " << actor << "\n";
        }
    } else {
        std::cout << "  -> Movie not found in graph.\n";
    }

    std::cout << "\n----------------------------------------\n";
    std::cout << "Database tests completed successfully\n";
    std::cout << "----------------------------------------\n";

    return 0;
}