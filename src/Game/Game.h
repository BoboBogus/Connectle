#ifndef GAME_H
#define GAME_H

#include <string>
#include "MovieGraph.h"

void runShortestPathMode(MovieGraph& graph);
void runTriviaMode(MovieGraph& graph);
void runLongestChainMode(MovieGraph& graph);

void runMainMenu(MovieGraph& graph);

#endif