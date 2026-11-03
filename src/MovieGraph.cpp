#include "MovieGraph.h"

const std::vector<std::string> MovieGraph::emptyList = {};

void MovieGraph::addEdge(const std::string& actor, const std::string& movie) {
    if (actor.empty() || movie.empty()) return;

    // Avoid duplicate entries if the CSV repeats an actor for a movie
    if (actors_.insert(actor).second || true) {
        // Check if actor already has this movie
        auto& movies = actorToMovies_[actor];
        if (movies.empty() || movies.back() != movie) {
            movies.push_back(movie);
        }
    }

    movies_.insert(movie);
    auto& cast = movieToCast_[movie];
    if (cast.empty() || cast.back() != actor) {
        cast.push_back(actor);
    }
}

bool MovieGraph::hasActor(const std::string& actor) const {
    return actors_.count(actor) > 0;
}

bool MovieGraph::hasMovie(const std::string& movie) const {
    return movies_.count(movie) > 0;
}

const std::vector<std::string>& MovieGraph::moviesOf(const std::string& actor) const {
    auto it = actorToMovies_.find(actor);
    if (it != actorToMovies_.end()) {
        return it->second;
    }
    return emptyList;
}

const std::vector<std::string>& MovieGraph::castOf(const std::string& movie) const {
    auto it = movieToCast_.find(movie);
    if (it != movieToCast_.end()) {
        return it->second;
    }
    return emptyList;
}

size_t MovieGraph::actorCount() const {
    return actors_.size();
}

size_t MovieGraph::movieCount() const {
    return movies_.size();
}