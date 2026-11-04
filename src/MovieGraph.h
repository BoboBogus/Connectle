////Collaborated by Angel Reyes and Jayden Valera Alexander Schunko
#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class MovieGraph {
private:
    std::unordered_map<std::string, std::vector<std::string>> actorToMovies_;
    std::unordered_map<std::string, std::vector<std::string>> movieToCast_;
    std::unordered_set<std::string> actors_;
    std::unordered_set<std::string> movies_;

public:
    MovieGraph() = default;

    // Add a connection between an actor and a movie
    void addEdge(const std::string& actor, const std::string& movie);

    // Lookup checks
    bool hasActor(const std::string& actor) const;
    bool hasMovie(const std::string& movie) const;

    // Getters
    const std::vector<std::string>& moviesOf(const std::string& actor) const;
    const std::vector<std::string>& castOf(const std::string& movie) const;

    // Sizes
    size_t actorCount() const;
    size_t movieCount() const;
    std::vector<std::string> allActors() const;
    
    static const std::vector<std::string> emptyList;
};