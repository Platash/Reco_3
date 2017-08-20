#ifndef TRACKER_H
#define TRACKER_H


#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <thread>
#include <atomic>
#include <time.h>

#include "common.h"
#include "facedetector.h"


class MyTracker
{
public:
    MyTracker();
    void startTracking(std::string trackerType, cv::Mat* frame,
                       int p1_x_, int p1_y_, int p2_x_, int p2_y_);
    void stopTracking();
    void track(cv::Mat* frame);
    void modifyTracking(int p1_x_, int p1_y_, int p2_x_, int p2_y_);

private:
    cv::Ptr<cv::Tracker> tracker;
    cv::Mat dst;
    int fileNameIndex;
    int frameCount;
     cv::Rect2d roi;
     std::atomic<int> p1_x;
     std::atomic<int> p1_y;
     std::atomic<int> p2_x;
     std::atomic<int> p2_y;
     std::atomic<bool> roiChanged;
     FaceDetector faceDetector;
};

#endif // TRACKER_H
