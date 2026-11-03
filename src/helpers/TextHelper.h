#pragma once
// Small string helpers shared by the parser and the graph.
// Nothing here knows about movies, actors, or CSV.

#include <string>
#include <vector>

namespace text {

// Trim whitespace from both ends.
std::string trim(const std::string& s);

// Lowercase (ASCII), trim, and collapse runs of whitespace into one space.
// Used to match CSV header names: "  Released_Year " -> "released_year".
std::string normalize(const std::string& s);

// FORMAT FOR THE DATASET: the form used to store and compare actor names.
// Keeps only letters and digits, lowercased, and drops spaces and punctuation.
// "Piper Perabo""piper  perabo""PiperPerabo" -> "piperperabo"
std::string formatForDataset(const std::string& name);

// FORMAT FOR DISPLAY: turns a squashed name from the file into something
// readable. A space goes before a capital letter that follows a lowercase
// letter, a digit, an accented character, or a '.', except inside "Mc":
//   "PiperPerabo" -> "Piper Perabo"      "SamuelL.Jackson" -> "Samuel L. Jackson"
//   "EwanMcGregor" -> "Ewan McGregor"    "LeonardoDiCaprio" -> "Leonardo Di Caprio"
std::string formatForDisplay(const std::string& name);

//   ['TomHanks', "VincentD'Onofrio"]  ->  {TomHanks, VincentD'Onofrio}
// Items may use ' or " quotes. Empty items are dropped.
std::vector<std::string> parseList(const std::string& s);

}