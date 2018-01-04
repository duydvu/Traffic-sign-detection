#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "tiny_dnn/tiny_dnn.h"
using namespace tiny_dnn;
using namespace std;
using namespace cv;

int w = 600;
int h = 480;

void image_processing(Mat &img, vec_t &result, int i) {
    const char *ORIGINAL = "Original";
    const char *BRIGHTEN = "BRIGHTEN";
    const char *FINAL = "Final";
    namedWindow(ORIGINAL, WINDOW_AUTOSIZE);
    namedWindow(BRIGHTEN, WINDOW_AUTOSIZE);
    namedWindow(FINAL, WINDOW_AUTOSIZE);

    resize(img, img, Size(w, h));

    // calculate brightness
    Mat gray;
    cvtColor(img, gray, cv::COLOR_RGB2GRAY);
    Scalar summ = sum(gray);
    double brightness = summ[0] / ((pow(2, 8) - 1) * img.rows * img.cols) * 2;

    // convert to hsv image
    Mat hsv_image;
    cvtColor(img, hsv_image, cv::COLOR_BGR2HSV);

    // if the image is to dark, lighten it up
    if (brightness <= 0.75)
    {
        for (int i = 0; i < img.rows; i++)
        {
            for (int j = 0; j < img.cols * 3; j += 3)
            {
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

    // Combine all regions
    Mat final_;
    addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, final_);
    addWeighted(blue_hue_range, 1.0, final_, 1.0, 0.0, final_);

    // blur to reduce noise
    GaussianBlur(final_, final_, cv::Size(9, 9), 2, 2);

    // detect circle
    vector<vector<Point>> contours;
    Mat bimage = final_ >= 70;
    Mat b1, b2;

    findContours(bimage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

    Mat cimage = Mat::zeros(bimage.size(), CV_8UC3);

    for (size_t i = 0; i < contours.size(); i++)
    {
        size_t count = contours[i].size();
        if (count < 6)
            continue;

        Mat pointsf;
        Mat(contours[i]).convertTo(pointsf, CV_32F);
        RotatedRect box = fitEllipse(pointsf);

        if (MAX(box.size.width, box.size.height) > MIN(box.size.width, box.size.height) * 3)
            continue;
        drawContours(cimage, contours, (int)0, Scalar::all(255), 1, 8);

        ellipse(img, box, Scalar(0, 0, 255), 1, CV_AA);
        ellipse(img, box.center, box.size * 0.5f, box.angle, 0, 360, Scalar(0, 255, 255), 1, CV_AA);
        Point2f vtx[4];
        box.points(vtx);
        for (int j = 0; j < 4; j++)
            line(img, vtx[j], vtx[(j + 1) % 4], Scalar(0, 255, 0), 1, CV_AA);
    }

    // detect circles
    // vector<Vec3f> circles;
    // HoughCircles(final_, circles, CV_HOUGH_GRADIENT, 4/3.0, final_.rows, 200, 20, 10, 75);
    // cout << "Number of circles: " << circles.size() << endl;
    // for (size_t i = 0; i < circles.size(); i++)
    // {
    //   Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
    //   int radius = cvRound(circles[i][2]);
    //   // circle center
    //   circle(img, center, 3, Scalar(0, 255, 0), -1, 8, 0);
    //   // circle outline
    //   circle(img, center, radius, Scalar(0, 0, 255), 3, 8, 0);
    // }

    // show images
    imshow(ORIGINAL, img);
    cvtColor(hsv_image, hsv_image, cv::COLOR_HSV2BGR);
    imshow(BRIGHTEN, cimage);
    imshow(FINAL, final_);
    cout << "#" << ++i << ". Number of channels: " << final_.channels() << endl;
    cout << "Brightness: " << brightness << endl;
    waitKey(0);

    // for (int i = 0; i < final_.rows; i++) {
    //   for (int j = 0; j < final_.cols; j++) {
    //     result.push_back((double)final_.at<uint8_t>(i, j));
    //   }
    // }
}