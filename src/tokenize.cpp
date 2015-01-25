#include "tokenize.h"
#include <string>

unique_ptr<vector<string_view>>
tokenize_string_views(const string_view &str, const string& delimiters) {

    unique_ptr<vector<string_view>> strs(new vector<string_view>);

    size_t start = 0;
    for (size_t i = 0; i < str.length(); i++) {
        if (delimiters.find(str[i]) != string::npos) {
            if (i != start) {
                strs->push_back(str.substr(start, i - start));
            }
            start = i + 1;
        }
    }
    if (str.length() != start) {
        strs->push_back(str.substr(start, str.length() - start));
    }

    return strs;
}
