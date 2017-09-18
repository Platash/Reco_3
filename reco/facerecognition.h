#ifndef FACERECOGNITION_H
#define FACERECOGNITION_H

#include <opencv2/core/utility.hpp>
#include <opencv2/core.hpp>
#include <opencv2/face.hpp>
//#include <opencv2/face/facerec.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/objdetect.hpp>

#include "facedetector.h"
#include "common/face.h"
#include "../averageface.h"

class FaceRecognition
{
public:
    FaceRecognition() {

    }

    void train(std::string path);
    int predict(Mat face);
    void readModelFromFile(std::string path);
    void saveModel(std::string path);

    bool isTrained = false;
private:
    int vectorsCount = 200;
    double threshold = 5.0;
    cv::Ptr<cv::face::FaceRecognizer> model = cv::face::createEigenFaceRecognizer(vectorsCount);
    FaceDetector faceDetector;
    AverageFace averageFace;
};

#endif // FACERECOGNITION_H
