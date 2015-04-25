#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <memory>

#include "fast_join.h"
#include "hadoop/Pipes.hh"
#include "hadoop/TemplateFactory.hh"
#include "hadoop/StringUtils.hh"

using namespace std;

class FastJoinMapper : public HadoopPipes::Mapper {
    public:
        FastJoinMapper(HadoopPipes::TaskContext& context) {
        }

        void map(HadoopPipes::MapContext& context) {
            string s = context.getInputValue();
            vector<string_view> tokenized_string = *tokenize_string_views(
                string_view(s), DEFAULT_DELIMITERS
            );

            for (const auto& sig : token_sensitive_sig(tokenized_string, tau * tokenized_string.size())) {
                context.emit(sig, s);
            }
        }
}

class FastJoinReducer : public HadoopPipes::Reducer {
    public:
        FastJoinReducer(HadoopPipes::TaskContext& context) {
        }

        void reduce(HadoopPipes::ReduceContext& context) {
            vector<string> possible_matches;
            while (context.nextValue()) {
                possible_matches.push_back(context.getInputValue());
            }

            do {
                if ((val = fuzzy_jaccard_similarity(possible_matches[0], possible_matches[1], delta)) >= 0.8) {
                    context.emit(possible_matches[0], possible_matches[1]);
                    context.emit(possible_matches[1], possible_matches[0]);
                }
            } while (next_permutation(possible_matches));
        }
}

unique_ptr<vector<string>> read_data(const string file_name) {

    unique_ptr<vector<string>> data(new vector<string>());

    string tmp;
    ifstream data_file(file_name, ifstream::in);
    if (data_file.is_open()) {
        while (!data_file.eof()) {
            getline(data_file, tmp);
            data->push_back(tmp);
        }
    }

    return data;
}

int main(int argc, char** argv) {
    return HadoopPipes::runTask(
        HadoopPipes::TemplateFactory<FastJoinMapper, FastJoinReducer>()
    );
}
