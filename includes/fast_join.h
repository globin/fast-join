#pragma once

#include <memory>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

unique_ptr<vector<tuple<size_t, size_t, double>>> fast_join(const vector<string> &data1, const vector<string> &data2);
