// code file for searches - Alexander Schunko

#include "Search.h"
#include "MovieGraph.h"
#include <algorithm>
#include <chrono>
#include <queue>
#include <unordered_set>

using namespace std;

namespace search {
// visit next actor
struct Step {
    string actor;       // who to visit
    string fromActor;   // where to visit from
    string fromMovie;
};

int searchResult::moviesAway() const {
    if (found == true) {
        return (int)movies.size();
    } else {
        return -1;
    }
}

int searchResult::nodes() const {
    return actorsVisited + moviesVisited;
}

// returns how long it takes the search to run
static double runTime(chrono::steady_clock::time_point startTime) {
    chrono::duration<double, milli> elapsed = chrono::steady_clock::now() - startTime;
    return elapsed.count();
}

// constructs the path properly
void buildPath(string first, string last, unordered_map<string, string>& cameFromActor, unordered_map<string, string>& cameFromMovie, vector<string>& actors, vector<string>& movies) {
    string current = last;
    while (current != first) {
        actors.push_back(current);
        movies.push_back(cameFromMovie[current]);
        current = cameFromActor[current];
    }
    actors.push_back(first);

    reverse(actors.begin(), actors.end());
    reverse(movies.begin(), movies.end());
}

searchResult bfs(MovieGraph& graph, string start, string end) {
    auto startTime = chrono::steady_clock::now();
    searchResult result;

    if (graph.hasActor(start) && graph.hasActor(end)) {
        queue<string> waiting;
        unordered_set<string> visitedA;
        unordered_set<string> visitedM;

        unordered_map<string, string> cameFromActor;
        unordered_map<string, string> cameFromMovie;

        waiting.push(start);
        visitedA.insert(start);
        result.actorsVisited = 1;

        bool found = false;
        if (start == end) {
            found = true;
        }

        while (!waiting.empty() && !found) {
            string actor = waiting.front();
            waiting.pop();

            for (auto x : graph.moviesOf(actor)) {
                if (visitedM.count(x) != 0) {
                    continue;
                }
                visitedM.insert(x);
                result.moviesVisited++;

                for (auto y : graph.castOf(x)) {
                    if (visitedA.count(y) != 0) {
                        continue;
                    }
                    visitedA.insert(y);
                    result.actorsVisited++;
                    cameFromActor[y] = actor;
                    cameFromMovie[y] = x;
                    waiting.push(y);

                    if (y == end) {
                        found = true;
                        break;
                    }
                }
                if (found) {
                    break;
                }
            }
        }

        if (found) {
            result.found = true;
            buildPath(start, end, cameFromActor, cameFromMovie, result.actors, result.movies);
        }
    }
    result.time = runTime(startTime);
    return result;
} // <--- FIXED: Added missing closing brace for bfs()

searchResult dfs(MovieGraph& graph, string start, string end) {
    auto startTime = chrono::steady_clock::now();
    searchResult result;

    if (graph.hasActor(start) && graph.hasActor(end)) {
        vector<Step> stack;
        unordered_set<string> visitedA;
        unordered_set<string> visitedM;
        unordered_map<string, string> cameFromActor;
        unordered_map<string, string> cameFromMovie;

        bool found = false;
        stack.push_back(Step{start, "", ""});

        while (!stack.empty() && !found) {
            Step currentStep = stack.back();
            stack.pop_back();

            if (visitedA.count(currentStep.actor) != 0) {
                continue;
            }
            visitedA.insert(currentStep.actor);
            result.actorsVisited++;
            cameFromActor[currentStep.actor] = currentStep.fromActor;
            cameFromMovie[currentStep.actor] = currentStep.fromMovie;

            if (currentStep.actor == end) {
                found = true;
                break;
            }
            
            for (auto x : graph.moviesOf(currentStep.actor)) {
                if (visitedM.count(x) != 0) {
                    continue;
                }
                visitedM.insert(x);
                result.moviesVisited++;

                for (auto y : graph.castOf(x)) {
                    if (visitedA.count(y) != 0) {
                        continue;
                    }
                    stack.push_back(Step{y, currentStep.actor, x});
                }
            }
        }

        if (found) {
            result.found = true;
            buildPath(start, end, cameFromActor, cameFromMovie, result.actors, result.movies);
        }
    }
    result.time = runTime(startTime);
    return result;
}

// Everything one side of DDFS search needs
struct Side {
    vector<Step> stack;
    unordered_set<string> visitedA;
    unordered_set<string> visitedM;
    unordered_map<string, string> cameFromActor;
    unordered_map<string, string> cameFromMovie;
};

// Takes one step for DDFS
static bool oneStep(MovieGraph& graph, Side& s1, Side& s2, searchResult& result, string& meetingActor) {
    if (!s1.stack.empty()) {
        Step current = s1.stack.back();
        s1.stack.pop_back();

        if (s1.visitedA.count(current.actor) != 0) {
            return false;
        }
        s1.visitedA.insert(current.actor);
        result.actorsVisited++;
        s1.cameFromActor[current.actor] = current.fromActor;
        s1.cameFromMovie[current.actor] = current.fromMovie;

        if (s2.visitedA.count(current.actor) != 0) {
            meetingActor = current.actor;
            return true;
        }

        for (auto x : graph.moviesOf(current.actor)) {
            if (s1.visitedM.count(x) != 0) {
                continue;
            }
            s1.visitedM.insert(x);
            result.moviesVisited++;

            for (auto y : graph.castOf(x)) {
                if (s1.visitedA.count(y) != 0) {
                    continue;
                }
                s1.stack.push_back(Step{y, current.actor, x});
            }
        }
    }
    return false;
}

searchResult ddfs(MovieGraph& graph, string start, string end) {
    auto startTime = chrono::steady_clock::now();
    searchResult result;

    if (graph.hasActor(start) && graph.hasActor(end)) {
        Side s1;
        Side s2;
        s1.stack.push_back(Step{start, "", ""});
        s2.stack.push_back(Step{end, "", ""});

        string meetingActor = "";
        bool met = false;

        if (start == end) {
            meetingActor = start;
            met = true;
            result.actorsVisited = 1;
        }

        while (!met && (!s1.stack.empty() || !s2.stack.empty())) {
            met = oneStep(graph, s1, s2, result, meetingActor);
            if (!met) {
                met = oneStep(graph, s2, s1, result, meetingActor);
            }
        }

        if (met) {
            result.found = true;
            buildPath(start, meetingActor, s1.cameFromActor, s1.cameFromMovie, result.actors, result.movies);
            
            string current = meetingActor;
            while (current != end) {
                result.movies.push_back(s2.cameFromMovie[current]);
                current = s2.cameFromActor[current];
                result.actors.push_back(current);
            }
        }
    }
    result.time = runTime(startTime);
    return result;
}

} // namespace search