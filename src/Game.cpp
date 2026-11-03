#include "Game.h"
#include "Search.h"
#include "TextUtils.h"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <random>

using namespace std;

namespace{
    // Looks up an actor by typed name. Returns true and fills in actor name if found.
    bool findActorName(const NameIndex& index, const string& typedName, string& actorName)
    {
        string key = toLowerCase(trimSpaces(typedName));
        unordered_map<string, string>::const_iterator found = index.actorLowerToName.find(key);
        if (found == index.actorLowerToName.end()){
            return false;
        }
        actorName = found->second;
        return true;
    }

    // Looks up a movie by typed title. Returns true and fills movie title if found.
    bool findMovieTitle(const NameIndex& index, const string& typedTitle, string& movieTitle)
    {
        string key = toLowerCase(trimSpaces(typedTitle));
        unordered_map<string, string>::const_iterator found = index.movieLowerToTitle.find(key);
        if (found == index.movieLowerToTitle.end())
        {
            return false;
        }
        movieTitle = found->second;
        return true;
    }

    // Checks that this actor really appeared in this movie.
    bool actorIsInMovie(const MovieGraph& graph, const string& actorName, const string& movieTitle)
    {
        const vector<string>& castList = graph.castOf(movieTitle);
        for (int position = 0; position < (int)castList.size(); position++)
        {
            if (castList[position] == actorName)
            {
                return true;
            }
        }
        return false;
    }

    // Prints a path like: Actor A -> [Movie 1] -> Actor B -> [Movie 2] -> Actor C
    void printPath(const vector<string>& path){
        for (int position = 0; position < (int)path.size(); position++)
        {
            bool isActorPosition = (position % 2 == 0);

            if (isActorPosition){
                cout << path[position];
            }
            else{
                cout << "[" << path[position] << "]";
            }

            if (position < (int)path.size() - 1){
                cout << " -> ";
            }
        }
        cout << endl;
    }

    // Number of movies in a path = (nodes - 1) / 2
    int countMoviesInPath(const vector<string>& path)
    {
        if (path.empty())
        {
            return 0;
        }
        return ((int)path.size() - 1) / 2;
    }

    // Keeps asking until the user types a real actor. Returns false if they type "quit".
    bool askForActor(const NameIndex& index, const string& promptText, string& actorName)
    {
        while (true)
        {
            cout << promptText;
            string typedName;
            getline(cin, typedName);

            if (toLowerCase(trimSpaces(typedName)) == "quit")
            {
                return false;
            }
            if (findActorName(index, typedName, actorName))
            {
                return true;
            }
            cout << "  Actor not found. Check the spelling or type 'quit' to go back." << endl;
        }
    }

    void printSearchResult(const string& algorithmName, const SearchResult& result)
    {
        cout << endl << "--- " << algorithmName << " ---" << endl;
        if (!result.found)
        {
            cout << "No path found between these actors." << endl;
        }
        else
        {
            printPath(result.path);
            cout << "Movies apart:  " << countMoviesInPath(result.path) << endl;
        }
        cout << "Nodes visited: " << result.nodesVisited << endl;
        cout << "Time:          " << result.milliseconds << " ms" << endl;
    }
}

NameIndex buildNameIndex(const MovieGraph& graph)
{
    NameIndex index;

    vector<string> actorList = graph.allActors();
    for (int position = 0; position < (int)actorList.size(); position++)
    {
        index.actorLowerToName[toLowerCase(actorList[position])] = actorList[position];
    }

    vector<string> movieList = graph.allMovies();
    for (int position = 0; position < (int)movieList.size(); position++)
    {
        index.movieLowerToTitle[toLowerCase(movieList[position])] = movieList[position];
    }

    return index;
}

