#include "landmarkdetection.h"


void LandmarkDetection::init(std::string path) {
    dlib::deserialize(path) >> shapePredictor;
}

dlib::full_object_detection LandmarkDetection::LandmarkDetection::getLandmarks(cv::Mat cvMatImage, std::string path) {

    //array2d<rgb_pixel> img;
    dlib::array2d<dlib::bgr_pixel> dlibImage;

    dlib::assign_image(dlibImage, dlib::cv_image<dlib::bgr_pixel>(cvMatImage));
    dlib::rectangle rec(dlibImage.nc(), dlibImage.nr());

    dlib::full_object_detection shape = shapePredictor(dlibImage, rec);

    //writeToFile(shape, path);
    return shape;
}
