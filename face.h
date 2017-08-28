#ifndef FACE_H
#define FACE_H

#include <opencv2/core/utility.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include "opencv2/opencv.hpp"

class Face {
public:
    Face() {

    }

    int score;
    cv::Mat face;

};

#endif // FACE_H
