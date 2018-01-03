#include <iostream>
#include <string>
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

  // Read images
  cout << "Reading images...\n";
  vector<vec_t> x;
  convert_images("./data/Real/1", x);
  convert_images("./data/Real/2", x);
  convert_images("./data/Real/3", x);
  convert_images("./data/Real/4", x);
  convert_images("./data/Real/5", x);
  convert_images("./data/Real/6", x);
  convert_images("./data/Real/7", x);
  
  // Read labels
  cout << "Reading labels...\n";
  vector<vec_t> y;
  read_csv("./data/Real/1.csv", y);
  read_csv("./data/Real/4.csv", y);
  read_csv("./data/Real/5.csv", y);
  read_csv("./data/Real/6.csv", y);
  read_csv("./data/Real/7.csv", y);
  cout << "Finished reading " + to_string(y.size()) + " labels.\n";

  network<sequential> net;
  adagrad opt;

  net << fc(80 * 60 * 3, 120) << sigmoid()
      << fc(120, 8) << softmax();

  // Load network
  // ifstream input("nets.txt");
  // input >> net;

  int epochs = 50;
  int batch = 20;
  int i = 0;

  net.fit<mse>(opt, x, y, batch, epochs, [&]() {
    cout << "||" << flush;
  }, [&]() {
    auto loss = net.get_loss<mse>(x, y);
    cout << "\nEpoch #" + to_string(++i) + ". Loss: " + to_string(loss) << endl; 
    // Save network
    ofstream output("nets.txt");
    output << net;
  });


  return 0;
}