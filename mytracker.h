#ifndef TRACKER_H
#define TRACKER_H


#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include "common.h"


class MyTracker
{
public:
    MyTracker();
    void startTracking(cv::Rect2d roi_, std::string trackerType, cv::Mat* frame);
    void stopTracking();
    void track(cv::Mat* frame);

private:
    cv::Ptr<cv::Tracker> tracker;
    cv::Rect2d roi;
    cv::Mat dst;
    int fileNameIndex;
    int frameCount;

};

#endif // TRACKER_H
