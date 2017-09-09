#include "facedetector.h"

FaceDetector::FaceDetector() {
    if(!face_cascade.load(FACE_CASCADE_PATH)) {
        printf("--(!)Error loading face cascade\n");
    }
}

bool FaceDetector::getBestFace(const cv::Mat& frame, cv::Rect2d& roi, cv::Rect& bestFaceRoi) {
    std::cout << "start: getBestFace " << std::endl;
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
        write_log("detect faces: " + to_string(i));
        cv::Rect intersection = roi_ & detected_faces[i];
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
        bestFaceRoi = detected_faces.at(index);
    }
    return result;

}

bool FaceDetector::detectAndCropFaces(std::string directory, std::vector<cv::Mat>& faces) {
    std::cout << "start detectAndCropFaces " << std::endl;
    std::vector<cv::Mat> images;
    readImages(directory, images);
    int i = 0;
    for(auto& image: images) {
        write_log("img number: " + std::to_string(i));
        i++;
        cv::Mat face;
        if(detectAndCropFace(image, face)) {
            cv::Mat equalized = preprocessor.equalizeColor(face);
            faces.push_back(equalized.clone());
        }
    }
    std::cout << "end: detectAndCropFaces " << "faces size: " << faces.size() << std::endl;
    return faces.size() > 0;
}

bool FaceDetector::detectAndCropFace(cv::Mat& src, cv::Mat& dst) {
    write_log("Start: detectAndCropFace ");
    std::vector<cv::Rect> detected_faces;


   // cv::Mat equalized = preprocessor.equalize(src);

    //src = equalized;
    face_cascade.detectMultiScale(src, detected_faces);
    if(detected_faces.size() != 1) {
        write_log("End false: detectAndCropFace ");
        return false;
    }

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

    dst = src(detected_faces[0]);
    write_log("End: detectAndCropFace ");
    return true;
}
