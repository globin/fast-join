set(SOURCE_FILES fast_join.cpp
                 fuzzy_signature.cpp
                 fuzzy_similarity.cpp
                 tokenize.cpp)
add_library(fast_join ${SOURCE_FILES})
target_link_libraries(fast_join simmetrics)
