#include "mytracker.h"
#include "common.h"

enum class TrackerType {

};

MyTracker::MyTracker() {

}

void MyTracker::startTracking(cv::Rect2d roi_, std::string trackerType, cv::Mat* frame_p) {
    fileNameIndex = 0;
    frameCount = 0;
    tracker = cv::Tracker::create("BOOSTING");
    roi = roi_;
    if(roi.width==0 || roi.height==0)
        return;
    tracker->init(*frame_p, roi);
}

void MyTracker::stopTracking() {

}

void MyTracker::track(cv::Mat* frame) {

    // return at the end of video
    if(frame->rows==0 || frame->cols==0)
        return;
    tracker->update(*frame, roi);
    // draw the tracked object
    cv::Mat crop = (*frame)(roi);
    if (crop.empty()) return;

    //cv::Mat equalized = prep.equalize(crop);
    //prep.detectAndCropFace(equalized);
    rectangle(*frame, roi, cv::Scalar(255, 0, 0), 2, 1);

    if(frameCount >= 20) {
        frameCount = 0;
        imwrite(FACE_RESULT_PATH + "cropped/img_"+ std::to_string(fileNameIndex) + ".jpg", crop);
        ++fileNameIndex;
    }
    ++frameCount;

}

