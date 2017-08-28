#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <dirent.h>     // for linux systems
#include <sys/stat.h>   // for linux systems
#include <algorithm>    // std::sort

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/opencv.hpp>

#include <QPixmap>
#include <QImage>

#define FACE_MAX_SIZE 400
#define CROP_THRESHOLD 10
#define CROP_THRESHOLDx2 20

static const std::string FACE_RESULT_PATH = "/home/siobhan/UJ/Masters_stuff/results/tracked_faces/";
static const std::string FACE_CASCADE_PATH = "/home/siobhan/UJ/Masters/Cascades/haarcascade_frontalface_alt_tree.xml";


int readFileNames(std::vector<std::string> &filenames, const std::string &directory);
void readImages(std::string& directory, std::vector<cv::Mat>& images);
void writeImages(std::vector<cv::Mat> &images, std::string path);
QPixmap mat2Pixmap(cv::Mat matImg);


#endif // COMMON_H
