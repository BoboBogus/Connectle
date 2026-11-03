#include "DatasetLoader.h"
#include "CsvParser.h"
#include "TextHelper.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool DatasetLoader::loadDataset(const std::string& filename, MovieGraph& graph) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }

    std::string headerLine;
    if (!std::getline(file, headerLine)) {
        std::cerr << "Error: File is empty or missing header." << std::endl;
        return false;
    }

    auto headerMap = CsvParser::parseHeader(headerLine);

    // Verify required columns exist ("title" and "cast")
    if (headerMap.find("title") == headerMap.end() || headerMap.find("cast") == headerMap.end()) {
        std::cerr << "Error: CSV must contain 'title' and 'cast' columns." << std::endl;
        return false;
    }

    int titleIdx = headerMap["title"];
    int castIdx = headerMap["cast"];

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::vector<std::string> cols = CsvParser::parseLine(line);
        if (cols.size() <= static_cast<size_t>(std::max(titleIdx, castIdx))) {
            continue; // Malformed row or missing columns
        }

        std::string movieTitle = text::trim(cols[titleIdx]);
        std::string rawCast = cols[castIdx];

        // Parse cast list using the text helper
        std::vector<std::string> actors = text::parseList(rawCast);

        for (const std::string& actor : actors) {
            // Optional: format or store actor as needed
            // e.g., actor name formatting if required by your search queries
            graph.addEdge(actor, movieTitle);
        }
    }

    file.close();
    return true;
}