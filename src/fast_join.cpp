#include "fast_join.h"
#include "fuzzy_signature.h"
#include "fuzzy_similarity.h"


unique_ptr<vector<tuple<size_t, size_t, double>>> fast_join(const vector<string> &data1, const vector<string> &data2) {
    unique_ptr<vector<tuple<size_t, size_t, double>>> matches(new vector<tuple<size_t, size_t, double>>());
    double val;

    for (size_t i = 0; i < data1.size(); i++) {
        for (size_t j = 0; j < data2.size(); j++) {
            if ((val = fuzzy_jaccard_similarity(data1[i], data2[j], 0.6)) > 0.4 && val < 1) {
                matches->push_back(make_tuple(i, j, val));
            }
        }
    }

    return matches;
}
