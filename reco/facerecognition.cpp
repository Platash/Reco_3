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
    std::vector<std::string> subdirnames;
    int subdirCount = readSubdirNames(subdirnames, pathSrc);
    std::cout << subdirCount << std::endl;
    std::vector<cv::Mat> images;
    for(auto subdirname: subdirnames) {
        std::cout << subdirname << std::endl;
        //images = faceDetector.detectAndCropFaces(pathSrc);

       // writeImages(images, pathDst + subdirname);
    }

}
