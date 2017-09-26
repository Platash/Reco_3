#ifndef QUALITYASSESSMENT_H
#define QUALITYASSESSMENT_H

#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "common/common.h"

class QualityAssessment {

    bool measuringBlur;
    double blurCoef;
    bool measuringSize;
    double sizeCoef;
    int magic = 35000;
    double getBlurriness(cv::Mat& image);


public:
    QualityAssessment():measuringBlur(true), blurCoef(0.5), measuringSize(true),
        sizeCoef(0.5) {

    }
    double getScore(cv::Mat& image);
};

#endif // QUALITYASSESSMENT_H
