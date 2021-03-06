#include "averageface.h"


void AverageFace::init(std::string path) {
    dlib::deserialize(path) >> shapePredictor;

    setBoundaryPoints();
    setEyecornerPoints();
    isInitialized = true;
}

void AverageFace::getLandmarks(Face& face) {
    write_log("start getLandmarks");
    if(!isInitialized) {
        init(getLocalPath() + LANDMARKS_PREDICTOR_PATH);

    }
    dlib::array2d<dlib::bgr_pixel> dlibImage;
    dlib::assign_image(dlibImage, dlib::cv_image<dlib::bgr_pixel>(face.face));
    dlib::rectangle rec(dlibImage.nc(), dlibImage.nr());
    dlib::full_object_detection shape = shapePredictor(dlibImage, rec);
    for(int i = 0; i < shape.num_parts(); ++i) {
        face.landmarks.push_back(cv::Point2f(shape.part(i).x(), shape.part(i).y()));
    }
    write_log( "finish getLandmarks" + std::to_string(face.landmarks.size()));
}

void AverageFace::setBoundaryPoints() {
    boundaryPts.push_back(cv::Point2f(0, 0));
    boundaryPts.push_back(cv::Point2f(FACE_SIZE_WE / 2, 0));
    boundaryPts.push_back(cv::Point2f(FACE_SIZE_WE - 1, 0));
    boundaryPts.push_back(cv::Point2f(FACE_SIZE_WE - 1, FACE_SIZE_HE / 2));
    boundaryPts.push_back(cv::Point2f(FACE_SIZE_WE - 1, FACE_SIZE_HE - 1));
    boundaryPts.push_back(cv::Point2f(FACE_SIZE_WE / 2, FACE_SIZE_HE - 1));
    boundaryPts.push_back(cv::Point2f(0, FACE_SIZE_HE - 1));
    boundaryPts.push_back(cv::Point2f(0, FACE_SIZE_HE / 2));
}

void AverageFace::setEyecornerPoints() {
    eyecornerDst.push_back(cv::Point2f(0.3 * FACE_SIZE_WE, FACE_SIZE_HE / 4));
    eyecornerDst.push_back(cv::Point2f(0.7 * FACE_SIZE_WE, FACE_SIZE_HE / 4));
    eyecornerSrc.push_back(cv::Point2f(0, 0));
    eyecornerSrc.push_back(cv::Point2f(0, 0));
}

void AverageFace::alignFace(Face& face) {
    write_log("Align face: landmarks count " + std::to_string(face.landmarks.size()));

    //cv::Mat img_face = face.face.clone();

    face.face.convertTo(face.face, CV_32FC3, 1);
    // The corners of the eyes are the landmarks number 36 and 45
    eyecornerSrc[0] = face.landmarks.at(36);
    eyecornerSrc[1] = face.landmarks.at(45);

    // Calculate similarity transform
    cv::Mat tform;
    similarityTransform(eyecornerSrc, eyecornerDst, tform);
    // Apply similarity transform to input image and landmarks
    cv::Mat img = cv::Mat::zeros(FACE_SIZE_HE, FACE_SIZE_WE, CV_32FC3);
    cv::warpAffine(face.face, img, tform, img.size(), cv::INTER_CUBIC,
                   cv::BORDER_CONSTANT, cv::Scalar(180, 180, 180));

    cv::transform(face.landmarks, face.landmarks, tform);
    for (size_t j = 0; j < boundaryPts.size(); j++) {
        face.landmarks.push_back(boundaryPts[j]);
    }
    face.face = img;
}

cv::Mat AverageFace::makeAverageFace(std::vector<Face>& faces) {
    write_log("start makeAverageFace");
    std::vector<cv::Point2f> pointsAvg(68);
    std::vector<std::vector<cv::Point2f>> pointsNorm;
    write_log("line 102: " + std::to_string(pointsAvg.size()));
    int faceCount = faces.size();
    write_log("face count: " + std::to_string(faces.size()));
    int i = 0;
    for(auto& face: faces) {
        for (size_t j = 0; j < LANDMARKS_COUNT; j++) {
            pointsAvg[j] += face.landmarks[j] * ( 1.0 / faceCount);
        }
        pointsNorm.push_back(face.landmarks);

        i++;
        imagesNorm.push_back(face.face);

    }
    // Append boundary points to average points.
    for ( size_t j = 0; j < boundaryPts.size(); j++) {
        pointsAvg.push_back(boundaryPts[j]);
    }
    // Calculate Delaunay triangles
    cv::Rect rect(0, 0, FACE_SIZE_WE, FACE_SIZE_HE);
    std::vector<std::vector<int>> dt;
    calculateDelaunayTriangles(rect, pointsAvg, dt);
    // Space for output images
    cv::Mat output = cv::Mat::ones(FACE_SIZE_HE, FACE_SIZE_WE, CV_32FC3);
    cv::Size size(FACE_SIZE_WE,FACE_SIZE_HE);
    write_log("Start averaging");
    // Warp input images to average image landmarks
    for(size_t i = 0; i < faceCount; i++) {
        cv::Mat img = cv::Mat::zeros(FACE_SIZE_HE, FACE_SIZE_WE, CV_32FC3);
        // Transform triangles one by one
        for(size_t j = 0; j < dt.size(); j++) {
            // Input and output points corresponding to jth triangle
            std::vector<cv::Point2f> tin;
            std::vector<cv::Point2f> tout;
            for(int k = 0; k < 3; k++) {
                cv::Point2f pIn = pointsNorm[i][dt[j][k]];
                constrainPoint(pIn, size);

                cv::Point2f pOut = pointsAvg[dt[j][k]];
                constrainPoint(pOut,size);

                tin.push_back(pIn);
                tout.push_back(pOut);
            }
            warpTriangle(imagesNorm[i], img, tin, tout);
        }
        output = output + img;
    }

    output = output / (double)faceCount;
    cv::Mat cropped = cropFace(output);

    imagesNorm.clear();

    write_log("finish makeAverageFace");
    return cropped;
}

