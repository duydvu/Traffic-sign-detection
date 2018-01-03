#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "tiny_dnn/tiny_dnn.h"
using namespace tiny_dnn;
using namespace std;

void read_csv(string file_name, vector<vec_t> &data) {
    ifstream file(file_name);
    string line;
    getline(file, line);   // ignore first line

    while (getline(file, line)) {
        stringstream lineStream(line);
        string cell;

        int i = 0;
        while (getline(lineStream, cell, ',')) {
            i++;
            if(i % 8 == 0) {
                vec_t vec = {0, 0, 0, 0, 0, 0, 0, 0};
                vec[ stod(cell) - 1 ] = 1;
                data.push_back(vec);
            }
        }
        // This checks for a trailing comma with no data after it.
        // if (!lineStream && cell.empty()) {
        //     If there was a trailing comma then add an empty element.
        //     result.push_back("");
        // }
    }
}