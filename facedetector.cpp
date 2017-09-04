#include "facedetector.h"

FaceDetector::FaceDetector() {
    if(!face_cascade.load(FACE_CASCADE_PATH)) {
        printf("--(!)Error loading face cascade\n");
    }
}

bool FaceDetector::getBestFace(cv::Mat& frame, cv::Rect2d& roi, cv::Rect& bestFaceRoi) {
    std::vector<cv::Rect> detected_faces;
    cv::Rect roi_(roi);
    bool result = false;
    int index = 0;
    int areaMax = 0;
    int areaTemp = 0;
    face_cascade.detectMultiScale(frame, detected_faces);
    if(detected_faces.size() == 0) {
        return false;
    }
    for(int i = 0; i < detected_faces.size(); ++i) {
        cv::Rect intersection = roi_ & detected_faces[i];
        //rectangle(*frame_p, intersection, cv::Scalar(255, 255, 0), 2, 1);
        areaTemp = intersection.area();
        if(areaTemp <= 0) {
            continue;
        }
        if(areaTemp > areaMax) {
            areaMax = areaTemp;
            index = i;
            result = true;
        }
    }
    if(result) {
        bestFaceRoi = detected_faces[index];
    }
    return result;

}

bool FaceDetector::detectAndCropFaces(std::string directory, std::vector<cv::Mat>& faces) {
    std::cout << "start detectAndCropFaces " << std::endl;
    std::vector<cv::Mat> images;
    readImages(directory, images);
    std::cout << images.size() << std::endl;
    for(size_t i = 0; i < images.size(); ++i) {
        cv::Mat image;

        if(detectAndCropFace(images[i], image)) {
            faces.push_back(image);
        }

    }
    std::cout << "end: detectAndCropFaces " << "faces size: " << faces.size() << std::endl;
    return faces.size() > 0;
}

bool FaceDetector::detectAndCropFace(const cv::Mat& src, cv::Mat& dst) {
    std::cout << "Start: detectAndCropFace " << std::endl;
    std::vector<cv::Rect> detected_faces;
    cv::Mat frame_gray;
    cvtColor(src, frame_gray, cv::COLOR_BGR2GRAY);

    face_cascade.detectMultiScale(src, detected_faces);
    if(detected_faces.size() != 1) {
        std::cout << "End false: detectAndCropFace " << std::endl;
        return false;
    }

    std::cout << "continue detectAndCropFace " << std::endl;
    int magic = 20;
    int magic2 = 40;

    int a = src.cols - (detected_faces[0].x + detected_faces[0].width + 40);
    int b = detected_faces[0].x - 20;
    int c = src.rows - (detected_faces[0].y + detected_faces[0].height + 40);
    int d = detected_faces[0].y - 20;

    if(a >=0 && b >= 0) {
        detected_faces[0].x -= 20;
        detected_faces[0].width += 40;
    } else {
        int e = std::min(a, b);
        int f = 20 + e;
        detected_faces[0].x -= f;
        detected_faces[0].width += 2 * f;
    }

    if(c >=0 && d >= 0) {
        detected_faces[0].y -= 20;
        detected_faces[0].height += 40;
    } else {
        int e = std::min(c, d);
        int f = 20 + e;
        detected_faces[0].y -= f;
        detected_faces[0].height += 2 * f;
    }



    std::cout << "continue detectAndCropFace3 " << std::endl;

    dst = src(detected_faces[0]);
    std::cout << "End: detectAndCropFace " << std::endl;
    return true;
}
