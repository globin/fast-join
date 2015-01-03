#pragma once

#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

typedef std::map<size_t, std::tuple<size_t, double>> FuzzyOverlapMap;

std::unique_ptr<std::tuple<FuzzyOverlapMap, size_t, size_t>> fuzzy_overlap(std::string s1, std::string s2, double delta, const char* delims);
double fuzzy_overlap_weight(const FuzzyOverlapMap &map);

double fuzzy_jaccard_similarity(std::string s1, std::string s2, double delta);
double fuzzy_jaccard_similarity(std::string s1, std::string s2, double delta, const char* delims);
