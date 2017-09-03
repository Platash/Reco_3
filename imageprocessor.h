#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QWidget>
#include <QResource>
#include <QIcon>
#include <QPixmap>

//#include <opencv2/core/utility.hpp>
//#include <opencv2/tracking.hpp>
//#include <opencv2/videoio.hpp>
//#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include "opencv2/opencv.hpp"

//#include <thread>
#include <atomic>
//#include <time.h>
#include <algorithm>

#include "peopletracker.h"
#include "facedetector.h"
#include "reco/facerecognition.h"
#include "qualityassessment.h"
#include "common/common.h"
#include "common/face.h"
#include "preprocessing.h"
#include "averageface.h"

class ImageProcessor {
public:
    ImageProcessor():faceCount(10), worstScore(0) {

    }

    ~ImageProcessor() {
    }

    bool pickFace(cv::Mat image, Rect2d &roi, Rect2d &bestRoi);
    bool processAverageFace();
    std::vector<Face>& getFaces();
    int getFaceCount();

    std::atomic<int> faceCount;
    cv::Mat averageFace;

private:

    int worstScore;
    FaceDetector faceDetector;
    FaceRecognition faceRecognition;
    QualityAssessment qualityAssessment;

    std::vector<Face> faces;
    Preprocessing prep;
    AverageFace averageFaceCreator;

};

#endif // IMAGEPROCESSOR_H
