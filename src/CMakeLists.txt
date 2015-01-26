set(SOURCE_FILES fast_join.cpp
                 fuzzy_signature.cpp
                 fuzzy_similarity.cpp
                 tokenize.cpp
                 levenshtein.cpp)
add_library(fast_join ${SOURCE_FILES})
