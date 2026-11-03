 #ifndef GRAPH_H
 #define GRAPH_H

 #include <string>
 #include <unordered_map>
 #include <unordered_set>
 #include <vector>

 struct MovieGraph
 {
	 std::unordered_map<std::string, std::vector<std::string>> actorToMovies;
	 std::unordered_map<std::string, std::vector<std::string>> movieToActors;

	 void addCredit(const std::string& actor, const std::string& movie);
	 bool hasActor(const std::string& actor) const;
	 const std::vector<std::string>& moviesOf(const std::string& actor) const;
	 const std::vector<std::string>& castOf(const std::string& movie) const;
	 std::vector<std::string> allActors() const;
	 std::vector<std::string> allMovies() const;

 private:
	 std::unordered_map<std::string, std::unordered_set<std::string>> actorMovieSet;
	 std::unordered_map<std::string, std::unordered_set<std::string>> movieActorSet;
 };

 #endif
