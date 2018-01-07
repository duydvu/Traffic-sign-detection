#include <stdlib.h>
#include <time.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include "tiny_dnn/tiny_dnn.h"
using namespace tiny_dnn;
using namespace tiny_dnn::activation;
using namespace tiny_dnn::layers;
using namespace std;
using namespace cv;

int w = 250,
    h = 250,
    W = w - 50,
    H = h - 50;

void image_processing(Mat &img, vec_t &result, vec_t &roi, int &i/*, network<sequential> &net*/) {
    srand(time(NULL));
    const char *Ori = "Original";
    const char *Res = "Result";

    /* Insert img to final_ */

    // crop sign from img
    img(Rect(roi[0], roi[1], roi[2] - roi[0] - 1, roi[3] - roi[1] - 1)).copyTo(img);

    // randomly resize img
    int img_w = img.cols * (90 + rand() % 20) / 100,
        img_h = img.rows * (90 + rand() % 20) / 100;
    resize(img, img, Size(img_w, img_h));
    // check if img is too big
    if(img.cols > W)
        resize(img, img, Size(W, img.rows * W / img.cols));
    if(img.rows > H)
        resize(img, img, Size(img.cols * H / img.rows, H));
    // create random background color
    int r = rand() % 256,
        g = rand() % 256,
        b = rand() % 256;
    // random position
    int x = rand() % (w - img.cols),
        y = rand() % (h - img.rows);
    // change roi position and size
    roi[0] = x;
    roi[1] = y;
    roi[2] = x + img.cols;
    roi[3] = y + img.rows;
    // initialize output image with random background
    Mat final_(w, h, CV_8UC3, Scalar(r, g, b));
    // copy img to final_
    img.copyTo(final_(Rect(x, y, img.cols, img.rows)));

    // blur to reduce noise
    GaussianBlur(final_, final_, Size(9, 9), 2, 2);

    // show images
    // imshow(Ori, img);
    // imshow(Res, final_);
    // cout << "#" << i << ". Number of channels: " << final_.channels() << endl;
    // cout << r << " " << g << " " << b << endl;
    // waitKey(0);

    // save to result
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w * 3; j += 3) {
            result.push_back((double)final_.at<uint8_t>(i, j));
            result.push_back((double)final_.at<uint8_t>(i, j + 1));
            result.push_back((double)final_.at<uint8_t>(i, j + 2));
        }
    }
}

// const char *ORIGINAL = "Original";
// const char *RED = "Red";
// const char *BLUE = "Blue";
// const char *WHITE = "White";
// const char *CONTOURS = "Contours";
// // namedWindow(ORIGINAL, WINDOW_AUTOSIZE);
// // namedWindow(RED, WINDOW_AUTOSIZE);
// // namedWindow(BLUE, WINDOW_AUTOSIZE);
// // namedWindow(WHITE, WINDOW_AUTOSIZE);
// // namedWindow(CONTOURS, WINDOW_AUTOSIZE);

// resize(img, img, Size(800, 480));

// // blur to reduce noise
// GaussianBlur(img, img, cv::Size(3, 3), 2, 2);

// // calculate brightness
// Mat gray;
// cvtColor(img, gray, cv::COLOR_RGB2GRAY);
// Scalar summ = sum(gray);
// double brightness = summ[0] / ((pow(2, 8) - 1) * img.rows * img.cols) * 2;

// // convert to hsv image
// Mat hsv_image;
// cvtColor(img, hsv_image, cv::COLOR_BGR2HSV);

// // if the image is to dark, lighten it up
// if (brightness <= 1)
// {
//     for (int i = 0; i < img.rows; i++)
//     {
//         for (int j = 0; j < img.cols * 3; j += 3)
//         {
//             hsv_image.at<uint8_t>(i, j + 2) = (uint8_t)min((double)hsv_image.at<uint8_t>(i, j + 2) * 1 / brightness, (double)255);
//         }
//     }
// }

// // Extract red regions
// Mat lower_red_hue_range;
// Mat upper_red_hue_range;
// Mat red_hue_range;
// inRange(hsv_image, cv::Scalar(0, 50, 50), cv::Scalar(20, 255, 255), lower_red_hue_range);
// inRange(hsv_image, cv::Scalar(150, 50, 50), cv::Scalar(179, 255, 255), upper_red_hue_range);
// addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_range);

// // Extract blue regions
// Mat blue_hue_range;
// inRange(hsv_image, cv::Scalar(70, 100, 50), cv::Scalar(140, 255, 255), blue_hue_range);

// // Extract white regions
// Mat white_hue_range;
// inRange(hsv_image, cv::Scalar(0, 0, 100), cv::Scalar(180, 50, 255), white_hue_range);

// // Detect edges
// Mat edge;
// Canny(gray, edge, 50, 50 * 3, 3);

// Mat final_;
// addWeighted(red_hue_range, 1.0, blue_hue_range, 1.0, 0.0, final_);
// addWeighted(white_hue_range, 1.0, final_, 1.0, 0.0, final_);
// final_ = final_ & (~edge);

// // detect circle
// // vector<vector<Point>> contours;
// // vector<Point> sign;     // the traffic sign to be detected
// // Mat bimage = final_ >= 127;

// // findContours(bimage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

