#ifndef LANDMARKDETECTION_H
#define LANDMARKDETECTION_H

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/dnn.h>
#include <dlib/clustering.h>
#include <dlib/string.h>
#include <dlib/opencv.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <fstream>
#include <iostream>
#include <vector>

class LandmarkDetection {
public:
    LandmarkDetection() {

    }

    void init(std::string path);

    dlib::full_object_detection getLandmarks(cv::Mat cvMatImage, std::string path);

private:
    dlib::shape_predictor shapePredictor;

};

#endif // LANDMARKDETECTION_H
