// header fiile for searches - Alexander Schunko
// contains 3 search algorithms
//bfs() - finds shortest path but takes some time
//dfs() - finds path fast but not always shortest
//ddfs() - dfs but starts from both ends so runs faster

#pragma once
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

// what every search algorithm will return after being called
struct searchResult{
    bool found = false; // if a path was found - default to no
    vector<std::string> actors; // the actors in the path
    vector<std::string> movies; // the movies in the path
    int actorsVisited = 0; // how many actors were visited
    int moviesVisited = 0; // how many movies were visited
    double time = 0; // how long the search took

    int moviesAway(); // movies in chain
    int nodes(); // nodes visited
};


searchResult bfs(MovieGraph& graph, std::string start, std::string end); // breadth first search
searchResult dfs(MovieGraph& graph, std::string start, std::string end); // depth first search
searchResult ddfs(MovieGraph& graph, std::string start, std::string end); // double depth first