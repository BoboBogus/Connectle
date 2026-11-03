 #include "Graph.h"

 namespace
 {
	 const std::vector<std::string> emptyList;
 }

 void MovieGraph::addCredit(const std::string& actor, const std::string& movie)
 {
	 if (actor.empty() || movie.empty() || !actorMovieSet[actor].insert(movie).second)
	 {
		 return;
	 }

	 movieActorSet[movie].insert(actor);
	 actorToMovies[actor].push_back(movie);
	 movieToActors[movie].push_back(actor);
 }

 bool MovieGraph::hasActor(const std::string& actor) const
 {
	 return actorToMovies.find(actor) != actorToMovies.end();
 }

 const std::vector<std::string>& MovieGraph::moviesOf(const std::string& actor) const
 {
	 const auto found = actorToMovies.find(actor);
	 return found == actorToMovies.end() ? emptyList : found->second;
 }

 const std::vector<std::string>& MovieGraph::castOf(const std::string& movie) const
 {
	 const auto found = movieToActors.find(movie);
	 return found == movieToActors.end() ? emptyList : found->second;
 }

 std::vector<std::string> MovieGraph::allActors() const
 {
	 std::vector<std::string> actors;
	 actors.reserve(actorToMovies.size());
	 for (const auto& entry : actorToMovies)
	 {
		 actors.push_back(entry.first);
	 }
	 return actors;
 }

 std::vector<std::string> MovieGraph::allMovies() const
 {
	 std::vector<std::string> movies;
	 movies.reserve(movieToActors.size());
	 for (const auto& entry : movieToActors)
	 {
		 movies.push_back(entry.first);
	 }
	 return movies;
 }
