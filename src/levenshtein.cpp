#include "levenshtein.h"

unsigned int levenshtein_distance(const string_view &s1, const string_view &s2) {
    const size_t len1 = s1.size(), len2 = s2.size();
    vector<unsigned int> col(len2+1), prevCol(len2+1);

    for (unsigned int i = 0; i < prevCol.size(); i++) {
        prevCol[i] = i;
    }
    for (unsigned int i = 0; i < len1; i++) {
        col[0] = i + 1;
        for (unsigned int j = 0; j < len2; j++) {
            col[j+1] = min(min(prevCol[1 + j] + 1, col[j] + 1),
                    prevCol[j] + (s1[i]==s2[j] ? 0 : 1));
        }
        col.swap(prevCol);
    }

    return prevCol[len2];
}

double levenshtein_similarity(const string_view &str1, const string_view &str2) {

    size_t lev_dist = levenshtein_distance(str1, str2);

    double max_len = (double) str1.size();
    double str2_len = (double) str2.size();

    if (max_len < str2_len)
        max_len = str2_len;

    if (max_len == 0)
        return 1;
    else
        return 1.0 - ((double) lev_dist / max_len);
}
