#include "imageprocessor.h"


bool ImageProcessor::pickFace(cv::Mat image, cv::Rect2d roi) {
    cv::Rect bestFaceRoi;
    if(faceDetector.getBestFace(&image, roi, bestFaceRoi)) {
        cv::Mat crop = image(bestFaceRoi);
        if (crop.empty()) return;
        cv::Mat equalized = prep.equalize(crop);
        int score = qualityAssessment.getScore(equalized);
        if(faces.size() < faceCount) {
            faces.push_back(Face(equalized, score));
            return true;
        } else if(score > worstScore) {
            std::sort(faces.begin(), faces.end());
            faces.at(faceCount - 1) = Face(equalized, score);
            return true;
        }
        return false;
    }

}



void ImageProcessor::processAverageFace() {
    for(auto face: faces) {
        averageFaceCreator.getLandmarks(face);
    }

}

std::vector<Face> ImageProcessor::getFaces() {
    return faces;
}
