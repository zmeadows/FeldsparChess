export module unstd.string_util;

import <iterator>;
import <sstream>;
import <string>;
import <vector>;

template <typename Out>
void split(const std::string& s, char delim, Out result)
{
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        if (!item.empty()) {
            *result++ = item;
        }
    }
}

// @TODO: Make std::string_view version.
export std::vector<std::string> split(const std::string& s, char delim = ' ')
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

export void split(
    std::vector<std::string_view>& output, const std::string& strv, std::string_view delims = " ")
{
    output.clear();
    size_t first = 0;

    while (first < strv.size()) {
        const auto second = strv.find_first_of(delims, first);

        if (first != second) output.emplace_back(strv.substr(first, second - first));

        if (second == std::string_view::npos) break;

        first = second + 1;
    }
}
