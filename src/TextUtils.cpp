#include "TextUtils.h"

#include <cctype>

std::string toLowerCase(const std::string& text)
{
    std::string result = text;
    for (char& character : result)
    {
        character = static_cast<char>(std::tolower(static_cast<unsigned char>(character)));
    }
    return result;
}

std::string trimSpaces(const std::string& text)
{
    std::string::size_type first = 0;
    while (first < text.size() && std::isspace(static_cast<unsigned char>(text[first])))
    {
        ++first;
    }

    std::string::size_type last = text.size();
    while (last > first && std::isspace(static_cast<unsigned char>(text[last - 1])))
    {
        --last;
    }
    return text.substr(first, last - first);
}
