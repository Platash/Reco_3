#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/opencv.hpp"

#include "common.h"

class FaceDetector
{
public:
    FaceDetector();

    bool getBestFace(cv::Mat* frame, cv::Rect2d roi, cv::Rect* bestFaceRoi);

private:
    cv::CascadeClassifier face_cascade;
};

#endif // FACEDETECTOR_H
