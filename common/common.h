#ifndef COMMON_H
#define COMMON_H

#include <iostream>

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>

#include <dlib/image_processing/frontal_face_detector.h>

#include <QPixmap>
#include <QImage>

#include "face.h"


#define CROP_THRESHOLD 10
#define CROP_THRESHOLDx2 20
#define SOURCE_IMAGE_MAX_SIZE 800


#define CASCADE_PATH "/home/siobhan/UJ/Masters/Cascades/"
#define LANDMARKS_PREDICTOR_PATH "/home/siobhan/UJ/Masters_stuff/shape_predictor_68_face_landmarks.dat"

#define CROP_THRESHOLD 10
#define CROP_THRESHOLDx2 20

#define LOGGING

#define SMALL_FACE_SIZE 50
#define LANDMARKS_COUNT 68

#define FACE_SIZE_W 220
#define FACE_SIZE_H 220

#define FACE_SIZE_WE 440
#define FACE_SIZE_HE 440


static const std::string FACE_RESULT_PATH = "/home/siobhan/UJ/Masters_stuff/results/tracked_faces/";
static const std::string FACE_CASCADE_PATH = "/home/siobhan/UJ/Masters/Cascades/haarcascade_frontalface_alt_tree.xml";

//===================IO methods =======================
//namespace com {


int readFileNames(std::vector<std::string> &filenames, const std::string &directory);
int readSubdirNames(std::vector<std::string> &subdirnames, const std::string &directory);
void readImages(std::string directory, std::vector<cv::Mat>& images);
void writeImage(cv::Mat image, std::string path, std::string name);
void writeImages(std::vector<cv::Mat> &images, std::string path, std::string name);
void writeImages(std::vector<Face> &faces, std::string path, std::string name);
void writeShapeToFile(dlib::full_object_detection shape, std::string path);
void read_csv(const std::string& filename, std::vector<cv::Mat>& images, std::vector<int>& labels, char separator = ';');
void write_log(std::string text);

//==================Image processing =================
cv::Mat normalize(cv::InputArray _src);
QPixmap mat2Pixmap(cv::Mat matImg);
QImage Mat2QImage(cv::Mat const& src);
QImage cvMat2qImage(cv::Mat mat);
void drawMask(cv::Mat &src, cv::Point2f left, cv::Point2f right, cv::Point2f down);
bool applyBlurredMask(cv::Mat src, cv::Mat mask, cv::Mat &dst, double sigma, double maskRadius);
cv::Mat cropFace(cv::Mat img);


//} //namespace com
#endif // COMMON_H
