#include <gtest/gtest.h>
#include <simmetrics/tokenizer.h>

#include "fast_join.h"
#include "fuzzy_similarity.h"

TEST(FastJoinTest, ReturnCode) {
    ASSERT_EQ(0, fast_join());
}

TEST(FuzzyJaccardSimilarity, Value) {
    ASSERT_DOUBLE_EQ(15.0/17.0, fuzzy_jaccard_similarity("nba mcgrady", "macgrady nba", 0.8));
}

TEST(FuzzyOverlapTest, BuildMap) {
    ASSERT_EQ(2, std::get<0>(*fuzzy_overlap("test fnord", "test fnord", 0.8, WHITESPACE_DELIMITERS)).size());
    ASSERT_EQ(1, std::get<0>(*fuzzy_overlap("test fnord", "test foo", 0.8, WHITESPACE_DELIMITERS)).size());
    ASSERT_EQ(1, std::get<0>(*fuzzy_overlap("test fnord", "test foo foo", 0.8, WHITESPACE_DELIMITERS)).size());
    ASSERT_EQ(2, std::get<0>(*fuzzy_overlap("test fnord", "test fnord fnord", 0.8, WHITESPACE_DELIMITERS)).size());
    ASSERT_EQ(1, std::get<0>(*fuzzy_overlap("test foo", "test fnord", 0.8, WHITESPACE_DELIMITERS)).size());
    ASSERT_EQ(1, std::get<0>(*fuzzy_overlap("test foo foo", "test fnord", 0.8, WHITESPACE_DELIMITERS)).size());
    ASSERT_EQ(2, std::get<0>(*fuzzy_overlap("test fnord fnord", "test fnord", 0.8, WHITESPACE_DELIMITERS)).size());
}

TEST(FuzzyOverlapWeightTest, Weights) {
    EXPECT_DOUBLE_EQ(2, fuzzy_overlap_weight(std::get<0>(*fuzzy_overlap("test fnord", "test fnord", 0.8, WHITESPACE_DELIMITERS))));
    EXPECT_DOUBLE_EQ(1.8, fuzzy_overlap_weight(std::get<0>(*fuzzy_overlap("test fnord", "test fnors", 0.8, WHITESPACE_DELIMITERS))));
    EXPECT_DOUBLE_EQ(1, fuzzy_overlap_weight(std::get<0>(*fuzzy_overlap("test fnord", "test foo", 0.8, WHITESPACE_DELIMITERS))));
    EXPECT_DOUBLE_EQ(0.8, fuzzy_overlap_weight(std::get<0>(*fuzzy_overlap("tesst fnord", "testt foo", 0.8, WHITESPACE_DELIMITERS))));
    EXPECT_DOUBLE_EQ(1, fuzzy_overlap_weight(std::get<0>(*fuzzy_overlap("test fnord", "test foo foo", 0.8, WHITESPACE_DELIMITERS))));
    EXPECT_DOUBLE_EQ(2, fuzzy_overlap_weight(std::get<0>(*fuzzy_overlap("test fnord", "test fnord fnord", 0.8, WHITESPACE_DELIMITERS))));
    EXPECT_DOUBLE_EQ(1, fuzzy_overlap_weight(std::get<0>(*fuzzy_overlap("test foo", "test fnord", 0.8, WHITESPACE_DELIMITERS))));
    EXPECT_DOUBLE_EQ(0.8, fuzzy_overlap_weight(std::get<0>(*fuzzy_overlap("tesst foo", "testt fnord", 0.8, WHITESPACE_DELIMITERS))));
    EXPECT_DOUBLE_EQ(1, fuzzy_overlap_weight(std::get<0>(*fuzzy_overlap("test foo foo", "test fnord", 0.8, WHITESPACE_DELIMITERS))));
    EXPECT_DOUBLE_EQ(0.8, fuzzy_overlap_weight(std::get<0>(*fuzzy_overlap("tesst foo foo", "testt fnord", 0.8, WHITESPACE_DELIMITERS))));
    EXPECT_DOUBLE_EQ(2, fuzzy_overlap_weight(std::get<0>(*fuzzy_overlap("test fnord fnord", "test fnord", 0.8, WHITESPACE_DELIMITERS))));
    EXPECT_DOUBLE_EQ(2, fuzzy_overlap_weight(std::get<0>(*fuzzy_overlap("test fnord", "fnord test", 0.8, WHITESPACE_DELIMITERS))));
    EXPECT_DOUBLE_EQ(0, fuzzy_overlap_weight(std::get<0>(*fuzzy_overlap("ttttt fffff", "fnord test", 0.8, WHITESPACE_DELIMITERS))));
}

TEST(TokenizeTest, WhitespaceTokenize) {
    ASSERT_EQ(2, tokenize_to_vector("test fnord", WHITESPACE_DELIMITERS)->size());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
