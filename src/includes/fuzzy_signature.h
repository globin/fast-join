#pragma once

#include <string>
#include <vector>

using namespace std;

tuple<vector<string>, vector<string>> partition_ned(const string &t, const string &t_prime, double delta);
