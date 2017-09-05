#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <dirent.h>     // for linux systems
#include <sys/stat.h>   // for linux systems
#include <algorithm>    // std::sort

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>

#include <dlib/image_processing/frontal_face_detector.h>
//#include <dlib/image_processing/render_face_detections.h>
//#include <dlib/image_processing.h>
//#include <dlib/gui_widgets.h>
//#include <dlib/image_io.h>
//#include <dlib/dnn.h>
//#include <dlib/clustering.h>
//#include <dlib/string.h>
//#include <dlib/opencv.h>

#include <QPixmap>
#include <QImage>


#define CROP_THRESHOLD 10
#define CROP_THRESHOLDx2 20
#define SOURCE_IMAGE_MAX_SIZE 800


#define CASCADE_PATH "/home/siobhan/UJ/Masters/Cascades/"
#define LANDMARKS_PREDICTOR_PATH "/home/siobhan/UJ/Masters_stuff/shape_predictor_68_face_landmarks.dat"

#define CROP_THRESHOLD 10
#define CROP_THRESHOLDx2 20

#define LOGGING

static int SMALL_FACE_SIZE = 50;
static int LANDMARKS_COUNT = 68;

static int FACE_MAX_SIZE_W = 400;
static int FACE_MAX_SIZE_H = 400;

static const std::string FACE_RESULT_PATH = "/home/siobhan/UJ/Masters_stuff/results/tracked_faces/";
static const std::string FACE_CASCADE_PATH = "/home/siobhan/UJ/Masters/Cascades/haarcascade_frontalface_alt_tree.xml";

//===================IO methods =======================
int readFileNames(std::vector<std::string> &filenames, const std::string &directory);
int readSubdirNames(std::vector<std::string> &subdirnames, const std::string &directory);
void readImages(std::string& directory, std::vector<cv::Mat>& images);
void writeImages(std::vector<cv::Mat> &images, std::string path);
void writeShapeToFile(dlib::full_object_detection shape, std::string path);
void read_csv(const std::string& filename, std::vector<cv::Mat>& images, std::vector<int>& labels, char separator = ';');
void write_log(std::string text);

//==================Image processing =================
QPixmap mat2Pixmap(cv::Mat matImg);


#endif // COMMON_H
