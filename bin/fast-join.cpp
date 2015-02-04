#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <memory>

#include "fast_join.h"

using namespace std;
using namespace chrono;

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

int main(int argv, char** argc) {
    if (argv < 2 || argv > 3) {
        cerr << "Usage: " << argc[0] << " FILE_NAME FILE_NAME2" << endl;
        return 1;
    }

    auto start = high_resolution_clock::now();
    vector<string> data = *read_data(argc[1]);
    cout << "read data: " <<
            duration_cast<duration<double>>(high_resolution_clock::now() - start).count() * 1000 << "ms" << endl;

    vector<string> data2;
    if (argv == 3) {
        data2 = *read_data(argc[2]);
    } else {
        data2 = data;
    }

    start = high_resolution_clock::now();
    auto matches = *fast_join(data, data2);
    cout << "fast join: " << duration_cast<duration<double>>(high_resolution_clock::now() - start).count() * 1000
         << "ms" << endl;

    cout << "matches: " << matches.size() << endl << endl;

    for (auto &match : matches) {
        cout << get<2>(match) << " " << get<0>(match) << " " << get<1>(match) << endl;
        cout << data[get<0>(match)] << endl << data[get<1>(match)] << endl << endl;
    }

    return 0;
}
