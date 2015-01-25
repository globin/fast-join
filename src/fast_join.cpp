#include <algorithm>
#include <iostream>
#include <chrono>
#include <set>

#include "fast_join.h"
#include "fuzzy_signature.h"
#include "fuzzy_similarity.h"
#include "tokenize.h"

using namespace chrono;

unique_ptr<vector<tuple<size_t, size_t, double>>> fast_join(const vector<string> &data1, const vector<string> &data2) {
    vector<tuple<size_t, size_t>> possible_matches;
    double delta = 0.8;

    vector<vector<string_view>> tokenized_data1, tokenized_data2;

    for (string s : data1) {
        tokenized_data1.push_back(*tokenize_string_views(string_view(s), DEFAULT_DELIMITERS));
    }
    for (string s : data2) {
        tokenized_data2.push_back(*tokenize_string_views(string_view(s), DEFAULT_DELIMITERS));
    }

    auto start = high_resolution_clock::now();

    for (size_t i = 0; i < data1.size(); i++) {
        for (size_t j = 0; j < data2.size(); j++) {

            multiset<string_view> sig1, sig2;
            vector<string_view> sig1_tmp, sig2_tmp;
            for (string_view t1 : tokenized_data1[i]) {
                for (string_view t2 : tokenized_data2[j]) {
                    tie(sig1_tmp, sig2_tmp) = partition_ned(t1, t2, delta);

                    for (auto part : sig1_tmp) {
                        sig1.insert(part);
                    }
                    for (auto part : sig2_tmp) {
                        sig2.insert(part);
                    }
                }
            }

            vector<string_view> tmp;
            set_intersection(sig1.begin(), sig1.end(), sig2.begin(), sig2.end(), inserter(tmp, tmp.begin()));

            if (!tmp.empty()) {
                possible_matches.push_back(make_tuple(i, j));
            }
        }
        if (i % 1000 == 0) cout << i << endl;
    }

    cout << "signatures: " <<
            duration_cast<duration<double>>(high_resolution_clock::now() - start).count() * 1000 << "ms" << endl;

    unique_ptr<vector<tuple<size_t, size_t, double>>> matches(new vector<tuple<size_t, size_t, double>>());
    double val;

    start = high_resolution_clock::now();

    size_t i, j;
    for (auto possible_match : possible_matches) {
        tie(i, j) = possible_match;
        if ((val = fuzzy_jaccard_similarity(data1[i], data2[j], delta)) >= 0.8 && val < 1) {
            matches->push_back(make_tuple(i, j, val));
        }
    }

    cout << "fuzzy_jaccard: " <<
            duration_cast<duration<double>>(high_resolution_clock::now() - start).count() * 1000 << "ms" << endl;

    return matches;
}
