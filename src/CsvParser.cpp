#include "CsvParser.h"
#include "TextHelper.h"

std::vector<std::string> CsvParser::parseLine(const std::string& line) {
    std::vector<std::string> result;
    std::string current;
    bool inQuotes = false;

    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];

        if (c == '"') {
            // Check for escaped quote ("")
            if (inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                current.push_back('"');
                i++;
            } else {
                inQuotes = !inQuotes;
            }
        } else if (c == ',' && !inQuotes) {
            result.push_back(current);
            current.clear();
        } else {
            current.push_back(c);
        }
    }
    result.push_back(current);
    return result;
}

std::unordered_map<std::string, int> CsvParser::parseHeader(const std::string& headerLine) {
    std::unordered_map<std::string, int> headerMap;
    std::vector<std::string> rawCols = parseLine(headerLine);
    
    for (size_t i = 0; i < rawCols.size(); ++i) {
        std::string norm = text::normalize(rawCols[i]);
        headerMap[norm] = static_cast<int>(i);
    }
    
    return headerMap;
}