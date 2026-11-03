#include "Game.h"
#include "TextUtils.h"

#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace
{
	std::vector<std::string> parseCsvRow(const std::string& line)
	{
		std::vector<std::string> fields;
		std::string field;
		bool quoted = false;

		for (std::string::size_type index = 0; index < line.size(); ++index)
		{
			const char character = line[index];
			if (character == '"')
			{
				if (quoted && index + 1 < line.size() && line[index + 1] == '"')
				{
					field.push_back('"');
					++index;
				}
				else
				{
					quoted = !quoted;
				}
			}
			else if (character == ',' && !quoted)
			{
				fields.push_back(field);
				field.clear();
			}
			else
			{
				field.push_back(character);
			}
		}
		fields.push_back(field);
		return fields;
	}

	std::string normalizedHeader(const std::string& header)
	{
		std::string normalized;
		for (unsigned char character : header)
		{
			if (std::isalnum(character))
			{
				normalized.push_back(static_cast<char>(std::tolower(character)));
			}
		}
		return normalized;
	}

	std::string cleanActorName(std::string name)
	{
		name = trimSpaces(name);
		while (!name.empty() && (name.front() == '[' || name.front() == ']' ||
								 name.front() == '{' || name.front() == '}'))
		{
			name.erase(name.begin());
			name = trimSpaces(name);
		}
		while (!name.empty() && (name.back() == '[' || name.back() == ']' ||
								 name.back() == '{' || name.back() == '}'))
		{
			name.pop_back();
			name = trimSpaces(name);
		}
		if (name.size() >= 2 &&
			((name.front() == '\'' && name.back() == '\'') ||
			 (name.front() == '"' && name.back() == '"')))
		{
			name = name.substr(1, name.size() - 2);
		}
		return trimSpaces(name);
	}

	std::vector<std::string> splitCast(const std::string& castField)
	{
		std::vector<std::string> actors;
		std::string actor;
		char quote = '\0';

		for (char character : castField)
		{
			if ((character == '\'' || character == '"') && (quote == '\0' || quote == character))
			{
				quote = quote == '\0' ? character : '\0';
				actor.push_back(character);
			}
			else if ((character == ',' || character == ';' || character == '|') && quote == '\0')
			{
				std::string cleaned = cleanActorName(actor);
				if (!cleaned.empty())
				{
					actors.push_back(cleaned);
				}
				actor.clear();
			}
			else
			{
				actor.push_back(character);
			}
		}

		std::string cleaned = cleanActorName(actor);
		if (!cleaned.empty())
		{
			actors.push_back(cleaned);
		}
		return actors;
	}

	bool loadMovieCsv(const std::string& filename, MovieGraph& graph, std::size_t& creditCount)
	{
		std::ifstream input(filename.c_str());
		if (!input)
		{
			std::cerr << "Could not open dataset: " << filename << std::endl;
			return false;
		}

		std::string line;
		if (!std::getline(input, line))
		{
			std::cerr << "The dataset is empty." << std::endl;
			return false;
		}

		const std::vector<std::string> headers = parseCsvRow(line);
		std::size_t titleColumn = headers.size();
		std::size_t castColumn = headers.size();
		bool castIsSingleActor = false;

		for (std::size_t column = 0; column < headers.size(); ++column)
		{
			const std::string header = normalizedHeader(headers[column]);
			if (header == "title" || header == "movie" || header == "movietitle" || header == "originaltitle")
			{
				titleColumn = column;
			}
			if (header == "cast" || header == "actors" || header == "actor")
			{
				castColumn = column;
				castIsSingleActor = header == "actor";
			}
		}

		if (titleColumn == headers.size() || castColumn == headers.size())
		{
			std::cerr << "CSV headers must include a title/movie column and a cast/actors/actor column."
					  << std::endl;
			return false;
		}

		creditCount = 0;
		while (std::getline(input, line))
		{
			const std::vector<std::string> fields = parseCsvRow(line);
			if (titleColumn >= fields.size() || castColumn >= fields.size())
			{
				continue;
			}

			const std::string movie = trimSpaces(fields[titleColumn]);
			if (movie.empty())
			{
				continue;
			}

			const std::vector<std::string> actors = castIsSingleActor
				? std::vector<std::string>(1, cleanActorName(fields[castColumn]))
				: splitCast(fields[castColumn]);
			for (const std::string& actor : actors)
			{
				if (!actor.empty())
				{
					graph.addCredit(actor, movie);
					++creditCount;
				}
			}
		}
		return true;
	}
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: connectle <dataset.csv>\n"
				  << "CSV needs a title/movie column and cast/actors/actor column. "
				  << "Separate cast names with commas, semicolons, or |." << std::endl;
		return 1;
	}

	MovieGraph graph;
	std::size_t creditCount = 0;
	if (!loadMovieCsv(argv[1], graph, creditCount) || creditCount == 0)
	{
		if (creditCount == 0)
		{
			std::cerr << "No actor/movie credits were loaded." << std::endl;
		}
		return 1;
	}

	std::cout << "Loaded " << graph.allActors().size() << " actors, "
			  << graph.allMovies().size() << " movies, and " << creditCount << " credits." << std::endl;
	runMainMenu(graph);
	return 0;
}
