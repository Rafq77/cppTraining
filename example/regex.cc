#include <iostream>
#include <string>
#include <regex>

int main()
{
    std::regex tagPattern{"<[a-zA-Z0-9]*>"};

    std::string lines[] = {"Roses are #ff0000",
                           "violets are #0000ff",
                           "all of my base are belong to you"};

    std::regex color_regex("#([a-f0-9]{2})"
                            "([a-f0-9]{2})"
                            "([a-f0-9]{2})");

    // does the pattern occur in the lines?
    for (const auto &line : lines) {
        std::cout << line << ": " 
                  << std::regex_search(line, color_regex) << '\n';
    }

    // print the matches
    std::smatch color_match;
    for (const auto &line : lines) {
        std::regex_search(line, color_match, color_regex);
        std::cout << "matches for '" << line << "'\n";
        for (size_t i = 0; i < color_match.size(); ++i) {
            std::ssub_match sub_match = color_match[i];
            std::string sub_match_str = sub_match.str();
            std::cout << i << ": " << sub_match_str << '\n';
        }
    }

    // search all occurences in a text
    std::string text;
    for (const auto &line : lines) {
        text += line;
    }

    auto pos = text.cbegin();
    while (std::regex_search(pos, text.cend(), color_match, color_regex))
    {
        std::cout << "found at position "
                  << color_match[0].first - text.begin() << ": "
                  << color_match[0].str() << '\n';
        pos = color_match[0].second;
    }

    return 0;
}

