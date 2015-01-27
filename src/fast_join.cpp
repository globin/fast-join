#include <algorithm>
#include <iostream>
#include <chrono>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "fast_join.h"
#include "fuzzy_signature.h"
#include "fuzzy_similarity.h"
#include "tokenize.h"
#include "hash_tuple.h"

using namespace chrono;

unique_ptr<vector<tuple<size_t, size_t, double>>> fast_join(const vector<string> &data1, const vector<string> &data2) {
    double delta = 0.8;
    double tau = 0.8;

    vector<vector<string_view>> tokenized_data1, tokenized_data2;

    for (const string s : data1) {
        tokenized_data1.push_back(*tokenize_string_views(string_view(s), DEFAULT_DELIMITERS));
    }
    for (const string s : data2) {
        tokenized_data2.push_back(*tokenize_string_views(string_view(s), DEFAULT_DELIMITERS));
    }

    auto start = high_resolution_clock::now();

    unordered_multimap<string_view, size_t> inverted_index1;
    for (size_t i = 0; i < tokenized_data1.size(); i++) {
        for (const auto& sig : token_sensitive_sig(tokenized_data1[i], tau * tokenized_data1[i].size())) {
            inverted_index1.emplace(sig, i);
        }
    }

    unordered_multimap<string_view, size_t> inverted_index2;
    for (size_t i = 0; i < tokenized_data1.size(); i++) {
        for (const auto& sig : token_sensitive_sig(tokenized_data2[i], tau * tokenized_data2[i].size())) {
            inverted_index2.emplace(sig, i);
        }
    }

    cout << "signatures: " <<
            duration_cast<duration<double>>(high_resolution_clock::now() - start).count() * 1000 << "ms" << endl;

    start = high_resolution_clock::now();

    vector<tuple<size_t, size_t>> possible_matches;
    for (auto iter = inverted_index1.begin(); iter != inverted_index1.end(); iter++) {
        auto index2_range = inverted_index2.equal_range(iter->first);
        for (auto inner_iter = index2_range.first; inner_iter != index2_range.second; inner_iter++) {
            possible_matches.push_back(make_tuple(iter->second, inner_iter->second));
        }
    }
    cout << "filter: " <<
            duration_cast<duration<double>>(high_resolution_clock::now() - start).count() * 1000 << "ms" << endl;
    cout << "possible matches: " << possible_matches.size() << endl;

    start = high_resolution_clock::now();

    double val;
    size_t i, j;
    unique_ptr<vector<tuple<size_t, size_t, double>>> matches(new vector<tuple<size_t, size_t, double>>());
    for (const auto &possible_match : possible_matches) {
        tie(i, j) = possible_match;
        if ((val = fuzzy_jaccard_similarity(data1[i], data2[j], delta)) >= 0.8 && val < 1) {
            matches->push_back(make_tuple(i, j, val));
        }
    }

    cout << "fuzzy_jaccard: " <<
            duration_cast<duration<double>>(high_resolution_clock::now() - start).count() * 1000 << "ms" << endl;

    return matches;
}
