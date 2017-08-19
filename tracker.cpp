#include "tracker.h"

Tracker::Tracker() {

}

void Tracker::startTracking() {

}

void Tracker::stopTracking() {

}

void Tracker::track(cv::Mat frame) {

    cv::Rect2d roi;
    cv::Mat dst;
    cv::Scalar scalar(255, 0, 0);
    cv::Ptr<cv::Tracker> tracker = cv::Tracker::create( "BOOSTING" );
    roi=selectROI("tracker", frame);
    //quit if ROI was not selected
    if(roi.width==0 || roi.height==0)
        return;
    tracker->init(frame, roi);
    int i = 0;
    int index = 0;
    std::string path = FACE_RESULT_PATH;
    for ( ;; ){

        // return at the end of video
        if(frame.rows==0 || frame.cols==0)
            break;
        tracker->update(frame,roi);
        // draw the tracked object
        cv::Mat crop = frame(roi);
        if (crop.empty()) continue;

        //cv::Mat equalized = prep.equalize(crop);
        //prep.detectAndCropFac/e(equalized);
        rectangle(frame, roi, scalar, 2, 1);

        if(i >= 20) {
            i = 0;
            imwrite(path + "cropped/img_"+ std::to_string(index) + ".jpg", crop);
            ++index;
        }
        ++i;

    }
}
