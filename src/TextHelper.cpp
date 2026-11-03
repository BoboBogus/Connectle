#include "TextHelper.h"

#include <cctype>

namespace text {

namespace {

// ASCII checks
bool isSpace(char c) { return std::isspace(static_cast<unsigned char>(c)) != 0; }
bool isLower(char c) { return c >= 'a' && c <= 'z'; }
bool isUpper(char c) { return c >= 'A' && c <= 'Z'; }
bool isDigit(char c) { return c >= '0' && c <= '9'; }
bool isNonAscii(char c) { return static_cast<unsigned char>(c) >= 0x80; }

}

std::string trim(const std::string& s) {
    std::size_t b = 0, e = s.size();
    while (b < e && isSpace(s[b])) ++b;
    while (e > b && isSpace(s[e - 1])) --e;
    return s.substr(b, e - b);
}

std::string normalize(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    bool pendingSpace = false;
    for (char c : s) {
        if (isSpace(c)) {
            pendingSpace = !out.empty();
        } else {
            if (pendingSpace) out.push_back(' ');
            pendingSpace = false;
            out.push_back(isUpper(c) ? static_cast<char>(c - 'A' + 'a') : c);
        }
    }
    return out;
}

std::string formatForDataset(const std::string& name) {
    std::string out;
    out.reserve(name.size());
    for (char c : name) {
        if (isUpper(c)) {
            out.push_back(static_cast<char>(c - 'A' + 'a'));
        } else if (isLower(c) || isDigit(c) || isNonAscii(c)) {
            out.push_back(c);
        }

    }
    return out;
}

std::string formatForDisplay(const std::string& name) {
    std::string out;
    out.reserve(name.size() + 4);
    std::size_t wordStart = 0;

    for (std::size_t i = 0; i < name.size(); ++i) {
        const char c = name[i];

        if (isSpace(c)) {
            out.push_back(c);
            wordStart = out.size();
            continue;
        }

        if (i > 0 && isUpper(c)) {
            const char prev = name[i - 1];
            const bool previousEndsAWord =
                isLower(prev) || isDigit(prev) || isNonAscii(prev) || prev == '.';
            const bool insideMc = out.compare(wordStart, std::string::npos, "Mc") == 0;

            if (previousEndsAWord && !insideMc) {
                out.push_back(' ');
                wordStart = out.size();
            }
        }
        out.push_back(c);
    }
    return out;
}

std::vector<std::string> parseList(const std::string& s) {
    std::vector<std::string> items;
    std::size_t i = 0;

    while (i < s.size()) {
        const char quote = s[i];
        if (quote != '\'' && quote != '"') {
            ++i;
            continue;
        }

        std::string item;
        for (++i; i < s.size() && s[i] != quote; ++i) {
            if (s[i] == '\\' && i + 1 < s.size()) ++i;
            item.push_back(s[i]);
        }
        ++i;

        item = trim(item);
        if (!item.empty()) items.push_back(std::move(item));
    }
    return items;
}

}