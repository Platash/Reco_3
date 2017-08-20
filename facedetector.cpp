#include "facedetector.h"

FaceDetector::FaceDetector() {
    if(!face_cascade.load(FACE_CASCADE_PATH)) {
        printf("--(!)Error loading face cascade\n");
    }
}

bool FaceDetector::getBestFace(cv::Mat* frame_p, cv::Rect2d roi, cv::Rect* bestFaceRoi) {
    std::vector<cv::Rect> detected_faces;
    cv::Rect roi_(roi);
    bool result = false;
    face_cascade.detectMultiScale(*frame_p, detected_faces);
    if(detected_faces.size() == 0) {
        return false;
    }
    for(int i = 0; i < detected_faces.size(); ++i) {
        cv::Rect intersection = roi_ & detected_faces[i];
        rectangle(*frame_p, intersection, cv::Scalar(255, 255, 0), 2, 1);
        //std::cout << "Intersection: " << intersection.area() << std::endl;
        if(intersection.area() == 0) {
            continue;
        }

    }

    return result;

}