void AverageFace::similarityTransform(std::vector<cv::Point2f> &inPoints,
                                      std::vector<cv::Point2f> &outPoints,
                                      cv::Mat& tform) {
    std::vector <cv::Point2f> inPts = inPoints;
    std::vector <cv::Point2f> outPts = outPoints;

    inPts.push_back(cv::Point2f(0,0));
    outPts.push_back(cv::Point2f(0,0));

    inPts[2].x =  c60 * (inPts[0].x - inPts[1].x) - s60 *
            (inPts[0].y - inPts[1].y) + inPts[1].x;

    inPts[2].y =  s60 * (inPts[0].x - inPts[1].x) + c60 *
            (inPts[0].y - inPts[1].y) + inPts[1].y;

    outPts[2].x =  c60 * (outPts[0].x - outPts[1].x) - s60 *
            (outPts[0].y - outPts[1].y) + outPts[1].x;

    outPts[2].y =  s60 * (outPts[0].x - outPts[1].x) + c60 *
            (outPts[0].y - outPts[1].y) + outPts[1].y;

    tform = cv::estimateRigidTransform(inPts, outPts, false);
}

void AverageFace::calculateDelaunayTriangles(cv::Rect rect,
                                             std::vector<cv::Point2f> &points,
                                             std::vector<std::vector<int> > &delaunayTri) {
    write_log( "start delaunay" );

    cv::Subdiv2D subdiv(rect);
    for(std::vector<cv::Point2f>::iterator it = points.begin(); it != points.end(); it++) {
        subdiv.insert(*it);
    }

    std::vector<cv::Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    std::vector<cv::Point2f> pt(3);
    std::vector<int> ind(3);

    for(size_t i = 0; i < triangleList.size(); ++i) {
        cv::Vec6f t = triangleList[i];
        pt[0] = cv::Point2f(t[0], t[1]);
        pt[1] = cv::Point2f(t[2], t[3]);
        pt[2] = cv::Point2f(t[4], t[5 ]);

        if(rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2])) {
            for(int j = 0; j < 3; j++) {
                for (size_t k = 0; k < points.size(); k++) {
                    if (abs(pt[j].x - points[k].x) < 1.0 && abs(pt[j].y - points[k].y) < 1) {
                        ind[j] = k;
                    }
                }
            }
            delaunayTri.push_back(ind);
        }
    }
    std::cout << "finish delaunay" << std::endl;

}

void AverageFace::applyAffineTransform(cv::Mat& warpImage, cv::Mat& src,
                                       std::vector<cv::Point2f> &srcTri,
                                       std::vector<cv::Point2f> &dstTri) {
    // Given a pair of triangles, find the affine transform.
    cv::Mat warpMat = getAffineTransform(srcTri, dstTri);

    // Apply the Affine Transform just found to the src image
    warpAffine(src, warpImage, warpMat, warpImage.size(), cv::INTER_LINEAR,
               cv::BORDER_REFLECT_101);
}

void AverageFace::warpTriangle(cv::Mat& img1, cv::Mat& img2,
                               std::vector<cv::Point2f> t1,
                               std::vector<cv::Point2f> t2) {
    // Find bounding rectangle for each triangle
    cv::Rect r1 = boundingRect(t1);
    cv::Rect r2 = boundingRect(t2);

    // Offset points by left top corner of the respective rectangles
    std::vector<cv::Point2f> t1Rect;
    std::vector<cv::Point2f> t2Rect;
    std::vector<cv::Point> t2RectInt;
    for(int i = 0; i < 3; i++) {
        t2RectInt.push_back(cv::Point((int)(t2[i].x - r2.x), (int)(t2[i].y - r2.y)));

        t1Rect.push_back(cv::Point2f( t1[i].x - r1.x, t1[i].y -  r1.y) );
        t2Rect.push_back(cv::Point2f( t2[i].x - r2.x, t2[i].y - r2.y) );
    }

    // Get mask by filling triangle
    cv::Mat mask = cv::Mat::zeros(r2.height, r2.width, CV_32FC3);
    fillConvexPoly(mask, t2RectInt, cv::Scalar(1.0, 1.0, 1.0), 16, 0);

    // Apply warpImage to small rectangular patches
    cv::Mat img1Rect;
    img1(r1).copyTo(img1Rect);

    cv::Mat warpImage = cv::Mat::zeros(r2.height, r2.width, img1Rect.type());

    applyAffineTransform(warpImage, img1Rect, t1Rect, t2Rect);

    // Copy triangular region of the rectangular patch to the output image
    multiply(warpImage,mask, warpImage);
    multiply(img2(r2), cv::Scalar(1.0,1.0,1.0) - mask, img2(r2));
    img2(r2) = img2(r2) + warpImage;
}

void AverageFace::constrainPoint(cv::Point2f &p, cv::Size sz) {
    p.x = std::min(std::max( (double)p.x, 0.0), (double)(sz.width - 1));
    p.y = std::min(std::max( (double)p.y, 0.0), (double)(sz.height - 1));
}
