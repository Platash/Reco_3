//#include "imageprocessor.h"
//#include "common/common.h"

//bool ImageProcessor::pickFace(cv::Mat image, cv::Rect2d& roi, cv::Rect2d& bestRoi) {
//    cv::Rect bestFaceRoi;
//    if(faceDetector.getBestFace(image, roi, bestFaceRoi)) {
//        write_log("got best fase ");
//        cv::Mat crop = image(bestFaceRoi);
//       // cv::imwrite("/home/siobhan/UJ/Masters_stuff/results/tracked_faces/img_" + to_string(rand() % 300 + 19850) + ".jpg", crop);

//        bestRoi = bestFaceRoi;
//        double score = qualityAssessment.getScore(crop);
//        write_log("quality: " + std::to_string(score));
//        if(faces.size() < faceCount) {
//            faces.push_back(Face(crop, score));
//            //cv::imwrite("/home/siobhan/UJ/Masters_stuff/results/tracked_faces/img1_" + to_string(rand() % 300 + 19850) + ".jpg", crop);
//            return true;

//        } else if(score > worstScore) {
//            std::sort(faces.begin(), faces.end());
//            faces.at(faceCount - 1) = Face(crop, score);
//            //cv::imwrite("/home/siobhan/UJ/Masters_stuff/results/tracked_faces/img2_" + to_string(rand() % 300 + 19850) + ".jpg", crop);
//            return true;
//        }
//        return true;
//    }
//    return false;
//}


//bool ImageProcessor::processAverageFace() {
//    int i = 0;
//    for(auto& face: faces) {
//        cv::imwrite("/home/siobhan/UJ/Masters_stuff/results/best/img_" + to_string(i) + ".jpg", face.face);
//        averageFaceCreator.getLandmarks(face);
//        ++i;
//    }
//    averageFace = averageFaceCreator.makeAverageFace(faces);

//    cv::imwrite("/home/siobhan/UJ/Masters_stuff/results/best/img_best.jpg", averageFace);

//    std::cout << "finish processAverageFace" << std::endl;
//    return true;
//}

//std::vector<Face> &ImageProcessor::getFaces() {
//    return faces;
//}

//int ImageProcessor::getFaceCount() {
//    return faces.size();
//}

//void ImageProcessor::resetFaces() {
//    faces.clear();
//}
