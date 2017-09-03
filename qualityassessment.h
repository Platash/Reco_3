#ifndef QUALITYASSESSMENT_H
#define QUALITYASSESSMENT_H

#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
//#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include "opencv2/opencv.hpp"

#include "common/common.h"

class QualityAssessment {
    bool measuringContrast;
    double contrastCoef;
    bool measuringBlur;
    double blurCoef;
    bool measuringSize;
    double sizeCoef;

    int getContrast(cv::Mat& image);
    int getBlurriness(cv::Mat& image);

public:
    QualityAssessment():measuringBlur(true), measuringSize(true), blurCoef(0.5), sizeCoef(0.5) {

    }
    double getScore(cv::Mat& image);
};

#endif // QUALITYASSESSMENT_H
