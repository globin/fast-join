#include <algorithm>
#include <iostream>
#include <chrono>

#include "fast_join.h"
#include "fuzzy_signature.h"
#include "fuzzy_similarity.h"

using namespace chrono;

unique_ptr<vector<tuple<size_t, size_t, double>>> fast_join(const vector<string> &data1, const vector<string> &data2) {
    vector<tuple<size_t, size_t>> possible_matches;
    double delta = 0.8;

    auto start = high_resolution_clock::now();

    for (size_t i = 0; i < data1.size(); i++) {
        for (size_t j = 0; j < data2.size(); j++) {
            vector<string> sig1, sig2;
            tie(sig1, sig2) = partition_ned(data1[i], data2[i], delta);
            sort(sig1.begin(), sig1.end());
            sort(sig2.begin(), sig2.end());

            vector<string> tmp;
            set_intersection(sig1.begin(), sig1.end(), sig2.begin(), sig2.end(), back_inserter(tmp));

//            cout << "empty: " << tmp.empty() << endl;
            if (!tmp.empty()) {
                possible_matches.push_back(make_tuple(i, j));
            }
        }
    }

    cout << "signatures: " <<
            duration_cast<duration<double>>(high_resolution_clock::now() - start).count() * 1000 << "ms" << endl;

    unique_ptr<vector<tuple<size_t, size_t, double>>> matches(new vector<tuple<size_t, size_t, double>>());
    double val;

    start = high_resolution_clock::now();

    size_t i, j;
    for (auto possible_match : possible_matches) {
        tie(i, j) = possible_match;
        if ((val = fuzzy_jaccard_similarity(data1[i], data2[j], delta)) > 0.6 && val < 1) {
            matches->push_back(make_tuple(i, j, val));
        }
    }

    cout << "fuzzy_jaccard: " <<
            duration_cast<duration<double>>(high_resolution_clock::now() - start).count() * 1000 << "ms" << endl;

    return matches;
}
