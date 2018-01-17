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

static int i = 0;
// convert image to vec_t
void convert_image(const string &imagefilename/*, int i*/,
                   vector<vec_t> &data/*, vec_t &roi*/)
{
  // Load network
  // network<sequential> net;
  // net << conv(50, 50, 5, 3, 12, padding::same) << sigmoid()
  //     << max_pool(50, 50, 12, 2)
  //     << conv(25, 25, 5, 12, 36, padding::same) << sigmoid()
  //     << max_pool(25, 25, 36, 2)
  //     << dropout_layer(12 * 12 * 36, 0.3)
  //     << fc(12 * 12 * 36, 150) << sigmoid()
  //     << fc(150, 8) << softmax();
  // ifstream input("nets.txt");
  // input >> net;


  // VideoCapture video(imagefilename);
  // int ex = static_cast<int>(video.get(CV_CAP_PROP_FOURCC));
  // VideoWriter outputVideo;
  // outputVideo.open("output.avi", ex, video.get(CV_CAP_PROP_FPS), Size(640, 480), true);
  // if (!outputVideo.isOpened()) {
  //   cout << "Could not open the output video for write" << endl;
  //   return;
  // }

  // Rect result;
  // Mat img[5];
  // Mat frame;
  // int i = 0, j = -1;
  // bool f1 = false, f2 = false;
  // for(int i = 0; i < 5; i++) {
  //   video >> frame;
  //   frame.copyTo(img[i]);
  // }

  // for (;;) {
  //   if(i > 2) {
  //     f1 = true;
  //   }
  //   else j++;
  //   if(f1 && !f2) {
  //     video >> frame;
  //     if(frame.empty())
  //       f2 = true;
  //     else {
  //       for(int j = 0; j < 4; j++)
  //         img[j + 1].copyTo(img[j]);
  //       frame.copyTo(img[4]);
  //     }
  //   }
  //   if(f2) {
  //     j++;
  //   }
  //   if(j > 4) break;
  //   outputVideo << image_processings(img, i, j, net);
  //   i++;
  // }

  vec_t result;
  int i = 0;
  Mat img;

  // VideoCapture video(imagefilename);
  // for (;;) {
  //   for(int j = 0; j < 10; j++) {
  //     video >> img;
  //     if (img.empty()) {
  //       cout << "End." << endl;
  //       return;
  //     }
  //   }
  //     image_processing(img, result, i /*, net*/);
  //     i++;
  // }

  img = imread(imagefilename, IMREAD_COLOR);
  if(img.empty()) {
    cout << "Could not open or find the image" << endl;
    return;
  }
  image_processing(img, result, i);
  data.push_back(result);
}

// convert all images found in directory to vec_t
void convert_images(const string &directory,
                    vector<vec_t> &data, vector<label_t> &y, label_t label, int n)
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
    for(int j = 0; j < n; j++) {
      y.push_back(label);
      convert_image(imgs[i], data);
    }
  }

  cout << "Successfully opened " + to_string(data.size()) + " files!\n";
}