#include "facerecognition.h"
#include "common/common.h"
#include "facedetector.h"

void FaceRecognition::train(std::string path) {

}

int FaceRecognition::predict() {

}

void FaceRecognition::prepareDatabase(std::string pathSrc, std::string pathDst) {
    prepareImages(pathSrc, pathDst);

}

void FaceRecognition::prepareImages(std::string pathSrc, std::string pathDst) {
    std::vector<std::string> subdirnames ;
    int subdirCount = readSubdirNames(subdirnames, pathSrc);
    std::cout << subdirCount << std::endl;
    std::cout << pathSrc << std::endl;

    for(auto subdirname: subdirnames) {
        std::cout << subdirname << std::endl;
        std::vector<cv::Mat> images;
        if(faceDetector.detectAndCropFaces(pathSrc + "/" + subdirname + "/", images)) {
            std::string path = pathDst + "/" + subdirname + "/";
            writeImages(images, path);
        }

    }

}
