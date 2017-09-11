#include "preprocessing.h"

Mat Preprocessing::equalizeBW(const Mat &src) {
    Mat dst_clahe;
    Ptr<CLAHE> clahe = createCLAHE();
    clahe->setClipLimit(2);
    clahe->apply(src, dst_clahe);
    return dst_clahe;
}

Mat Preprocessing::equalizeColor(const Mat &src) {
    Mat im_gray;
    cvtColor(src, im_gray, CV_RGB2GRAY);
    Mat dst_clahe;
    Mat dst_color;

    Ptr<CLAHE> clahe = createCLAHE();
    clahe->setClipLimit(2);
    clahe->apply(im_gray, dst_clahe);
    cvtColor(dst_clahe, dst_color, CV_GRAY2RGB);
    return dst_color;
}

Mat Preprocessing::rescale(Mat& src) {
    int width = src.cols;
    int height = src.rows;

    int max_dim = (width >= height) ? width : height;
    float scale = ((float) FACE_SIZE_HE) / max_dim;
    Rect roi;
    if (width >= height)     {
        roi.width = FACE_SIZE_WE;
        roi.x = 0;
        roi.y = 0;
        roi.height = height * scale;
    } else {
        roi.y = 0;
        roi.x = 0;
        roi.height = FACE_SIZE_HE;
        roi.width = width * scale;
    }
    Mat dst;
    resize(src, dst, roi.size());
    return dst;

}
