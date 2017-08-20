#include "mytracker.h"
#include "common.h"

enum class TrackerType {

};

MyTracker::MyTracker() {

}

void MyTracker::startTracking(std::string trackerType, cv::Mat* frame_p,
                              int p1_x_, int p1_y_, int p2_x_, int p2_y_) {
    fileNameIndex = 0;
    frameCount = 0;
    p1_x = p1_x_;
    p1_y = p1_y_;
    p2_x = p2_x_;
    p2_y = p2_y_;
    roi = cv::Rect2d(cv::Point(p1_x, p1_y), cv::Point(p2_x, p2_y));
    //roiChanged = true;
    tracker = cv::Tracker::create("BOOSTING");
    tracker->init(*frame_p, roi);
}

void MyTracker::modifyTracking(int p1_x_, int p1_y_, int p2_x_, int p2_y_) {
    std::cout << "modify tracking" << std::endl;
    p1_x = p1_x_;
    p1_x = p1_x_;
    p1_y = p1_y_;
    p2_x = p2_x_;
    p2_y = p2_y_;
    roiChanged = true;
}

void MyTracker::stopTracking() {

}

void MyTracker::track(cv::Mat* frame_p) {
    if(roiChanged) {
        std::cout << "roi changed" << std::endl;
        roi = cv::Rect2d(cv::Point(p1_x, p1_y), cv::Point(p2_x, p2_y));
        tracker = cv::Tracker::create("BOOSTING");
        tracker->init(*frame_p, roi);
        //tracker->init(*frame_p, roi);
        roiChanged = false;
        if(roi.width==0 || roi.height==0)
            return;
    }
    // return at the end of video
    bool success = tracker->update(*frame_p, roi);
    // draw the tracked object
    cv::Mat crop = (*frame_p)(roi);
    if (crop.empty()) return;

    //cv::Mat equalized = prep.equalize(crop);
    //prep.detectAndCropFace(equalized);
    if(success) {
        rectangle(*frame_p, roi, cv::Scalar(255, 0, 0), 2, 1);
    }


    if(frameCount >= 10) {
        frameCount = 0;
        imwrite(FACE_RESULT_PATH + "img_"+ std::to_string(fileNameIndex) + ".jpg", crop);
        ++fileNameIndex;
    }
    ++frameCount;

}

