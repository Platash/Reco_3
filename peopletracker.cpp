#include "peopletracker.h"
#include "common/common.h"

enum class TrackerType {

};

PeopleTracker::PeopleTracker() {
    roiChanged = false;
}


void PeopleTracker::startTracking(std::string trackerType, cv::Mat* frame_p,
                              int p1_x_, int p1_y_, int p2_x_, int p2_y_) {
    fileNameIndex = 0;
    frameCount = 0;
    p1_x = p1_x_;
    p1_y = p1_y_;
    p2_x = p2_x_;
    p2_y = p2_y_;
    roi = cv::Rect2d(cv::Point(p1_x, p1_y), cv::Point(p2_x, p2_y));
    roiChanged = false;
    tracker = cv::Tracker::create(trackerType);
    tracker->init(*frame_p, roi);
}

void PeopleTracker::modifyTracking(int p1_x_, int p1_y_, int p2_x_, int p2_y_) {
    std::cout << "modify tracking" << std::endl;
    p1_x = p1_x_;
    p1_x = p1_x_;
    p1_y = p1_y_;
    p2_x = p2_x_;
    p2_y = p2_y_;
    roiChanged = true;
}

void PeopleTracker::stopTracking() {
}

cv::Rect2d& PeopleTracker::getRoi() {
    return roi;
}
void PeopleTracker::setRoi(cv::Rect2d roi_) {
    roi = roi_;
}

bool PeopleTracker::track(cv::Mat frame) {
    if(roiChanged) {
        std::cout << "roi changed" << std::endl;
        roi = cv::Rect2d(cv::Point(p1_x, p1_y), cv::Point(p2_x, p2_y));
        tracker = cv::Tracker::create(trackerType);
        tracker->init(frame, roi);
        roiChanged = false;
        if(roi.width==0 || roi.height==0)
            return false;
    }

    if(tracker->update(frame, roi)) {
        //rectangle(*frame_p, roi, cv::Scalar(255, 0, 0), 2, 1);
        return true;
    }
    return false;
}

