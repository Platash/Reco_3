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

class Preprocessing
{

private:
    int readFileNames(vector<string> &filenames, const string &directory);
    //Mat detectAndCrop(Mat& src);

    const string& directory;
    vector<Mat> images;
    vector<Mat>* faces;

    CascadeClassifier face_cascade;

public:
    Preprocessing();
    Preprocessing(const string& directory_, vector<Mat>* faces_):faces(faces_),
                                                                 directory(directory_) {
        if(!face_cascade.load(FACE_CASCADE_PATH)) {
            printf("--(!)Error loading face cascade\n");  };
    }

    void readFiles();
    Mat equalize(const Mat& src);
    void detectAndCropFace(const Mat& frame);
    void detectAndCropFaces();

    void prepareFaces();
    Mat rescale(Mat& src);
    void writeImages(vector<Mat>& images) const;
    Mat alignFace(Mat& face) const;




};

#endif // PREPROCESSING_H
