#include "facedetector.h"

FaceDetector::FaceDetector() {
    if(!face_cascade.load(FACE_CASCADE_PATH)) {
        printf("--(!)Error loading face cascade\n");
    }
}

bool FaceDetector::getBestFace(cv::Mat frame, cv::Rect2d roi, cv::Rect bestFaceRoi) {
    std::vector<cv::Rect> detected_faces;

    bool result = false;
    face_cascade.detectMultiScale(frame, detected_faces);
    if(detected_faces.size() == 0) {
        return false;
    }
    for(int i = 0; i < detected_faces.size(); ++i) {
        cv::Rect intersection = roi & detected_faces[i];
        std::cout << "Intersection: " << intersection.area() << std::endl;
        if(intersection.area() == 0) {
            continue;
        }

    }

    return result;

}
