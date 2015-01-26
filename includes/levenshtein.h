#pragma once

#include <string>
#include <experimental/string_view>
#include <vector>

using namespace std;
using namespace std::experimental;

unsigned int levenshtein_distance(const string_view &s1, const string_view & s2);
double levenshtein_similarity(const string_view &str1, const string_view &str2);
