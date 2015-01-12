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
    if (argv != 2) {
        cerr << "Usage: " << argc[0] << " FILE_NAME" << endl;
    }

    auto start = high_resolution_clock::now();
    vector<string> data = *read_data(argc[1]);
    cout << "read data: " <<
            duration_cast<duration<double>>(high_resolution_clock::now() - start).count() * 1000 << "ms" << endl;

    start = high_resolution_clock::now();
    auto matches = *fast_join(data, data);
    cout << "fast join: " <<
            duration_cast<duration<double>>(high_resolution_clock::now() - start).count() * 1000 << "ms" << endl;

    for (auto &match : matches) {
        cout << get<2>(match) << " " << get<0>(match) << " " << get<1>(match) << endl;
        cout << data[get<0>(match)] << " " << data[get<1>(match)] << endl << endl;
    }

    return 0;
}
