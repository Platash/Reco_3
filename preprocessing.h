#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <iostream>   //cout

#include "common/common.h"

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

    Mat equalizeBW(const Mat& src);
    Mat equalizeColor(const Mat &src);
    Mat rescale(Mat& src);
    Mat sharpen(Mat &src);
};

#endif // PREPROCESSING_H
