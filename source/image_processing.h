#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/shape/shape_distance.hpp>
#include "tiny_dnn/tiny_dnn.h"
using namespace tiny_dnn;
using namespace tiny_dnn::activation;
using namespace tiny_dnn::layers;
using namespace std;
using namespace cv;

int w = 50,
    h = 50;

Mat hsv_image;

const char *ORIGINAL = "Original";
const char *RED = "Red";
const char *BLUE = "Blue";
const char *WHITE = "White";
const char *CONTOURS = "Contours";

// Rect image_processing(Mat &img, int i);

// Mat image_processings(Mat Img[], int i, int j, network<sequential> &net) {
//     cout << "#" << i << endl;
//     Mat img[5];
//     Rect rect[5];
//     vector<int> non;
//     for (int k = 0; k < 5; k++) {
//         Img[k].copyTo(img[k]);
//         rect[k] = image_processing(img[k], i);
//         if(!rect[k].empty()) {
//             non.push_back(k);
//         }
//     }
//     if(non.size() > 1 && rect[j].empty()) {
//         for(int k = 1; k < non.size(); k++) {
//             if(non[k] > j && non[k - 1] < j) {
//                 Rect    a = rect[non[k]],
//                         b = rect[non[k - 1]];
//                 float   max = MAX(a.area(), b.area()),
//                         min = MIN(a.area(), b.area());
//                 if (max / min > 1.2)
//                     break;
//                 Point   c1(a.tl().x + a.size().width * 0.5f, a.tl().y + a.size().height * 0.5f),
//                         c2(b.tl().x + b.size().width * 0.5f, b.tl().y + b.size().height * 0.5f);
//                 float dis = sqrt((c1.x - c2.x) * (c1.x - c2.x) + (c1.y - c2.y) * (c1.y - c2.y));
//                 if (dis > 20)
//                     break;
//                 Size s((a.size().width + b.size().width) * 0.5f, (a.size().height + b.size().height) * 0.5f);
//                 Point tl = (c1 + c2) * 0.5f - (Point)s * 0.5f;
//                 rect[j] = Rect(Point(0, 0), Point(0, 0)) + tl + s;
//             }
//         }
//     }
//     if(!rect[j].empty()) {
//         Mat sign;
//         vec_t input;
//         Rect s = rect[j] + Size(20, 20) - Point(10, 10);
//         img[j](s & Rect(0, 0, img[j].cols, img[j].rows)).copyTo(sign);
//         GaussianBlur(sign, sign, cv::Size(3, 3), 2, 2);
//         resize(sign, sign, Size(w, h));
//         for (int k = 0; k < h; k++) {
//             for (int l = 0; l < w * 3; l += 3) {
//                 input.push_back((double)sign.at<uint8_t>(k, l));
//                 input.push_back((double)sign.at<uint8_t>(k, l + 1));
//                 input.push_back((double)sign.at<uint8_t>(k, l + 2));
//             }
//         }
//         putText(img[j], to_string(net.predict_label(input)), rect[j].tl(),
//                 FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 255, 0), 1, CV_AA);
//         rectangle(img[j], rect[j].tl(), rect[j].br(), Scalar(255, 0, 0), 2);
//     }
//     return img[j];
// }

// Rect image_processing(Mat &Img, int i) {
//     srand(time(NULL));

//     Mat img;
//     Img.copyTo(img);

//     // blur to reduce noise
//     GaussianBlur(img, img, cv::Size(3, 3), 2, 2);

//     // calculate brightness
//     Mat gray;
//     cvtColor(img, gray, cv::COLOR_RGB2GRAY);
//     Scalar summ = sum(gray);
//     double brightness = summ[0] / ((pow(2, 8) - 1) * img.rows * img.cols) * 2;

//     // convert to hsv image
//     cvtColor(img, hsv_image, cv::COLOR_BGR2HSV);

//     Mat lower_red_hue_range,
//         upper_red_hue_range,
//         red_hue_range,
//         blue_hue_range,
//         edge,
//         final_();
//     vector<vector<Point>> contours, c;
//     vector<Vec4i> hierarchy;
//     RotatedRect box;
//     int chosen = 0;
//     int area = 0;

//     inRange(hsv_image, cv::Scalar(0, 50, 50), cv::Scalar(10, 255, 255), lower_red_hue_range);
//     inRange(hsv_image, cv::Scalar(150, 50, 50), cv::Scalar(179, 255, 255), upper_red_hue_range);
//     addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_range);
//     inRange(hsv_image, cv::Scalar(90, 50, 50), cv::Scalar(110, 255, 255), blue_hue_range);

//     Canny(red_hue_range, edge, 50, 50 * 3, 3);
//     findContours(edge, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
//     Canny(blue_hue_range, edge, 50, 50 * 3, 3);
//     findContours(edge, c, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

//     contours.insert(contours.end(), c.begin(), c.end());

//     Mat con1(img.rows, img.cols, CV_8UC3, Scalar(0, 0, 0));
//     Mat con2(img.rows, img.cols, CV_8UC3, Scalar(0, 0, 0));

