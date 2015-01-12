#include <cmath>
#include <tuple>
#include "fuzzy_signature.h"

tuple<vector<string>, vector<string>> partition_ned(const string &t, const string &t_prime, double delta) {
    uint64_t max_edit_distance = (uint64_t) floor(((1 - delta) / delta) * t_prime.length());
    uint64_t partition_count = (uint64_t) ceil((max_edit_distance + 1) / (double) 2);
    uint64_t partition_len = (uint64_t) floor(t_prime.length() / (double) partition_count);

    vector<string> t_prime_partitions; // possible to improve speed with char* hackeria
    for (uint64_t i = 0; i < partition_count; i++) {
        t_prime_partitions.push_back(t_prime.substr(i * partition_len, partition_len));
    }

    uint64_t t_t2_edit_distance_threshold = (uint64_t) ceil((1 - delta) * max(t.size(), t_prime.size()));
    uint64_t token1_length_range_min = partition_len - (uint64_t) floor(t_t2_edit_distance_threshold / 2.0);
    uint64_t token1_length_range_max = partition_len + (uint64_t) ceil(t_t2_edit_distance_threshold / 2.0);
    vector<string> token1_partitions;
    // CASE 1 first partition
    for (uint64_t i = token1_length_range_min; i < token1_length_range_max; i++) {
        token1_partitions.push_back(t.substr(0, i));
    }
    // CASE 2 last partition
    for (uint64_t i = token1_length_range_max - 1; i >= token1_length_range_min; i--) {
        token1_partitions.push_back(t.substr(t.length() - i, i));
    }
    // CASE 3 middle partitions
    for (uint64_t i = 1; i <= t.length() - token1_length_range_min; i++) {
        for (uint64_t l = token1_length_range_min; l < token1_length_range_max && l + i <= t.length(); l++) {
            token1_partitions.push_back(t.substr(i, l));
        }
    }

    // TODO: pruning

    return make_tuple(token1_partitions, t_prime_partitions);
}
