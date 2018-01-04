#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <boost/foreach.hpp>
#include "tiny_dnn/tiny_dnn.h"
#include <boost/filesystem.hpp>
#include "image_processing.h"
using namespace tiny_dnn;
using namespace boost::filesystem;
using namespace std;
using namespace cv;


static int i;

// convert image to vec_t
void convert_image(const string &imagefilename,
                      vector<vec_t> &data)
{
  vec_t result;
  VideoCapture video("./data/cuocduaso/dethimau/MVI_1054.avi");
  
  Mat img;

  for (;;) {
    video >> img;
    if (img.empty())
    {
      cout << " < < <  Game over!  > > > ";
      break;
    }
    // img = imread(imagefilename, IMREAD_COLOR);
    image_processing(img, result, i);

  }

  // data.push_back(result);
}

// convert all images found in directory to vec_t
void convert_images(const string &directory,
                    vector<vec_t> &data)
{
  cout << "Open folder " + directory << endl;
  path dpath(directory);
  vector<string> imgs;
  i = 0;

  BOOST_FOREACH (const path &p,
                 make_pair(directory_iterator(dpath), directory_iterator()))
  {
    if (is_directory(p))
      continue;
    imgs.push_back(p.string());
  }
  sort(imgs.begin(), imgs.end());
  for (int i = 0; i < imgs.size(); i++)
    convert_image(imgs[i], data);

  cout << "Successfully opened " + to_string(data.size()) + " files!\n";
}