//     // merge close contours
//     float distance;
//     for (int i = 0; i < contours.size(); i++)
//     {
//         Moments a = moments(contours[i], true);
//         float ax = a.m10 / a.m00;
//         float ay = a.m01 / a.m00;
//         for (int j = i + 1; j < contours.size(); j++)
//         {
//             Moments b = moments(contours[j], true);
//             float bx = b.m10 / b.m00;
//             float by = b.m01 / b.m00;
//             distance = sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
//             if (distance < 10)
//             {
//                 contours[i].insert(contours[i].end(), contours[j].begin(), contours[j].end());
//                 contours.erase(contours.begin() + j);
//                 j--;
//             }
//         }
//     }

//     for (int i = 0; i < contours.size(); i++)
//     {
//         if (contours[i].size() <= 5)
//             continue;

//         box = fitEllipse(Mat(contours[i]));

//         int width = box.size.width, height = box.size.height;
//         if (MAX(width, height) > MIN(width, height) * 1.75)
//             continue;
//         if (MAX(width, height) < 20)
//             continue;
//         if (box.angle >= 30 && box.angle <= 150)
//             continue;
//         int x = box.center.x, y = box.center.y;
//         if (y - height * 0.5f < 0 || y + height * 0.5f > 440)
//             continue;
//         if (y * 4 >= x * 3 && (y - 480) * 4 >= -3 * x)
//             continue;
        
//         // convex hull
//         vector<Point> hull;
//         convexHull(contours[i], hull);

//         vector<Point> elip;
//         ellipse2Poly(box.center, box.size * 0.5f, box.angle, 0, 360, 10, elip);

//         // match shape
//         float shapeDif = matchShapes(hull, elip, CV_CONTOURS_MATCH_I1, 0);

//         // drawing
//         Scalar color(rand() % 256, rand() % 256, rand() % 256);
//         drawContours(con1, contours, i, color, 1, 8, hierarchy, 0, Point());
//         polylines(con2, hull, true, color);

//         polylines(img, elip, true, Scalar(0, 255, 0));
//         putText(img, to_string(shapeDif), box.center,
//                 FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0, 0, 0), 1, CV_AA);

//         bool flag = false;
//         for (int j = 0; j < hull.size(); j++)
//         {
//             Point a = hull[j] - hull[(j == 0) ? hull.size() - 1 : j - 1];
//             Point b = hull[(j == hull.size() - 1) ? 0 : j + 1] - hull[j];
//             double angle1 = atan2(a.y, a.x) * 180.0 / CV_PI;
//             double angle2 = atan2(b.y, b.x) * 180.0 / CV_PI;
//             double angle = angle2 - angle1;
//             if (angle < 0)
//                 angle += 360;
//             if (angle >= 60)
//                 flag = true;
//         }

//         if (shapeDif > 0.08 || flag)
//             continue;

//         int a;
//         if ((a = box.size.width * box.size.height) > area)
//         {
//             chosen = i;
//             area = a;
//         }
//     }
//     if (area > 0)
//     {
//         box = fitEllipse(Mat(contours[chosen]));
//         Point p1(box.center.x - box.size.width * 0.5f, box.center.y - box.size.height * 0.5f);
//         Point p2(box.center.x + box.size.width * 0.5f, box.center.y + box.size.height * 0.5f);
//         return Rect(p1, p2);
//     }

//     return Rect();
// }

// Mat img, upper_hue_range, lower_hue_range, hue_range;
// int minH, maxH,
//     minS, maxS,
//     minV, maxV;

// void extract(int, void *) {
//     if (maxH < minH) {
//         inRange(hsv_image, Scalar(0   , minS, minV), Scalar(maxH, maxS, maxV), lower_hue_range);
//         inRange(hsv_image, Scalar(minH, minS, minV), Scalar(179 , maxS, maxV), upper_hue_range);
//         addWeighted(lower_hue_range, 1.0, upper_hue_range, 1.0, 0.0, hue_range);
//     }
//     else
//         inRange(hsv_image, Scalar(minH, minS, minV), Scalar(maxH, maxS, maxV), hue_range);

//     // show images
//     imshow("RESULT", hue_range);
// }

// void image_processing(Mat &Img, vec_t &result, int i) {
//     cout << "#" << i << endl;

//     Img.copyTo(img);

//     // blur to reduce noise
//     GaussianBlur(img, img, cv::Size(3, 3), 2, 2);

//     // convert to hsv image
//     cvtColor(img, hsv_image, cv::COLOR_BGR2HSV);

//     createTrackbar("MinH: ", ORIGINAL, &minH, 179, extract);
//     createTrackbar("MaxH: ", ORIGINAL, &maxH, 179, extract);
//     createTrackbar("MinS: ", ORIGINAL, &minS, 255, extract);
//     createTrackbar("MaxS: ", ORIGINAL, &maxS, 255, extract);
//     createTrackbar("MinV: ", ORIGINAL, &minV, 255, extract);
//     createTrackbar("MaxV: ", ORIGINAL, &maxV, 255, extract);
//     imshow(ORIGINAL, img);

//     extract(0, 0);

//     waitKey(0);
// }

void image_processing(Mat &img, vec_t &result, int i) {
    GaussianBlur(img, img, cv::Size(3, 3), 2, 2);
    resize(img, img, Size(w, h));

    // imshow("Image", img);
    // waitKey(0);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w * 3; j += 3) {
            result.push_back((double)img.at<uint8_t>(i, j));
            result.push_back((double)img.at<uint8_t>(i, j + 1));
            result.push_back((double)img.at<uint8_t>(i, j + 2));
        }
    }
}
