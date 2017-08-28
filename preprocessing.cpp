#include "preprocessing.h"

Mat Preprocessing::equalize(const Mat &src) {
    string source_window = "Source image";
    string equalized_window = "Equalized Image";
    string equalized_window_cl = "Equalized Image Clahe";

    Mat im_gray;
    cvtColor(src,im_gray,CV_RGB2GRAY);

    Mat dst_eq;
    Mat dst_clahe;
    equalizeHist(im_gray, dst_eq);

    Ptr<CLAHE> clahe = createCLAHE();
    clahe->setClipLimit(2);
    clahe->apply(im_gray,dst_clahe);

    return dst_clahe;
}

Mat Preprocessing::rescale(Mat& src) {
    int width = src.cols;
    int height = src.rows;

    int max_dim = (width >= height) ? width : height;
    float scale = ((float) FACE_MAX_SIZE) / max_dim;
    Rect roi;
    if (width >= height)     {
        roi.width = FACE_MAX_SIZE;
        roi.x = 0;
        roi.y = 0;
        roi.height = height * scale;
    } else {
        roi.y = 0;
        roi.x = 0;
        roi.height = FACE_MAX_SIZE;
        roi.width = width * scale;
    }
    Mat dst;
    resize(src, dst, roi.size());
    return dst;

}
