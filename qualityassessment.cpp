#include "qualityassessment.h"

double QualityAssessment::getContrast(cv::Mat& image) {

}

double QualityAssessment::getBlurriness(cv::Mat& image) {
    cv::Mat Gx;
    cv::Mat Gy;
    cv::Sobel(image, Gx, CV_32F, 1, 0);
    cv::Sobel(image, Gy, CV_32F, 0, 1);
    double normGx = cv::norm(Gx);
    double normGy = cv::norm(Gy);
    write_log("norm: " + std::to_string( normGx));
    write_log("norm: " + std::to_string(normGy));
    double sumSq = normGx * normGx + normGy * normGy;
    write_log("Image size area: " + std::to_string(image.size().area()));
    return (std::sqrt(sumSq) / magic);
}

double QualityAssessment::getScore(cv::Mat& image) {
    double result = 0;
    if(measuringBlur) {

        result += blurCoef * getBlurriness(image);
        write_log("blur score = " + std::to_string(result));
    }
    if(measuringContrast) {
        result += contrastCoef * getContrast(image);
    }
    if(measuringSize) {
        double sizeScore = sizeCoef * ((double)(image.rows) / FACE_SIZE_WE);
        write_log("size score: " + std::to_string(sizeScore));
        result += sizeScore;
    }
    return result;
}

