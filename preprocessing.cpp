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
    cv::Mat temp;
    if(src.type() == CV_32FC3) {
        src.convertTo(temp, CV_8UC1, 1);
        cvtColor(temp, im_gray, CV_RGB2GRAY);
    } else {
        cvtColor(src, im_gray, CV_RGB2GRAY);
    }
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

Mat Preprocessing::sharpen(cv::Mat& src) {
    cv::Mat imgLaplacian;
    cv::Mat imgResult;

    cv::Laplacian(src, imgLaplacian, CV_8UC1);
    imgLaplacian = cv::Mat::zeros(src.size(), CV_32F);
    imgLaplacian.convertTo(imgLaplacian, CV_8U);

    cv::Mat kernel = (cv::Mat_<float>(3,3) <<
              0,  1, 0,
              1, -4, 1,
              0,  1, 0);

    cv::filter2D(src, imgLaplacian, CV_32F, kernel);
    src.convertTo(src, CV_32F);
    imgResult = src - imgLaplacian;

    // convert back to 8bits gray scale
    imgResult.convertTo(imgResult, CV_8U);
    imgLaplacian.convertTo(imgLaplacian, CV_8U);
    return imgResult;
}\

