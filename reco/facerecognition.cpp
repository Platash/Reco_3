#include "facerecognition.h"
#include "common/common.h"
#include "facedetector.h"

void FaceRecognition::train(std::string path) {
    std::vector<std::string> subdirnames;
    int subdirCount = readSubdirNames(subdirnames, path);
    std::vector<cv::Mat> faces;
    std::vector<int> labels;
    write_log("Training model: subdir count: " + std::to_string(subdirCount));
    for(auto subdirname: subdirnames) {
        std::vector<cv::Mat> facesTemp;
        readImages(path + "/" + subdirname + "/", facesTemp);
        for(auto face: facesTemp) {
            faces.push_back(face);
            labels.push_back(std::stoi(subdirname));
        }
    }
    model->train(faces, labels);
    isTrained = true;
}

int FaceRecognition::predict(cv::Mat face) {
    return model->predict(face);
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
            for(auto& image: images) {
                Face face(image, 0);
                averageFace.getLandmarks(face);
                averageFace.alignFace(face, image);
            }
            std::string path = pathDst + "/" + subdirname + "/";
            writeImages(images, path);
        }

    }

}

void FaceRecognition::readModelFromFile(std::string path) {
    model->load(path);
    isTrained = true;
}

void FaceRecognition::saveModel(std::string path) {
    model->save(path);
}
