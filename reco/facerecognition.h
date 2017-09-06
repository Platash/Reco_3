#ifndef FACERECOGNITION_H
#define FACERECOGNITION_H

#include <opencv2/core/utility.hpp>
#include <opencv2/core.hpp>
#include <opencv2/face.hpp>
#include <opencv2/face/facerec.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/xfeatures2d.hpp>

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
    void prepareImages(std::string pathSrc, std::string pathDst);
    void readModelFromFile();
    void saveModel();
    void prepareDatabase(std::string pathSrc, std::string pathDst);
private:
    int vectorsCount = 10;
    double threshold = 10.0;
    cv::Ptr<cv::face::FaceRecognizer> model = cv::face::createEigenFaceRecognizer(vectorsCount, threshold);
    FaceDetector faceDetector;
    AverageFace averageFace;
};

#endif // FACERECOGNITION_H
