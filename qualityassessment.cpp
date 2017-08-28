#include "qualityassessment.h"

int QualityAssessment::getContrast(cv::Mat& image) {

}

int QualityAssessment::getBlurriness(cv::Mat& image) {
    cv::Mat Gx;
    cv::Mat Gy;
    cv::Sobel(image, Gx, CV_32F, 1, 0);
    cv::Sobel(image, Gy, CV_32F, 0, 1);
    double normGx = norm(Gx);
    double normGy = norm(Gy);
    double sumSq = normGx * normGx + normGy * normGy;
    return static_cast<float>(1. / (sumSq / image.size().area() + 1e-6));
}

int QualityAssessment::getQuality(cv::Mat& image) {
    int result = 0;
    if(measuringBlur) {
        result += blurCoef * getBlurriness(image);
    }
    if(measuringContrast) {
        result += contrastCoef * getContrast(image);
    }
    return result;
}
