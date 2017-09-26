#include "facerecognition.h"
#include "common/common.h"
#include "facedetector.h"

#include <opencv2/face/facerec.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

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

void FaceRecognition::readModelFromFile(std::string path) {
    model->load(path);
    isTrained = true;
}

void FaceRecognition::saveModel(std::string path) {
    model->save(path);
}
