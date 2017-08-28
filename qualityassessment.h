#ifndef QUALITYASSESSMENT_H
#define QUALITYASSESSMENT_H

#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
//#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include "opencv2/opencv.hpp"

class QualityAssessment {
    bool measuringContrast;
    int contrastCoef;
    bool measuringBlur;
    int blurCoef;

    int getContrast(cv::Mat& image);
    int getBlurriness(cv::Mat& image);
public:
    QualityAssessment():measuringBlur(true), blurCoef(1) {

    }
    int getScore(cv::Mat& image);
};

#endif // QUALITYASSESSMENT_H
