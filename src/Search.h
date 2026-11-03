#ifndef SEARCH_H
#define SEARCH_H

#include "Graph.h"

#include <cstddef>
#include <string>
#include <vector>

struct SearchResult
{
	bool found;
	std::vector<std::string> path;
	std::size_t nodesVisited;
	double milliseconds;

	SearchResult() : found(false), nodesVisited(0), milliseconds(0.0) {}
};

SearchResult runBFS(const MovieGraph& graph, const std::string& startActor, const std::string& endActor);
SearchResult runBidirectionalBFS(const MovieGraph& graph, const std::string& startActor, const std::string& endActor);

#endif
