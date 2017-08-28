#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QWidget>
#include <QResource>
#include <QIcon>
#include <QPixmap>

#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/opencv.hpp"

#include <thread>
#include <atomic>
#include <time.h>
#include <algorithm>

#include "mytracker.h"
#include "facedetector.h"
#include "facerecognition.h"
#include "qualityassessment.h"
#include "common.h"
#include "face.h"
#include "preprocessing.h"

class ImageProcessor {
public:
    ImageProcessor():faceCount(10), worstScore(0) {
    }

    ~ImageProcessor() {
    }

    void processImage(cv::Mat image, Rect2d roi);

    std::atomic<int> faceCount;

private:

    int worstScore;
    FaceDetector faceDetector;
    FaceRecognition faceRecognition;
    QualityAssessment qualityAssessment;
    std::vector<Face> faces;
    Preprocessing prep;
};

#endif // IMAGEPROCESSOR_H
