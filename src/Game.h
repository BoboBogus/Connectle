#ifndef GAME_H
#define GAME_H

#include <string>
#include <unordered_map>
#include "Graph.h"

struct NameIndex{
    std::unordered_map<std::string, std::string> actorLowerToName;
    std::unordered_map<std::string, std::string> movieLowerToTitle;
};

NameIndex buildNameIndex(const MovieGraph& graph);

void runShortestPathMode(const MovieGraph& graph, const NameIndex& index);
void runTriviaMode(const MovieGraph& graph, const NameIndex& index);
void runLongestChainMode(const MovieGraph& graph, const NameIndex& index);

void runMainMenu(const MovieGraph& graph);

#endif