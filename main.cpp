#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <boost/foreach.hpp>
#include "tiny_dnn/tiny_dnn.h"
#include <boost/filesystem.hpp>
#include "source/convert.h"
#include "source/read_csv.h"

using namespace tiny_dnn;
using namespace tiny_dnn::activation;
using namespace tiny_dnn::layers;
using namespace std;
using namespace cv;

int main(int argc, char **argv) {

  // if(argc != 2) {
  //   cout << "Must be 1 parameter!" << endl;
  //   return 0;
  // }

  // Read labels
  cout << "Reading labels..." << endl;
  vector<label_t> y, _y[7], y_test, _y_test[7];         // Label
  vector<vec_t> roi, _roi[7], roi_test, _roi_test[7];   // Region of interest
  for (int i = 0; i < 7; i++) {
    // Read training labels
    read_csv("./data/Real/" + to_string(i + 1) + ".csv", _y[i], _roi[i]);
    y.insert(y.end(), _y[i].begin(), _y[i].end());
    roi.insert(roi.end(), _roi[i].begin(), _roi[i].end());
    // Read testing labels
    read_csv("./data/Testing/" + to_string(i + 1) + ".csv", _y_test[i], _roi_test[i]);
    y_test.insert(y_test.end(), _y_test[i].begin(), _y_test[i].end());
    roi_test.insert(roi_test.end(), _roi_test[i].begin(), _roi_test[i].end());
  }
  cout << "Finished reading " + to_string(y.size()) + " training labels." << endl;
  cout << "Finished reading " + to_string(y_test.size()) + " testing labels." << endl;
  cout << "Finished reading " + to_string(roi.size()) + " training vectors." << endl;
  cout << "Finished reading " + to_string(roi_test.size()) + " testing vectors." << endl;

  // Read images
  cout << "Reading images..." << endl;
  vector<vec_t> x, _x[7], x_test, _x_test[7];
  // convert_image(argv[1], x);
  for (int i = 0; i < 7; i++) {
    // Read training input
    convert_images("./data/Real/" + to_string(i + 1), _x[i], _roi[i]);
    x.insert(x.end(), _x[i].begin(), _x[i].end());
    // Read testing input
    convert_images("./data/Testing/" + to_string(i + 1), _x_test[i], _roi_test[i]);
    x_test.insert(x_test.end(), _x_test[i].begin(), _x_test[i].end());
  }
  cout << "Finished reading " + to_string(x.size()) + " training images." << endl;
  cout << "Finished reading " + to_string(x_test.size()) + " testing images." << endl;

  // shuffle data
  srand(time(NULL));
  for (int i = 0; i < y.size(); i++) {
    int ran = rand() % y.size();
    iter_swap(x.begin() + i, x.begin() + ran);
    iter_swap(y.begin() + i, y.begin() + ran);
  }

  network<sequential> net;
  adagrad opt;

  net << conv(50, 50, 5, 4, 4, padding::same) << sigmoid()
      << max_pool(50, 50, 4, 2) << sigmoid()
      << conv(25, 25, 5, 4, 4, padding::same) << sigmoid()
      << max_pool(25, 25, 4, 2) << sigmoid()
      << max_pool(12, 12, 4, 2) << sigmoid()
      << fc(6 * 6 * 4, 120) << sigmoid()
      << fc(120, 120) << sigmoid()
      << fc(120, 8) << softmax();

  // Load network
  // ifstream input("nets.txt");
  // input >> net;

  int epochs = 100;
  int batch = 100;
  int i = 0;

  // Training
  net.train<cross_entropy>(opt, x, y, batch, epochs, [&]() {
    cout << "||" << flush; 
  }, [&]() {
    cout << "\nEpoch #" + to_string(++i) + ".\n";
    result res[7];
    int success = 0, total = 0;
    for (int i = 0; i < 7; i++) {
      res[i] = net.test(_x_test[i], _y_test[i]);
      success += res[i].num_success;
      total += res[i].num_total;
      cout << res[i].num_success << "/" << res[i].num_total << ". " << to_string(res[i].num_success * 100 / (double)res[i].num_total) << "%" << endl;
    }
    cout << "Overall: " << success << "/" << total << ". " << to_string(success * 100 / (double)total) << "%" << endl;
    // Save network
    ofstream output("nets1.txt");
    output << net; 
  });

  // Testing
  // label_t y_label = net.predict_label(x[0]);
  // cout << "Label: " << y_label + 1 << endl;

  return 0;
}