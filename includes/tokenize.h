#pragma once

#include <memory>
#include <experimental/string_view>
#include <vector>

using namespace std;
using namespace std::experimental;

auto tokenize_string_views(const string_view &str, const string& delimiters) -> unique_ptr<vector<string_view>>;
auto q_gram_partition(const string_view& str, size_t q) -> vector<string_view>;
