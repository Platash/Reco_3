#include "imageprocessor.h"


bool ImageProcessor::pickFace(cv::Mat image, cv::Rect2d& roi, cv::Rect2d& bestRoi) {
    cv::Rect bestFaceRoi;
    if(faceDetector.getBestFace(image, roi, bestFaceRoi)) {
        cv::Mat crop = image(bestFaceRoi);
        //if (crop.empty()) return false;
        //cv::Mat equalized = prep.equalize(crop);
        int score = qualityAssessment.getScore(crop);
        if(faces.size() < faceCount) {
            faces.push_back(Face(crop, score));
            //rectangle(image, bestFaceRoi, cv::Scalar(255, 255, 0), 2, 1);
            return true;
        } else if(score > worstScore) {
            std::sort(faces.begin(), faces.end());
            faces.at(faceCount - 1) = Face(crop, score);

            return true;
        }
        return false;
    }
    return false;
}


bool ImageProcessor::processAverageFace() {
    int i = 0;
    for(auto& face: faces) {
        cv::imwrite("/home/siobhan/UJ/Masters_stuff/results/best/img_" + to_string(i) + ".jpg", face.face);
        averageFaceCreator.getLandmarks(face);
        ++i;
    }
    averageFace = averageFaceCreator.makeAverageFace(faces);
    cv::imwrite("/home/siobhan/UJ/Masters_stuff/results/best/img_best.jpg", averageFace);

    std::cout << "finish processAverageFace" << std::endl;
    return true;
}

std::vector<Face> &ImageProcessor::getFaces() {
    return faces;
}

int ImageProcessor::getFaceCount() {
    return faces.size();
}
