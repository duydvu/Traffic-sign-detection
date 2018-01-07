#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "tiny_dnn/tiny_dnn.h"
using namespace tiny_dnn;
using namespace std;

void read_csv(string file_name, vector<label_t> &y, vector<vec_t> &roi) {
    ifstream file(file_name);
    string line;
    getline(file, line); // ignore first line

    while (getline(file, line))
    {
        stringstream lineStream(line);
        string cell;

        int i = 0;
        vec_t _roi;
        while (getline(lineStream, cell, ','))
        {
            i++;
            if (i == 4 || i == 5 || i == 6 || i == 7)
                _roi.push_back(stod(cell));
            if (i == 7) {
                roi.push_back(_roi);
                roi.push_back(_roi);
                roi.push_back(_roi);
            }
            if (i == 8) {
                y.push_back(stod(cell) - 1);
                y.push_back(stod(cell) - 1);
                y.push_back(stod(cell) - 1);
            }
        }
        // This checks for a trailing comma with no data after it.
        // if (!lineStream && cell.empty()) {
        //     If there was a trailing comma then add an empty element.
        //     result.push_back("");
        // }
    }
}