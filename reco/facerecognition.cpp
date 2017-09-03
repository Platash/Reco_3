#include "facerecognition.h"
#include "common/common.h"
#include "facedetector.h"

void FaceRecognition::train(std::string path) {

}

int FaceRecognition::predict() {

}

void FaceRecognition::prepareDatabase(std::string path) {

}

void FaceRecognition::prepareImages(std::string pathSrc, std::string pathDst) {
    std::vector<cv::Mat> images = faceDetector.detectAndCropFaces(path);

    writeImages(images, path);
}
