#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <boost/foreach.hpp>
#include "tiny_dnn/tiny_dnn.h"
#include <boost/filesystem.hpp>
#include "image_processing.h"
using namespace tiny_dnn;
using namespace tiny_dnn::activation;
using namespace tiny_dnn::layers;
using namespace boost::filesystem;
using namespace std;
using namespace cv;


// convert image to vec_t
void convert_image(const string &imagefilename, int i,
                   vector<vec_t> &data, vec_t &roi)
{
  // Load network
  // network<sequential> net;
  // net << fc(25 * 25 * 4, 120) << sigmoid()
  //     << fc(120, 120) << sigmoid()
  //     << fc(120, 8) << softmax();
  // ifstream input("nets.txt");
  // input >> net;

  vec_t result;
  // VideoCapture video(imagefilename);

  Mat img;

  // for (;;) {
  //   video >> img;
  //   if (img.empty())
  //   {
  //     cout << " < < <  Game over!  > > > ";
  //     break;
  //   }
    img = imread(imagefilename, IMREAD_COLOR);
    if(img.empty()) {
      cout << "Could not open or find the image" << endl;
      return;
    }
    image_processing(img, result, roi, i);
    
  // }

  data.push_back(result);
}

// convert all images found in directory to vec_t
void convert_images(const string &directory,
                    vector<vec_t> &data, vector<vec_t> &roi)
{
  cout << "Open folder " + directory << endl;
  path dpath(directory);
  vector<string> imgs;

  BOOST_FOREACH (const path &p,
                 make_pair(directory_iterator(dpath), directory_iterator()))
  {
    if (is_directory(p))
      continue;
    imgs.push_back(p.string());
  }
  sort(imgs.begin(), imgs.end());
  for (int i = 0; i < imgs.size(); i++) {
    convert_image(imgs[i], i, data, roi[i * 3]);
    convert_image(imgs[i], i, data, roi[i * 3 + 1]);
    convert_image(imgs[i], i, data, roi[i * 3 + 2]);
  }

  cout << "Successfully opened " + to_string(data.size()) + " files!\n";
}