#pragma once

#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#define DEFAULT_DELIMITERS " _"

using namespace std;

typedef map<size_t, tuple<size_t, double>> FuzzyOverlapMap;
typedef tuple<FuzzyOverlapMap, size_t, size_t> FuzzyOverlap;

unique_ptr<FuzzyOverlap> fuzzy_overlap(const string &s1, const string &s2, const double &delta, const string &delims);
double fuzzy_overlap_weight(const FuzzyOverlapMap &map);

double fuzzy_jaccard_similarity(const string &s1, const string &s2, const double &delta);
double fuzzy_jaccard_similarity(const string &s1, const string &s2, const double &delta, const string &delims);
double fuzzy_cosine_similarity(const string &s1, const string &s2, const double &delta);
double fuzzy_cosine_similarity(const string &s1, const string &s2, const double &delta, const string &delims);
double fuzzy_dice_similarity(const string &s1, const string &s2, const double &delta);
double fuzzy_dice_similarity(const string &s1, const string &s2, const double &delta, const string &delims);
