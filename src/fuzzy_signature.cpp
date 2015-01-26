#include <cmath>
#include <cstdlib>
#include <memory>
#include <tuple>
#include <algorithm>
#include "fuzzy_signature.h"

using namespace std::experimental;

bool edit_distance_prune(int64_t len, int64_t partition_len, int64_t t_prime_partition_num, int64_t start, int64_t t_len,
                         int64_t t_prime_len, int64_t t_t2_edit_distance_threshold) {

    int64_t minimal_ed = abs(len - partition_len) + abs(t_prime_partition_num * partition_len - start)
            + abs((t_len - (start + len)) - (t_prime_len - (t_prime_partition_num + 1) * partition_len));

    return minimal_ed > t_t2_edit_distance_threshold;
}

bool duplication_prune(int64_t start, int64_t t_prime_partition_num, int64_t partition_len,
                       int64_t t_t2_edit_distance_threshold, int64_t t_prime_partition_count) {
    return start > t_prime_partition_num * partition_len + t_t2_edit_distance_threshold - 2 * (t_prime_partition_count - t_prime_partition_num - 1) ||
           start < t_prime_partition_num * partition_len - (t_t2_edit_distance_threshold - 2 * (t_prime_partition_count - t_prime_partition_num - 1));
}

unique_ptr<tuple<vector<string_view>, vector<string_view>>>
partition_ned(const string_view &t, const string_view &t_prime, double delta) {
    int64_t t_len = t.length();
    int64_t t_prime_len = t_prime.length();

    int64_t max_edit_distance = (int64_t) floor(((1 - delta) / delta) * t_prime_len);
    int64_t partition_count = (int64_t) ceil((max_edit_distance + 1) / (double) 2);
    int64_t partition_len = (int64_t) floor(t_prime_len / (double) partition_count);

    vector<string_view> t_prime_partitions;
    for (int64_t i = 0; i < partition_count; i++) {
        t_prime_partitions.push_back(t_prime.substr(i * partition_len, partition_len));
    }

    int64_t t_t2_edit_distance_threshold = (int64_t) ceil((1 - delta) * max(t_len, t_prime_len));
    int64_t zero = 0;
    int64_t token1_length_range_min = max(zero, min(t_len, partition_len - (int64_t) floor(t_t2_edit_distance_threshold / 2.0)));
    int64_t token1_length_range_max = max(zero, min(t_len, partition_len + (int64_t) ceil(t_t2_edit_distance_threshold / 2.0)));
    vector<string_view> token1_partitions;
    // CASE 1 first partition
    for (int64_t i = token1_length_range_min; i < token1_length_range_max; i++) {
        if (!edit_distance_prune(i, partition_len, 0, 0, t_len, t_prime_len, t_t2_edit_distance_threshold)) {
            token1_partitions.push_back(t.substr(0, i));
        }
    }
    // CASE 2 last partition
    for (int64_t i = token1_length_range_max - 1; i >= token1_length_range_min; i--) {
        if (!edit_distance_prune(i, partition_len, partition_count - 1, t_len - i, t_len, t_prime_len, t_t2_edit_distance_threshold)) {
            token1_partitions.push_back(t.substr(t_len - i, i));
        }
    }
    // CASE 3 middle partitions
    for (int64_t t_prime_partition_num = partition_count - 2; t_prime_partition_num > 0; t_prime_partition_num--) {
        for (int64_t start = min((int64_t) 1, t_prime_partition_num * partition_len - t_t2_edit_distance_threshold);
             start <= max(t_len - 1, t_prime_partition_num * partition_len + t_t2_edit_distance_threshold);
             start++) {
            for (int64_t len = token1_length_range_min; len < token1_length_range_max && len + start <= t_len; len++) {
                if (!edit_distance_prune(len, partition_len, t_prime_partition_num, start, t_len, t_prime_len, t_t2_edit_distance_threshold) &&
                    !duplication_prune(start, t_prime_partition_num, partition_len, t_t2_edit_distance_threshold, partition_count)) {
                    token1_partitions.push_back(t.substr(start, len));
                }
            }
        }
    }

    unique_ptr<tuple<vector<string_view>, vector<string_view>>> result(new tuple<vector<string_view>, vector<string_view>>(token1_partitions, t_prime_partitions));
    return result;
}
