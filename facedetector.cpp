#include "facedetector.h"

FaceDetector::FaceDetector() {
    if(!face_cascade.load(FACE_CASCADE_PATH)) {
        printf("--(!)Error loading face cascade\n");
    }
}

bool FaceDetector::getBestFace(cv::Mat& frame, cv::Rect2d& roi, cv::Rect& bestFaceRoi) {
    std::vector<cv::Rect> detected_faces;
    cv::Rect roi_(roi);
    bool result = false;
    int index = 0;
    int areaMax = 0;
    int areaTemp = 0;
    face_cascade.detectMultiScale(frame, detected_faces);
    if(detected_faces.size() == 0) {
        return false;
    }
    for(int i = 0; i < detected_faces.size(); ++i) {
        cv::Rect intersection = roi_ & detected_faces[i];
        //rectangle(*frame_p, intersection, cv::Scalar(255, 255, 0), 2, 1);
        areaTemp = intersection.area();
        if(areaTemp <= 0) {
            continue;
        }
        if(areaTemp > areaMax) {
            areaMax = areaTemp;
            index = i;
            result = true;
        }
    }
    if(result) {
        bestFaceRoi = detected_faces[index];
    }
    return result;

}

void FaceDetector::prepareFaces(std::string directory) {
    std::vector<cv::Mat> images;
    readImages(directory, images);
    for(size_t i = 0; i < images.size(); ++i) {
        cv::Mat image;
        detectAndCropFace(images[i], image);
    }
}

bool FaceDetector::detectAndCropFace(const cv::Mat& src, cv::Mat& dst) {
    std::vector<cv::Rect> detected_faces;
    //Mat frame_gray;
    //cvtColor(src, frame_gray, COLOR_BGR2GRAY);

    face_cascade.detectMultiScale(src, detected_faces);
    if(detected_faces.size() != 1)
        return false;

    int x_left = detected_faces[0].x - CROP_THRESHOLD >= 0 ? detected_faces[0].x - CROP_THRESHOLD : 0;
    int x_right = detected_faces[0].size().width + CROP_THRESHOLDx2 < src.size().width ?
                detected_faces[0].width + CROP_THRESHOLDx2 :
            src.size().width;
    int y_up = detected_faces[0].y - CROP_THRESHOLD >= 0 ? detected_faces[0].y - CROP_THRESHOLD : 0;
    int y_down = detected_faces[0].size().height + CROP_THRESHOLDx2 < src.size().height ?
                detected_faces[0].size().height + CROP_THRESHOLDx2 :
            src.size().height;

    cv::Rect rect(x_left, y_up, x_right, y_down);
    dst = src(rect);
    return true;
}
