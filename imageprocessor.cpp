#include "imageprocessor.h"


void ImageProcessor::pickFace(Mat image, cv::Rect2d roi) {
     cv::Rect bestFaceRoi;
     if(faceDetector.getBestFace(&image, roi, bestFaceRoi)) {
         cv::Mat crop = image(bestFaceRoi);
         if (crop.empty()) return;
         cv::Mat equalized = prep.equalize(crop);
         int score = qualityAssessment.getScore(equalized);
         if(faces.size() < faceCount) {
             faces.push_back(Face(equalized, score));
         } else if(score > worstScore) {
            std::sort(faces.begin(), faces.end());
            faces.at(faceCount - 1) = Face(equalized, score);
         }

     }


//     if(frameCount >= 10) {
//         frameCount = 0;
//         imwrite(FACE_RESULT_PATH + "img_"+ std::to_string(fileNameIndex) + ".jpg", crop);
//         ++fileNameIndex;
//     }
     //     ++frameCount;
}

void ImageProcessor::processAverageFace() {
    for(auto face: faces) {
        averageFaceCreator.getLandmarks(face);
    }

}
