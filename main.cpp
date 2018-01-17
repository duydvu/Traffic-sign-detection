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

  // // Read images
  cout << "Reading images..." << endl;
  vector<vec_t> x, _x[11], x_test, _x_test[11];
  vector<label_t> y, _y[11], y_test, _y_test[11];
  int n[11] = { 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1};
  // convert_image(argv[1], x);
  for (int i = 0; i < 11; i++) {
    // Read training input
    convert_images("./data/Real/" + to_string(i + 1), _x[i], _y[i], i, n[i]);
    x.insert(x.end(), _x[i].begin(), _x[i].end());
    y.insert(y.end(), _y[i].begin(), _y[i].end());
    // Read testing input
    convert_images("./data/Testing/" + to_string(i + 1), _x_test[i], _y_test[i], i, 1);
    x_test.insert(x_test.end(), _x_test[i].begin(), _x_test[i].end());
    y_test.insert(y_test.end(), _y_test[i].begin(), _y_test[i].end());
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
  cout << "Start training..." << endl;
  net << conv(50, 50, 5, 3, 12, padding::same) << sigmoid()
      << max_pool(50, 50, 12, 2)
      << conv(25, 25, 5, 12, 24, padding::same) << sigmoid()
      << max_pool(25, 25, 24, 2)
      << conv(12, 12, 3, 24, 48, padding::same) << sigmoid()
      << max_pool(12, 12, 48, 2)
      << dropout_layer(6 * 6 * 48, 0.3)
      << fc(6 * 6 * 48, 150) << sigmoid()
      << fc(150, 11) << softmax();

  // Load network
  // ifstream input("nets.txt");
  // input >> net;

  int epochs = 1000;
  int batch = 50;
  int i = 0;

  // Training
  net.train<cross_entropy>(opt, x, y, batch, epochs, [&]() {
    cout << "||" << flush; 
  }, [&]() {
    cout << "\nEpoch #" << ++i << endl;
    result res[11];
    int success = 0, total = 0;
    for (int i = 0; i < 11; i++) {
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

  return 0;
}