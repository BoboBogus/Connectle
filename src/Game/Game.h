#ifndef GAME_H
#define GAME_H

#include <string>
#include "MovieGraph.h"

void runShortestPathMode(const MovieGraph& graph);
void runTriviaMode(const MovieGraph& graph);
void runLongestChainMode(const MovieGraph& graph);

void runMainMenu(const MovieGraph& graph);

#endif