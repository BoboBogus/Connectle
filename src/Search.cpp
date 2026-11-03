// code file for searches - Alexander Schunko

#include "Search.h"
#include <algorithm>
#include <chrono>
#include <queue>
#include <unordered_set>

using namespace std;

// visit next actor
struct step {
    string actor; //who to visit
    string fromActor; //where to visit from
    string fromMovie;
};

int searchResult::moviesAway() {
    if(found == true){
        return movies.size();
    }
    else{
        return -1;
    }
}

int searchResult::nodes() {
    return actorsVisited + moviesVisited;
}

//returns how long it takes the search to run - I utilized AI to learn and help me
//implement this portion of the code
double time(chrono::steady_clock::time_point startTime){
    chrono::duration<double, milli> elapsed = chrono::steady_clock::now() - startTime;
    return elapsed.count();
}

//constructs the path properly
void buildPath(string first, string last, unordered_map<string, string>& cameFromActor, unordered_map<string, string>& cameFromMovie, vector<string>& actors, vector<string>& movies) {
    string current = last;
    while (current != first) {
        actors.push_back(current);
        movies.push_back(cameFromMovie[current]);
        current = cameFromActor[current];
    }
    actors.push_back(first);
    //fills in the path from last to first then we flip it to be in the correct order

    reverse(actors.begin(), actors.end());
    reverse(movies.begin(), movies.end());
}

searchResult bfs(MovieGraph& graph, string start, string end) { //bfs implementation
    auto startTime = chrono::steady_clock::now();
    searchResult result;

    if(graph.hasActor(start) && graph.hasActor(end)) {
        queue<string> waiting; //actors to visit
        unordered_set<string> visitedA;
        unordered_set<string> visitedM;

        unordered_map<string, string> cameFromActor; //where we came from
        unordered_map<string, string> cameFromMovie;

        waiting.push(start);
        visitedActors.insert(start);
        result.actorsVisited = 1;
        if(start == end) {
            bool found = true;
        }
        else{
            bool found == false;
        }

        while(waiting.empty() == false && found == false) {
            string actor = waiting.front();
            waiting.pop();

            for(auto x: graph.moviesOf(actor)){ //visits every movie of the actor
                if (visitedMovies.count(x) != 0){
                    continue; //skip if already visited
                }
                visitedMovies.insert(x);
                result.moviesVisited++;

                for (auto y: graph.castOf(x)) { //visits every actor in the movie
                    if (visitedActors.count(y) != 0){
                        continue;
                    }
                    visitedActors.insert(y);
                    result.actorsVisited++;
                    cameFromActor[y] = actor;
                    cameFromMovie[y] = x;
                    waiting.push(y);

                    if (y == end) {
                        found = true;
                        break;
                    }
                }
                if (found == true) {
                    break;
                }
            }
        }
        if(found == true) { // builds path then returns it
            result.found = true;
            buildPath(start, end, cameFromActor, cameFromMovie, result.actors, result.movies);
        }
    }
    result.time = time(startTime);
    return result;
}


searchResult dfs(MovieGraph& graph, string start, string end) { //dfs implementation
    auto startTime = chrono::steady_clock::now();
    searchResult result;

    if(graph.hasActor(start) && graph.hasActor(end)) {
        vector<step> stack; // next step is always on top
        unordered_set<string> visitedA;
        unordered_set<string> visitedM;
        unordered_map<string, string> cameFromActor;
        unordered_map<string, string> cameFromMovie;

        bool found = false;
        stack.push_back(Step{start, "", ""}); //push starting ndoe to begin search

        while (stack.empty() == false && found == false) {
            step current = stack.back();
            stack.pop_back();

            if (visitedA.count(step.actor) != 0) {
                continue; //skip if already visited
            }
            visitedA.insert(step.actor);
            result.actorsVisited++;
            cameFromActor[step.actor] = step.fromActor;
            cameFromMovie[step.actor] = step.fromMovie;

            if (step.actor == end) {
                found = true;
                break;
            }
            
            for(auto x: graph.moviesOf(step.actor)) { //visits every movie of the actor
                if (visitedM.count(x) != 0) {
                    continue; //skip if already visited
                }
                visitedM.insert(x);
                result.moviesVisited++;

                for (auto y: graph.castOf(x)) { //visits every actor in the movie
                    if (visitedA.count(y) != 0) {
                        continue;
                    }
                    stack.push_back(Step{y, step.actor, x});
                }
            }
        }
        if(found == true) { // builds path then returns it
            result.found = true;
            buildPath(start, end, cameFromActor, cameFromMovie, result.actors, result.movies);
        }
    }
    result.time = time(startTime);
    return result;
}

searchResult ddfs(MovieGraph& graph, string start, string end) { //dfs implementation
}
