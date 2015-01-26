#pragma once

#include <string>
#include <vector>
#include <experimental/string_view>

using namespace std;
using namespace std::experimental;

auto partition_ned(const string_view &t, const string_view &t_prime, double delta) ->
    unique_ptr<tuple<vector<string_view>, vector<string_view>>>;
