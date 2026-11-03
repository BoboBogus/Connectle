#include "MovieGraph.h"

std::vector<std::string> MovieGraph::allActors() const
{
	return std::vector<std::string>(actors_.begin(), actors_.end());
}
