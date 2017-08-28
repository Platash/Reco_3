#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include "constants.h"

#include <dirent.h> // for linux systems
#include <sys/stat.h> // for linux systems
#include <algorithm>    // std::sort
#include <iostream>   //cout

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/opencv.hpp>

#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

class Preprocessing {

private:
    const string directory;
    vector<Mat> images;

public:
    Preprocessing()=default;
    Preprocessing(const string directory_):directory(directory_) {

    }

    Mat equalize(const Mat& src);
    Mat rescale(Mat& src);
    //Mat alignFace(Mat& face) const;




};

#endif // PREPROCESSING_H
