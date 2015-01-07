#include <numeric>
#include <simmetrics/levenshtein.h>
#include <simmetrics/tokenizer.h>
#include <cmath>

#include "fuzzy_similarity.h"

using namespace std;

unique_ptr<FuzzyOverlap> fuzzy_overlap(const string &s1, const string &s2, const double &delta, const string &delims) {
    vector<string> s1_tokens = *tokenize_to_vector(s1.c_str(), delims.c_str());
    vector<string> s2_tokens = *tokenize_to_vector(s2.c_str(), delims.c_str());

    FuzzyOverlapMap similarities;

    // loop over s2 -> s1 to ensure map is keyed with tokens from s1 to keep logical order
    for (size_t i = 0; i < s2_tokens.size(); i++) {
        double max_sim = 0;
        size_t max_j = 0;

        for (size_t j = 0; j < s1_tokens.size(); j++) {
            double lev_sim = levenshtein_similarity(s2_tokens[i].c_str(), s1_tokens[j].c_str());
            if (lev_sim >= delta && lev_sim > max_sim) {
                max_sim = lev_sim;
                max_j = j;
            }
        }

        if (similarities.count(max_j) == 0 || get<0>(similarities[max_j]) < max_sim) {
            similarities[max_j] = make_tuple(i, max_sim);
        }
    }

    unique_ptr<FuzzyOverlap> res(new FuzzyOverlap(similarities, s1_tokens.size(), s2_tokens.size()));

    return res;
}

double fuzzy_overlap_weight(const FuzzyOverlapMap &map) {
    return accumulate(map.begin(), map.end(), 0.0, [](double acc, const pair<size_t, tuple<size_t, double>> &map_item) {
        return acc + get<1>(map_item.second);
    });
}

double fuzzy_jaccard_similarity(const string &s1, const string &s2, const double &delta) {
    return fuzzy_jaccard_similarity(s1, s2, delta, WHITESPACE_DELIMITERS);
}

double fuzzy_jaccard_similarity(const string &s1, const string &s2, const double &delta, const string &delims) {
    FuzzyOverlapMap overlap;
    size_t s1_token_count = 0, s2_token_count = 0;

    tie(overlap, s1_token_count, s2_token_count) = *fuzzy_overlap(s1, s2, delta, delims);
    double weight = fuzzy_overlap_weight(overlap);

    return weight / (s1_token_count + s2_token_count - weight);
}

double fuzzy_cosine_similarity(const string &s1, const string &s2, const double &delta) {
    return fuzzy_cosine_similarity(s1, s2, delta, WHITESPACE_DELIMITERS);
}

double fuzzy_cosine_similarity(const string &s1, const string &s2, const double &delta, const string &delims) {
    FuzzyOverlapMap overlap;
    size_t s1_token_count = 0, s2_token_count = 0;

    tie(overlap, s1_token_count, s2_token_count) = *fuzzy_overlap(s1, s2, delta, delims);
    double weight = fuzzy_overlap_weight(overlap);

    return weight / sqrt(s1_token_count * s2_token_count);
}

double fuzzy_dice_similarity(const string &s1, const string &s2, const double &delta) {
    return fuzzy_dice_similarity(s1, s2, delta, WHITESPACE_DELIMITERS);
}

double fuzzy_dice_similarity(const string &s1, const string &s2, const double &delta, const string &delims) {
    FuzzyOverlapMap overlap;
    size_t s1_token_count = 0, s2_token_count = 0;

    tie(overlap, s1_token_count, s2_token_count) = *fuzzy_overlap(s1, s2, delta, delims);
    double weight = fuzzy_overlap_weight(overlap);

    return 2 * weight / (s1_token_count + s2_token_count);
}
