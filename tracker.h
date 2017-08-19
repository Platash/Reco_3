#ifndef TRACKER_H
#define TRACKER_H


#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include "common.h"


class Tracker
{
public:
    Tracker();
    void startTracking();
    void stopTracking();
    void track(cv::Mat frame);
};

#endif // TRACKER_H
