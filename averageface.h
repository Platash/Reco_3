#ifndef AVERAGEFACE_H
#define AVERAGEFACE_H

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/dnn.h>
#include <dlib/clustering.h>
#include <dlib/string.h>
#include <dlib/opencv.h>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <fstream>
#include <dirent.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <iostream>

#include "common/face.h"
#include "common/common.h"

class AverageFace {
public:
    AverageFace():isInitialized(false) {
    }

    void init(std::string path);
    void getLandmarks(Face &face);
    cv::Mat makeAverageFace(std::vector<Face>& faces);
    void alignFace(Face &face);

private:
    void similarityTransform(std::vector<cv::Point2f>& inPoints,
                             std::vector<cv::Point2f>& outPoints, cv::Mat &tform);

    void calculateDelaunayTriangles(cv::Rect rect,
                                    std::vector<cv::Point2f> &points,
                                    std::vector<std::vector<int>> &delaunayTri);

    void applyAffineTransform(cv::Mat& warpImage, cv::Mat& src,
                              std::vector<cv::Point2f> &srcTri,
                              std::vector<cv::Point2f> &dstTri);

    void warpTriangle(cv::Mat& img1, cv::Mat& img2, std::vector<cv::Point2f> t1,
                      std::vector<cv::Point2f> t2);

    void constrainPoint(cv::Point2f &p,cv::Size sz);
    void setBoundaryPoints();
    void setEyecornerPoints();
    cv::Mat alignFaceForAveraging(Face face, int faceCount);

    bool isInitialized;
    float s60 = sin(60 * M_PI / 180.0);
    float c60 = cos(60 * M_PI / 180.0);

    dlib::shape_predictor shapePredictor;

    std::vector<cv::Mat> imagesNorm;
    std::vector<cv::Point2f> eyecornerDst;
    std::vector<cv::Point2f> eyecornerSrc;
    std::vector<cv::Point2f> boundaryPts;

};

#endif // AVERAGEFACE_H
