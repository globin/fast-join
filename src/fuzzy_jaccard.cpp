#include <numeric>
#include <simmetrics/levenshtein.h>
#include <simmetrics/tokenizer.h>

#include "fuzzy_jaccard.h"

using namespace std;

unique_ptr<FuzzyOverlapMap> fuzzy_overlap(string s1, string s2, double delta, const char* delims) {
    vector<string> s1_tokens = *tokenize_to_vector(s1.c_str(), delims);
    vector<string> s2_tokens = *tokenize_to_vector(s2.c_str(), delims);

    unique_ptr<FuzzyOverlapMap> similarities(new FuzzyOverlapMap());

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

        if (similarities->count(max_j) == 0 || get<0>((*similarities)[max_j]) < max_sim) {
            (*similarities)[max_j] = make_tuple(i, max_sim);
        }
    }

    return similarities;
}

double fuzzy_overlap_weight(const FuzzyOverlapMap &map) {
    return accumulate(map.begin(), map.end(), 0.0, [](double acc, const pair<size_t, tuple<size_t, double>> &map_item) {
        return acc + get<1>(map_item.second);
    });
}

double fuzzy_jaccard_similarity(string s1, string s2, double delta) {
    return fuzzy_jaccard_similarity(s1, s2, delta, WHITESPACE_DELIMITERS);
}

double fuzzy_jaccard_similarity(string s1, string s2, double delta, const char* delims) {
    unique_ptr<FuzzyOverlapMap> overlap = fuzzy_overlap(s1, s2, delta, delims);

    return fuzzy_overlap_weight(*overlap);
}
