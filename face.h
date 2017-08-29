#ifndef FACE_H
#define FACE_H

#include <opencv2/core/utility.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include "opencv2/opencv.hpp"

class Face {
public:
    Face(cv::Mat face_, int score_):face(face_), score(score_) {
    }

    inline bool operator <(const Face& that) {
        return score < that.score;
    }

    inline bool operator >(const Face& that) {
        return score > that.score;
    }

    cv::Mat face;
    int score;
    std::vector<cv::Point2f> landmarks;

};

#endif // FACE_H
