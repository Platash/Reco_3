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
            cv::cvtColor(face, grey, CV_BGR2GRAY);
            faces.push_back(normalize(grey));
            labels.push_back(std::stoi(subdirname));
            write_log(std::to_string(std::stoi(subdirname)));
        }
    }
    model->train(faces, labels);
    isTrained = true;
}

int FaceRecognition::predict(cv::Mat face) {
    cv::Mat grey;
    //drawMask();
    int result;
    if(face.channels() == 3) {
        cv::cvtColor(face, grey, CV_BGR2GRAY);
        result = model->predict(normalize(grey));
    } else {
        result = model->predict(normalize(face));
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
        std::vector<cv::Mat> results;

        if(faceDetector.detectAndCropFaces(pathSrc + "/" + subdirname + "/", images)) {
            for(auto& image: images) {
                Face face(image, 0);
                averageFace.getLandmarks(face);
                averageFace.alignFace(face, image);

                //drawMask(face.face, face.landmarks.at(0), face.landmarks.at(16), face.landmarks.at(8));
                results.push_back(image.clone());

            }
            std::string path = pathDst + "/" + subdirname + "/";
            writeImages(results, path);
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
