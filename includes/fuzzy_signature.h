#pragma once

#include <string>
#include <vector>
#include <experimental/string_view>

using namespace std;
using namespace std::experimental;

auto token_sensitive_sig(const vector<string_view> &token_set, double c) -> multiset<string_view>;
auto partition_ned(const string_view &t, const string_view &t_prime, double delta) ->
    unique_ptr<tuple<vector<string_view>, vector<string_view>>>;
