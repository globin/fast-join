#pragma once

#include <string>
#include <vector>
#include <experimental/string_view>

using namespace std;
using namespace std::experimental;

tuple<vector<string_view>, vector<string_view>>
        partition_ned(const string_view &t, const string_view &t_prime, double delta);
