#include "Search.h"

#include <chrono>
#include <queue>
#include <unordered_map>

namespace
{
	struct ParentLink
	{
		std::string actor;
		std::string movie;
		bool hasParent;

		ParentLink() : hasParent(false) {}
		ParentLink(const std::string& parentActor, const std::string& connectingMovie)
			: actor(parentActor), movie(connectingMovie), hasParent(true) {}
	};

	bool buildPath(const std::string& startActor,
				   const std::string& endActor,
				   const std::string& meetingActor,
				   const std::unordered_map<std::string, ParentLink>& fromStart,
				   const std::unordered_map<std::string, ParentLink>& fromEnd,
				   std::vector<std::string>& path)
	{
		std::vector<std::string> leftHalf;
		std::string actor = meetingActor;
		leftHalf.push_back(actor);

		while (actor != startActor)
		{
			const auto link = fromStart.find(actor);
			if (link == fromStart.end() || !link->second.hasParent)
			{
				return false;
			}
			leftHalf.push_back(link->second.movie);
			actor = link->second.actor;
			leftHalf.push_back(actor);
		}

		path.assign(leftHalf.rbegin(), leftHalf.rend());
		actor = meetingActor;
		while (actor != endActor)
		{
			const auto link = fromEnd.find(actor);
			if (link == fromEnd.end() || !link->second.hasParent)
			{
				return false;
			}
			path.push_back(link->second.movie);
			actor = link->second.actor;
			path.push_back(actor);
		}
		return true;
	}

	bool expandLayer(const MovieGraph& graph,
					 std::queue<std::string>& frontier,
					 std::unordered_map<std::string, ParentLink>& ownParents,
					 const std::unordered_map<std::string, ParentLink>& otherParents,
					 std::size_t& nodesVisited,
					 std::string& meetingActor)
	{
		const std::size_t layerSize = frontier.size();
		for (std::size_t index = 0; index < layerSize; ++index)
		{
			const std::string currentActor = frontier.front();
			frontier.pop();
			++nodesVisited;

			for (const std::string& movie : graph.moviesOf(currentActor))
			{
				for (const std::string& nextActor : graph.castOf(movie))
				{
					if (ownParents.find(nextActor) != ownParents.end())
					{
						continue;
					}

					ownParents.emplace(nextActor, ParentLink(currentActor, movie));
					if (otherParents.find(nextActor) != otherParents.end())
					{
						meetingActor = nextActor;
						return true;
					}
					frontier.push(nextActor);
				}
			}
		}
		return false;
	}

	double elapsedMilliseconds(const std::chrono::steady_clock::time_point& started)
	{
		return std::chrono::duration<double, std::milli>(
			std::chrono::steady_clock::now() - started).count();
	}
}

SearchResult runBFS(const MovieGraph& graph, const std::string& startActor, const std::string& endActor)
{
	const auto started = std::chrono::steady_clock::now();
	SearchResult result;

	if (!graph.hasActor(startActor) || !graph.hasActor(endActor))
	{
		result.milliseconds = elapsedMilliseconds(started);
		return result;
	}

	std::queue<std::string> frontier;
	std::unordered_map<std::string, ParentLink> parents;
	parents.emplace(startActor, ParentLink());
	frontier.push(startActor);

	while (!frontier.empty())
	{
		const std::string currentActor = frontier.front();
		frontier.pop();
		++result.nodesVisited;

		if (currentActor == endActor)
		{
			result.found = buildPath(startActor, endActor, endActor, parents,
									 std::unordered_map<std::string, ParentLink>(), result.path);
			break;
		}

		for (const std::string& movie : graph.moviesOf(currentActor))
		{
			for (const std::string& nextActor : graph.castOf(movie))
			{
				if (parents.find(nextActor) != parents.end())
				{
					continue;
				}
				parents.emplace(nextActor, ParentLink(currentActor, movie));
				if (nextActor == endActor)
				{
					result.found = buildPath(startActor, endActor, endActor, parents,
											 std::unordered_map<std::string, ParentLink>(), result.path);
					break;
				}
				frontier.push(nextActor);
			}
			if (result.found)
			{
				break;
			}
		}
		if (result.found)
		{
			break;
		}
	}

	result.milliseconds = elapsedMilliseconds(started);
	return result;
}

SearchResult runBidirectionalBFS(const MovieGraph& graph,
								 const std::string& startActor,
								 const std::string& endActor)
{
	const auto started = std::chrono::steady_clock::now();
	SearchResult result;

	if (!graph.hasActor(startActor) || !graph.hasActor(endActor))
	{
		result.milliseconds = elapsedMilliseconds(started);
		return result;
	}

	if (startActor == endActor)
	{
		result.found = true;
		result.path.push_back(startActor);
		result.nodesVisited = 1;
		result.milliseconds = elapsedMilliseconds(started);
		return result;
	}

	std::queue<std::string> startFrontier;
	std::queue<std::string> endFrontier;
	std::unordered_map<std::string, ParentLink> fromStart;
	std::unordered_map<std::string, ParentLink> fromEnd;
	fromStart.emplace(startActor, ParentLink());
	fromEnd.emplace(endActor, ParentLink());
	startFrontier.push(startActor);
	endFrontier.push(endActor);

	std::string meetingActor;
	while (!startFrontier.empty() && !endFrontier.empty())
	{
		bool met;
		if (startFrontier.size() <= endFrontier.size())
		{
			met = expandLayer(graph, startFrontier, fromStart, fromEnd,
							  result.nodesVisited, meetingActor);
		}
		else
		{
			met = expandLayer(graph, endFrontier, fromEnd, fromStart,
							  result.nodesVisited, meetingActor);
		}
		if (met)
		{
			result.found = buildPath(startActor, endActor, meetingActor,
									 fromStart, fromEnd, result.path);
			break;
		}
	}

	result.milliseconds = elapsedMilliseconds(started);
	return result;
}
