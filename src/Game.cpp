#include "Game.h"
#include "Search.h"
#include "TextHelper.h"

#include <iostream>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

namespace
{
    bool findActorName(const MovieGraph& graph, const std::string& typedName, std::string& actorName)
    {
        actorName = text::formatForDataset(text::trim(typedName));
        return !actorName.empty() && graph.hasActor(actorName);
    }

    bool findMovieTitle(const MovieGraph& graph, const std::string& typedTitle, std::string& movieTitle)
    {
        movieTitle = text::trim(typedTitle);
        return !movieTitle.empty() && graph.hasMovie(movieTitle);
    }

    bool actorIsInMovie(const MovieGraph& graph, const std::string& actorName, const std::string& movieTitle)
    {
        const std::vector<std::string>& cast = graph.castOf(movieTitle);
        for (const std::string& actor : cast)
        {
            if (actor == actorName)
            {
                return true;
            }
        }
        return false;
    }

    void printPath(const std::vector<std::string>& path)
    {
        for (std::size_t position = 0; position < path.size(); ++position)
        {
            if (position % 2 == 0)
            {
                std::cout << text::formatForDisplay(path[position]);
            }
            else
            {
                std::cout << '[' << path[position] << ']';
            }
            if (position + 1 < path.size())
            {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl;
    }

    std::size_t countMoviesInPath(const std::vector<std::string>& path)
    {
        return path.empty() ? 0 : (path.size() - 1) / 2;
    }

    bool askForActor(const MovieGraph& graph, const std::string& prompt, std::string& actorName)
    {
        while (true)
        {
            std::cout << prompt;
            std::string typedName;
            if (!std::getline(std::cin, typedName))
            {
                return false;
            }
            if (text::normalize(text::trim(typedName)) == "quit")
            {
                return false;
            }
            if (findActorName(graph, typedName, actorName))
            {
                return true;
            }
            std::cout << "Actor not found. Check the spelling or type 'quit' to return.\n";
        }
    }

    void printSearchResult(const std::string& algorithmName, const SearchResult& result)
    {
        std::cout << "\n--- " << algorithmName << " ---\n";
        if (!result.found)
        {
            std::cout << "No path found between these actors.\n";
        }
        else
        {
            printPath(result.path);
            std::cout << "Movies apart: " << countMoviesInPath(result.path) << '\n';
        }
        std::cout << "Nodes visited: " << result.nodesVisited << '\n'
                  << "Time: " << result.milliseconds << " ms\n";
    }

    bool pickRandomConnectedActors(const MovieGraph& graph,
                                   std::string& firstActor,
                                   std::string& secondActor)
    {
        const std::vector<std::string> actors = graph.allActors();
        if (actors.size() < 2)
        {
            return false;
        }

        std::random_device seedSource;
        std::mt19937 randomEngine(seedSource());
        std::uniform_int_distribution<std::size_t> pickActor(0, actors.size() - 1);

        for (int attempt = 0; attempt < 50; ++attempt)
        {
            const std::string candidateStart = actors[pickActor(randomEngine)];
            const std::string candidateEnd = actors[pickActor(randomEngine)];
            if (candidateStart == candidateEnd)
            {
                continue;
            }

            const SearchResult path = runBidirectionalBFS(graph, candidateStart, candidateEnd);
            if (path.found && countMoviesInPath(path.path) >= 2)
            {
                firstActor = candidateStart;
                secondActor = candidateEnd;
                return true;
            }
        }
        return false;
    }
}

void runShortestPathMode(const MovieGraph& graph)
{
    std::cout << "\n=== Shortest Movie Path Finder ===\n";
    std::string startActor;
    std::string endActor;
    if (!askForActor(graph, "Enter the first actor (or 'quit'): ", startActor) ||
        !askForActor(graph, "Enter the second actor (or 'quit'): ", endActor))
    {
        return;
    }

    std::cout << "\nWhich algorithm?\n"
              << "  1. Breadth-First Search\n"
              << "  2. Bidirectional Breadth-First Search\n"
              << "  3. Run both and compare\n"
              << "Choice: ";
    std::string choice;
    if (!std::getline(std::cin, choice))
    {
        return;
    }
    choice = text::trim(choice);

    if (choice == "1" || choice == "3")
    {
        printSearchResult("Breadth-First Search", runBFS(graph, startActor, endActor));
    }
    if (choice == "2" || choice == "3")
    {
        printSearchResult("Bidirectional BFS", runBidirectionalBFS(graph, startActor, endActor));
    }
    if (choice != "1" && choice != "2" && choice != "3")
    {
        std::cout << "That was not a valid choice.\n";
    }
}

void runTriviaMode(const MovieGraph& graph)
{
    std::cout << "\n=== Trivia: Connect the Actors ===\n";
    std::string startActor;
    std::string targetActor;
    if (!pickRandomConnectedActors(graph, startActor, targetActor))
    {
        std::cout << "Could not find a suitable connected actor pair. Try again.\n";
        return;
    }

    std::cout << "Connect " << text::formatForDisplay(startActor) << " to "
              << text::formatForDisplay(targetActor) << ".\n"
              << "Enter a movie featuring the current actor, then another actor from it.\n"
              << "Type 'giveup' at either prompt to reveal the shortest path.\n";

    std::unordered_set<std::string> usedActors{startActor};
    std::unordered_set<std::string> usedMovies;
    std::string currentActor = startActor;
    std::size_t moviesUsed = 0;
    bool connected = false;

    while (true)
    {
        std::cout << "\nCurrent actor: " << text::formatForDisplay(currentActor) << "\nMovie: ";
        std::string typedMovie;
        if (!std::getline(std::cin, typedMovie) || text::normalize(text::trim(typedMovie)) == "giveup")
        {
            break;
        }

        std::string movieTitle;
        if (!findMovieTitle(graph, typedMovie, movieTitle) ||
            !actorIsInMovie(graph, currentActor, movieTitle) || usedMovies.count(movieTitle) != 0)
        {
            std::cout << "Invalid link. Check the movie and try again.\n";
            continue;
        }

        std::cout << "Actor from " << movieTitle << ": ";
        std::string typedActor;
        if (!std::getline(std::cin, typedActor) || text::normalize(text::trim(typedActor)) == "giveup")
        {
            break;
        }

        std::string nextActor;
        if (!findActorName(graph, typedActor, nextActor) ||
            !actorIsInMovie(graph, nextActor, movieTitle) || usedActors.count(nextActor) != 0)
        {
            std::cout << "Invalid link. Choose a new actor from that movie.\n";
            continue;
        }

        usedMovies.insert(movieTitle);
        usedActors.insert(nextActor);
        currentActor = nextActor;
        ++moviesUsed;
        if (currentActor == targetActor)
        {
            connected = true;
            std::cout << "You connected them using " << moviesUsed << " movie(s)!\n";
            break;
        }
    }

    if (!connected)
    {
        std::cout << "The shortest path was:\n";
    }
    const SearchResult shortest = runBidirectionalBFS(graph, startActor, targetActor);
    if (shortest.found)
    {
        printPath(shortest.path);
    }
}

void runLongestChainMode(const MovieGraph& graph)
{
    std::cout << "\n=== Longest Chain Challenge ===\n"
              << "Enter an actor, a movie they appeared in, then a different actor from that movie.\n"
              << "No actors or movies may repeat. One invalid link ends the game.\n";
    std::string currentActor;
    if (!askForActor(graph, "Starting actor (or 'quit'): ", currentActor))
    {
        return;
    }

    std::unordered_set<std::string> usedActors{currentActor};
    std::unordered_set<std::string> usedMovies;
    std::size_t chainLength = 0;
    while (true)
    {
        std::cout << "\nCurrent actor: " << text::formatForDisplay(currentActor) << "\nMovie (or 'quit'): ";
        std::string typedMovie;
        if (!std::getline(std::cin, typedMovie) || text::normalize(text::trim(typedMovie)) == "quit")
        {
            break;
        }

        std::string movieTitle;
        if (!findMovieTitle(graph, typedMovie, movieTitle) ||
            !actorIsInMovie(graph, currentActor, movieTitle) || usedMovies.count(movieTitle) != 0)
        {
            std::cout << "Game over: invalid or repeated movie link.\n";
            break;
        }

        std::cout << "Actor from " << movieTitle << ": ";
        std::string typedActor;
        if (!std::getline(std::cin, typedActor))
        {
            break;
        }

        std::string nextActor;
        if (!findActorName(graph, typedActor, nextActor) ||
            !actorIsInMovie(graph, nextActor, movieTitle) || usedActors.count(nextActor) != 0)
        {
            std::cout << "Game over: invalid or repeated actor link.\n";
            break;
        }

        usedMovies.insert(movieTitle);
        usedActors.insert(nextActor);
        currentActor = nextActor;
        ++chainLength;
        std::cout << "Correct! Chain length: " << chainLength << '\n';
    }
    std::cout << "Final score: " << chainLength << " link(s).\n";
}

void runMainMenu(const MovieGraph& graph)
{
    while (true)
    {
        std::cout << "\n===== The Dobbinators: Movie Path Finder =====\n"
                  << "  1. Find shortest movie path\n"
                  << "  2. Trivia: connect two random actors\n"
                  << "  3. Longest chain challenge\n"
                  << "  4. Exit\nChoice: ";
        std::string choice;
        if (!std::getline(std::cin, choice))
        {
            return;
        }
        choice = text::trim(choice);
        if (choice == "1")
        {
            runShortestPathMode(graph);
        }
        else if (choice == "2")
        {
            runTriviaMode(graph);
        }
        else if (choice == "3")
        {
            runLongestChainMode(graph);
        }
        else if (choice == "4")
        {
            std::cout << "Goodbye!\n";
            return;
        }
        else
        {
            std::cout << "Please enter 1, 2, 3, or 4.\n";
        }
    }
}
