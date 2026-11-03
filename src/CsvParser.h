#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

class CsvParser {
public:
    // Parses a single CSV line into individual columns, respecting quotes
    static std::vector<std::string> parseLine(const std::string& line);

    // Reads header row and maps normalized column names to their indices
    static std::unordered_map<std::string, int> parseHeader(const std::string& headerLine);
};