//Mode 1 shortest path finder
void runShortestPathMode(const MovieGraph& graph, const NameIndex& index){
    cout << endl << "=== Shortest Movie Path Finder ===" << endl;

    string startActor;
    string endActor;

    if (!askForActor(index, "Enter the first actor (or 'quit'): ", startActor)){
        return;
    }
    if (!askForActor(index, "Enter the second actor (or 'quit'): ", endActor)){
        return;
    }

    cout << endl;
    cout << "Which algorithm?" << endl;
    cout << "  1. Breadth-First Search" << endl;
    cout << "  2. Bidirectional Breadth-First Search" << endl;
    cout << "  3. Run both and compare" << endl;
    cout << "Choice: ";

    string choiceText;
    getline(cin, choiceText);
    choiceText = trimSpaces(choiceText);

    if (choiceText == "1" || choiceText == "3"){
        SearchResult bfsResult = runBFS(graph, startActor, endActor);
        printSearchResult("Breadth-First Search", bfsResult);
    }
    if (choiceText == "2" || choiceText == "3"){
        SearchResult bidirectionalResult = runBidirectionalBFS(graph, startActor, endActor);
        printSearchResult("Bidirectional BFS", bidirectionalResult);
    }
    if (choiceText != "1" && choiceText != "2" && choiceText != "3"){
        cout << "That was not a valid choice." << endl;
    }
}

namespace{
    // Picks two random actors that are guaranteed to be connected
    bool pickRandomConnectedActors(const MovieGraph& graph, string& firstActor, string& secondActor)
    {
        // Only use actors who appeared in at least one movie
        vector<string> everyActor = graph.allActors();
        vector<string> usableActors;
        for (int position = 0; position < (int)everyActor.size(); position++)
        {
            if (!graph.moviesOf(everyActor[position]).empty())
            {
                usableActors.push_back(everyActor[position]);
            }
        }

        if (usableActors.size() < 2)
        {
            return false;
        }

        random_device seedSource;
        mt19937 randomEngine(seedSource());
        uniform_int_distribution<int> indexPicker(0, (int)usableActors.size() - 1);

        // Try several times to find a pair that has a path between them
        for (int attempt = 0; attempt < 50; attempt++)
        {
            string firstCandidate = usableActors[indexPicker(randomEngine)];
            string secondCandidate = usableActors[indexPicker(randomEngine)];

            if (firstCandidate == secondCandidate)
            {
                continue;
            }

            SearchResult check = runBidirectionalBFS(graph, firstCandidate, secondCandidate);
            if (check.found && countMoviesInPath(check.path) >= 2)
            {
                firstActor = firstCandidate;
                secondActor = secondCandidate;
                return true;
            }
        }
        return false;
    }
}

//mode 2 connect 2 actors by yourslef
void runTriviaMode(const MovieGraph& graph, const NameIndex& index){
    cout << endl << "=== Trivia: Connect the Actors ===" << endl;

    string startActor;
    string targetActor;
    if (!pickRandomConnectedActors(graph, startActor, targetActor)){
        cout << "Could not find two connected actors. Try again." << endl;
        return;
    }

    cout << "Connect " << startActor << " to " << targetActor << "." << endl;
    cout << "Type a movie the current actor was in, then an actor from that movie." << endl;
    cout << "Type 'giveup' at any prompt to see the shortest answer." << endl;

    unordered_set<string> usedMovies;
    string currentActor = startActor;
    int moviesUsed = 0;

    while (true)
    {
        cout << endl << "Current actor: " << currentActor << endl;

        // Ask for a movie
        cout << "Movie: ";
        string typedMovie;
        getline(cin, typedMovie);

        if (toLowerCase(trimSpaces(typedMovie)) == "giveup"){
            break;
        }

        string movieTitle;
        if (!findMovieTitle(index, typedMovie, movieTitle)){
            cout << "  That movie is not in the database. Try again." << endl;
            continue;
        }
        if (!actorIsInMovie(graph, currentActor, movieTitle)){
            cout << "  " << currentActor << " was not in that movie. Try again." << endl;
            continue;
        }
        if (usedMovies.count(movieTitle) > 0){
            cout << "  You already used that movie. Try a different one." << endl;
            continue;
        }

        //Ask for an actor from that movie 
        cout << "Actor from " << movieTitle << ": ";
        string typedActor;
        getline(cin, typedActor);

        if (toLowerCase(trimSpaces(typedActor)) == "giveup"){
            break;
        }

        string nextActor;
        if (!findActorName(index, typedActor, nextActor)){
            cout << "  That actor is not in the database. Try again." << endl;
            continue;
        }
        if (!actorIsInMovie(graph, nextActor, movieTitle)){
            cout << "  That actor was not in " << movieTitle << ". Try again." << endl;
            continue;
        }

        usedMovies.insert(movieTitle);
        moviesUsed++;
        currentActor = nextActor;

        if (currentActor == targetActor)
        {
            cout << endl << "You connected them using " << moviesUsed << " movie(s)!" << endl;
            break;
        }
    }

    // Show the best possible answer for comparison
    SearchResult bestResult = runBidirectionalBFS(graph, startActor, targetActor);
    if (bestResult.found)
    {
        cout << endl << "Shortest possible chain (" << countMoviesInPath(bestResult.path) << " movies):" << endl;
        printPath(bestResult.path);
    }
}

