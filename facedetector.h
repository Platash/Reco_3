#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

//#include <opencv2/core/utility.hpp>
//#include <opencv2/tracking.hpp>
//#include <opencv2/videoio.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>

#include "common/common.h"
#include "preprocessing.h"

class FaceDetector
{
public:
    FaceDetector();

    bool getBestFace(const Mat &frame, cv::Rect2d& roi, cv::Rect &bestFaceRoi);

    bool detectAndCropFaces(std::string directory, std::vector<cv::Mat> & faces);
    bool detectAndCropFace(Mat &src, cv::Mat& dst);
    bool cropFace(cv::Mat &src, cv::Mat dst);
private:
    cv::CascadeClassifier face_cascade;
    Preprocessing preprocessor;

};

#endif // FACEDETECTOR_H
