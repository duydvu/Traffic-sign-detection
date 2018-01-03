#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <boost/foreach.hpp>
#include "tiny_dnn/tiny_dnn.h"
#include <boost/filesystem.hpp>
using namespace tiny_dnn;
using namespace boost::filesystem;
using namespace std;
using namespace cv;

int w = 100;
int h = 100;
static int i;

// convert image to vec_t
void convert_image(const string &imagefilename,
                      vector<vec_t> &data)
{
  vec_t result;
  VideoCapture video("./data/cuocduaso/dethimau/MVI_1049.avi");
  const char *ORIGINAL = "Original";
  const char *BRIGHTEN = "BRIGHTEN";
  const char *FINAL = "Final";
  namedWindow(ORIGINAL, WINDOW_AUTOSIZE);
  namedWindow(BRIGHTEN, WINDOW_AUTOSIZE);
  namedWindow(FINAL, WINDOW_AUTOSIZE);
  Mat img;

  // for (;;) {
  //   video >> img;
  //   if (img.empty())
  //   {
  //     cout << " < < <  Game over!  > > > ";
  //     break;
  //   }
    img = imread(imagefilename, IMREAD_COLOR);
    resize(img, img, Size(w, h));

    // blur image
    cv::medianBlur(img, img, 3);

    // calculate brightness
    Mat gray;
    cvtColor(img, gray, cv::COLOR_RGB2GRAY);
    Scalar summ = sum(gray);
    double brightness = summ[0] / ((pow(2, 8) - 1) * img.rows * img.cols) * 2;
    
    // convert to hsv image
    Mat hsv_image;
    cvtColor(img, hsv_image, cv::COLOR_BGR2HSV);
    
    // if the image is to dark, lighten it up
    if(brightness <= 0.75) {
      for(int i = 0; i < img.rows; i++) {
        for(int j = 0; j < img.cols * 3; j += 3) {
          hsv_image.at<uint8_t>(i, j + 2) = (uint8_t)min((double)hsv_image.at<uint8_t>(i, j + 2) * 0.75 / brightness, (double)255);
        }
      }
    }

    // Extract red regions
    Mat lower_red_hue_range;
    Mat upper_red_hue_range;
    inRange(hsv_image, cv::Scalar(0, 100, 50), cv::Scalar(10, 255, 255), lower_red_hue_range);
    inRange(hsv_image, cv::Scalar(160, 100, 50), cv::Scalar(179, 255, 255), upper_red_hue_range);

    // Extract blue regions
    Mat blue_hue_range;
    inRange(hsv_image, cv::Scalar(80, 100, 50), cv::Scalar(140, 255, 255), blue_hue_range);

    Mat final_;
    addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, final_);
    addWeighted(blue_hue_range, 1.0, final_, 1.0, 0.0, final_);

    // blur
    GaussianBlur(final_, final_, cv::Size(9, 9), 2, 2);

    // show images
    imshow(ORIGINAL, img);
    cvtColor(hsv_image, hsv_image, cv::COLOR_HSV2BGR);
    imshow(BRIGHTEN, hsv_image);
    imshow(FINAL, final_);
    cout << "#" << ++i << ". Number of channels: " << final_.channels() << endl;
    cout << "Brightness: " << brightness << endl;
    waitKey(0);

    for (int i = 0; i < final_.rows; i++) {
      for (int j = 0; j < final_.cols; j++) {
        result.push_back((double)final_.at<uint8_t>(i, j));
      }
    }

  // }

  data.push_back(result);
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