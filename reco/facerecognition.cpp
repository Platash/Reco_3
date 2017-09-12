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
        for(auto& face: facesTemp) {
            cv::Mat grey;
            if(face.type() != CV_8UC1) {
                cv::Mat temp;
                face.convertTo(temp, CV_8UC1, 1);
                cv::cvtColor(temp, grey, CV_BGR2GRAY);
            } else {
                cv::cvtColor(face, grey, CV_BGR2GRAY);
            }
            faces.push_back(grey);
            labels.push_back(std::stoi(subdirname));
            write_log(std::to_string(std::stoi(subdirname)));
        }
    }
    model->train(faces, labels);
    isTrained = true;
}

int FaceRecognition::predict(cv::Mat face) {
    cv::Mat grey;
    cv::Mat temp;
    //drawMask();
    int result;
    if(face.type() != CV_8UC1) {
        face.convertTo(temp, CV_8UC1, 1);
        if(face.channels() == 3) {
            cv::cvtColor(temp, grey, CV_BGR2GRAY);
            result = model->predict(grey);
        } else {
            result = model->predict(temp);
        }

    } else {
        if(face.channels() == 3) {
            cv::cvtColor(face, grey, CV_BGR2GRAY);
            result = model->predict(grey);
        } else {
            result = model->predict(face);
        }
    }


    return result;
}



void FaceRecognition::prepareDatabase(std::string pathSrc, std::string pathDst) {
    prepareImages(pathSrc, pathDst);

}

void FaceRecognition::prepareImages(std::string pathSrc, std::string pathDst) {
    write_log("PrepareImages() ");
    std::vector<std::string> subdirnames ;
    int subdirCount = readSubdirNames(subdirnames, pathSrc);
    std::cout << subdirCount << std::endl;
    std::cout << pathSrc << std::endl;

    for(auto subdirname: subdirnames) {
        write_log(subdirname);
        std::vector<cv::Mat> images;

        std::vector<Face> faces;
        if(faceDetector.detectAndCropFaces(pathSrc + "/" + subdirname + "/", images)) {
            for(auto& image: images) {
                Face face(image, 0);
                averageFace.getLandmarks(face);
                faces.push_back(face);
            }
            std::string path = pathDst + "/" + subdirname + "/";
            averageFace.makeAverageFace(faces, true, path);


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