// // Mat cimage = Mat::zeros(bimage.size(), CV_8UC3);

// // sign = contours[0];
// // for (size_t i = 0; i < contours.size(); i++) {
// //     if (contours[i].size() < 6)
// //         continue;

// //     // find ellipse from contours
// //     Mat pointsf;
// //     Mat(contours[i]).convertTo(pointsf, CV_32F);
// //     RotatedRect box = fitEllipse(pointsf);
// //     // if ellipse is too long, omit it
// //     if (MAX(box.size.width, box.size.height) > MIN(box.size.width, box.size.height) * 2)
// //         continue;

// //     ellipse(img, box, Scalar(0, 0, 255), 1, CV_AA);
// //     ellipse(img, box.center, box.size * 0.5f, box.angle, 0, 360, Scalar(0, 255, 255), 1, CV_AA);
// //     Point2f vtx[4];
// //     box.points(vtx);
// //     for (int j = 0; j < 4; j++)
// //         line(img, vtx[j], vtx[(j + 1) % 4], Scalar(0, 255, 0), 1, CV_AA);

// //     // find maximum ellipse
// //     sign = arcLength(contours[i], false) > arcLength(sign, false) ? contours[i] : sign;
// //     drawContours(cimage, contours, (int)i, Scalar::all(255), 1, 8);
// // }
// // Mat pointsf;
// // Mat(sign).convertTo(pointsf, CV_32F);
// // RotatedRect box = fitEllipse(pointsf);
// // // draw ellipse on original image
// // ellipse(img, box, Scalar(0, 0, 255), 1, CV_AA);
// // ellipse(img, box.center, box.size * 0.5f, box.angle, 0, 360, Scalar(0, 255, 255), 1, CV_AA);
// // // draw rectangle bouding the ellipse on original image
// // Point2f vtx[4];
// // box.points(vtx);
// // for (int j = 0; j < 4; j++)
// //     line(img, vtx[j], vtx[(j + 1) % 4], Scalar(0, 255, 0), 1, CV_AA);

// // detect circles
// vector<Vec3f> circles;
// HoughCircles(red_hue_range, circles, CV_HOUGH_GRADIENT, 1, gray.rows / 4, 200, 20, 0, 75);
// cout << "Number of circles: " << circles.size() << endl;
// for (size_t i = 0; i < circles.size(); i++)
// {
//     Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
//     int radius = cvRound(circles[i][2]);
//     // circle center
//     circle(img, center, 3, Scalar(0, 255, 0), -1, 8, 0);
//     // circle outline
//     circle(img, center, radius, Scalar(0, 0, 255), 3, 8, 0);

//     int minX = center.x - radius >= 0 ? center.x : 0;
//     int minY = center.y - radius >= 0 ? center.y : 0;
//     int maxX = center.x + radius <= img.cols ? center.x + radius : img.cols;
//     int maxY = center.y + radius <= img.rows ? center.y + radius : img.rows;

//     Rect ROI(minX, minY, maxX - minX, maxY - minY);
//     Mat red, blue, white, ed;
//     red_hue_range(ROI).copyTo(red);
//     blue_hue_range(ROI).copyTo(blue);
//     white_hue_range(ROI).copyTo(white);
//     edge(ROI).copyTo(ed);
//     resize(red, red, Size(w, h));
//     resize(blue, blue, Size(w, h));
//     resize(white, white, Size(w, h));
//     resize(ed, ed, Size(w, h));

//     vec_t extracted_traffic_sign;
//     for (int i = 0; i < h; i++)
//     {
//         for (int j = 0; j < w; j++)
//         {
//             extracted_traffic_sign.push_back((double)red.at<uint8_t>(i, j));
//             extracted_traffic_sign.push_back((double)blue.at<uint8_t>(i, j));
//             extracted_traffic_sign.push_back((double)white.at<uint8_t>(i, j));
//             extracted_traffic_sign.push_back((double)ed.at<uint8_t>(i, j));
//         }
//     }
//     label_t y_label = net.predict_label(extracted_traffic_sign) + 1;
//     putText(img, to_string(y_label), center, FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255, 0, 0), 1, CV_AA);
// }

// // resize(red_hue_range, red_hue_range, Size(w, h));
// // resize(blue_hue_range, blue_hue_range, Size(w, h));
// // resize(white_hue_range, white_hue_range, Size(w, h));
// // resize(edge, edge, Size(w, h));

// // resize(final_, final_, Size(w, h));

// // show images
// imshow(ORIGINAL, img);
// imshow(RED, red_hue_range);
// imshow(BLUE, blue_hue_range);
// imshow(WHITE, white_hue_range);
// imshow(CONTOURS, edge);
// imshow("Final", final_);
// // imshow("Cimage", cimage);
// cout << "#" << ++i << ". Number of channels: " << final_.channels() << endl;
// cout << "Brightness: " << brightness << endl;
// waitKey(0);

// // for (int i = 0; i < h; i++) {
// //     for (int j = 0; j < w; j++) {
// //         result.push_back((double)red_hue_range.at<uint8_t>(i, j));
// //         result.push_back((double)blue_hue_range.at<uint8_t>(i, j));
// //         result.push_back((double)white_hue_range.at<uint8_t>(i, j));
// //         result.push_back((double)edge.at<uint8_t>(i, j));
// //     }
// // }