// mode 3 longest chain challenge
void runLongestChainMode(const MovieGraph& graph, const NameIndex& index)
{
    cout << endl << "=== Longest Chain Challenge ===" << endl;
    cout << "Enter an actor, then a movie they were in, then a different actor" << endl;
    cout << "from that movie, and so on. No repeats! One mistake ends the game." << endl << endl;

    string currentActor;
    if (!askForActor(index, "Starting actor (or 'quit'): ", currentActor))
    {
        return;
    }

    unordered_set<string> usedActors;
    unordered_set<string> usedMovies;
    usedActors.insert(currentActor);

    int chainLength = 0;

    while (true)
    {
        cout << endl << "Current actor: " << currentActor << endl;

        // Movie step 
        cout << "Movie (or 'quit'): ";
        string typedMovie;
        getline(cin, typedMovie);

        if (toLowerCase(trimSpaces(typedMovie)) == "quit")
        {
            cout << "You stopped the chain." << endl;
            break;
        }

        string movieTitle;
        if (!findMovieTitle(index, typedMovie, movieTitle))
        {
            cout << "Game over: that movie is not in the database." << endl;
            break;
        }
        if (!actorIsInMovie(graph, currentActor, movieTitle))
        {
            cout << "Game over: " << currentActor << " was not in that movie." << endl;
            break;
        }
        if (usedMovies.count(movieTitle) > 0)
        {
            cout << "Game over: that movie was already used." << endl;
            break;
        }

        //  Actor step 
        cout << "Actor from " << movieTitle << ": ";
        string typedActor;
        getline(cin, typedActor);

        string nextActor;
        if (!findActorName(index, typedActor, nextActor))
        {
            cout << "Game over: that actor is not in the database." << endl;
            break;
        }
        if (!actorIsInMovie(graph, nextActor, movieTitle))
        {
            cout << "Game over: that actor was not in " << movieTitle << "." << endl;
            break;
        }
        if (usedActors.count(nextActor) > 0)
        {
            cout << "Game over: that actor was already used." << endl;
            break;
        }

        // Valid link extend the chain
        usedMovies.insert(movieTitle);
        usedActors.insert(nextActor);
        currentActor = nextActor;
        chainLength++;

        cout << "Correct! Chain length: " << chainLength << endl;
    }

    cout << endl << "Final score: " << chainLength << " link(s)." << endl;
}

// Main menu loop
void runMainMenu(const MovieGraph& graph)
{
    NameIndex index = buildNameIndex(graph);

    while (true)
    {
        cout << endl;
        cout << "===== The Dobbinators: Movie Path Finder =====" << endl;
        cout << "  1. Find shortest movie path" << endl;
        cout << "  2. Trivia: connect two random actors" << endl;
        cout << "  3. Longest chain challenge" << endl;
        cout << "  4. Exit" << endl;
        cout << "Choice: ";

        string choiceText;
        getline(cin, choiceText);
        choiceText = trimSpaces(choiceText);

        if (choiceText == "1")
        {
            runShortestPathMode(graph, index);
        }
        else if (choiceText == "2")
        {
            runTriviaMode(graph, index);
        }
        else if (choiceText == "3")
        {
            runLongestChainMode(graph, index);
        }
        else if (choiceText == "4")
        {
            cout << "Goodbye!" << endl;
            return;
        }
        else
        {
            cout << "Please enter 1, 2, 3, or 4." << endl;
        }
    }
}