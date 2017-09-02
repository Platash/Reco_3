#include "averageface.h"

void AverageFace::init(std::string path) {
    dlib::deserialize(path) >> shapePredictor;
    isInitialized = true;
}

void AverageFace::getLandmarks(Face& face) {
    std::cout << "start getLandmarks" << std::endl;
    //array2d<rgb_pixel> img;
    if(!isInitialized) {
        init(LANDMARKS_PREDICTOR_PATH);

    }
    dlib::array2d<dlib::bgr_pixel> dlibImage;
    dlib::assign_image(dlibImage, dlib::cv_image<dlib::bgr_pixel>(face.face));
    dlib::rectangle rec(dlibImage.nc(), dlibImage.nr());

    dlib::full_object_detection shape = shapePredictor(dlibImage, rec);
    std::cout << "shape.size: " << shape.num_parts() << std::endl;
    for(int i = 0; i < shape.num_parts(); ++i) {
        //std::cout << i << " " << shape.part(i).x() << " " << shape.part(i).y()<< std::endl;
        face.landmarks.push_back(cv::Point2f(shape.part(i).x(), shape.part(i).y()));
    }
    std::cout << "finish getLandmarks" <<face.landmarks.size() <<   std::endl;
}

cv::Mat AverageFace::makeAverageFace(std::vector<Face>& faces) {
    std::cout << "start makeAverageFace" << std::endl;

    int numImages = faces.size();
    std::cout << "31" << std::endl;

    // Eye corners
    std::vector<cv::Point2f> eyecornerDst, eyecornerSrc;
    eyecornerDst.push_back(cv::Point2f(0.3 * FACE_MAX_SIZE_W, FACE_MAX_SIZE_H / 3));
    eyecornerDst.push_back(cv::Point2f(0.7 * FACE_MAX_SIZE_W, FACE_MAX_SIZE_H / 3));

    eyecornerSrc.push_back(cv::Point2f(0, 0));
    eyecornerSrc.push_back(cv::Point2f(0, 0));

    std::cout << "41" << std::endl;

    // Space for normalized images and points.
    std::vector <cv::Mat> imagesNorm;
    std::vector <std::vector<cv::Point2f>> pointsNorm;

    // Space for average landmark points
    std::vector<cv::Point2f> pointsAvg(faces.size());

    // 8 Boundary points for Delaunay Triangulation
    std::vector<cv::Point2f> boundaryPts;
    boundaryPts.push_back(cv::Point2f(0,0));
    boundaryPts.push_back(cv::Point2f(FACE_MAX_SIZE_W / 2, 0));
    boundaryPts.push_back(cv::Point2f(FACE_MAX_SIZE_W - 1,0));
    boundaryPts.push_back(cv::Point2f(FACE_MAX_SIZE_W - 1, FACE_MAX_SIZE_H / 2));
    boundaryPts.push_back(cv::Point2f(FACE_MAX_SIZE_W - 1, FACE_MAX_SIZE_H - 1));
    boundaryPts.push_back(cv::Point2f(FACE_MAX_SIZE_W / 2, FACE_MAX_SIZE_H - 1));
    boundaryPts.push_back(cv::Point2f(0, FACE_MAX_SIZE_H - 1));
    boundaryPts.push_back(cv::Point2f(0, FACE_MAX_SIZE_H / 2));

    // Warp images and trasnform landmarks to output coordinate system,
    // and find average of transformed landmarks.
    std::cout << "63" << std::endl;
    std::cout << "64: " << faces.at(0).landmarks.size() << std::endl;
    for(auto& face: faces) {
        std::vector <cv::Point2f> points = face.landmarks;
        std::cout << face.landmarks.size() <<std::endl;
        cv::Mat img_face = face.face;
        std::cout << "68" << std::endl;

        img_face.convertTo(img_face, CV_32FC3, 1/255.0);
        std::cout << "71" << std::endl;
        // The corners of the eyes are the landmarks number 36 and 45
        eyecornerSrc[0] = face.landmarks.at(36);
        eyecornerSrc[1] = face.landmarks.at(45);

        // Calculate similarity transform
        cv::Mat tform;
        std::cout << "78" << std::endl;
        similarityTransform(eyecornerSrc, eyecornerDst, tform);
        std::cout << "80" << std::endl;
        // Apply similarity transform to input image and landmarks
        cv::Mat img = cv::Mat::zeros(FACE_MAX_SIZE_H, FACE_MAX_SIZE_W, CV_32FC3);
        std::cout << "83" << std::endl;
        warpAffine(img_face, img, tform, img.size());
        std::cout << "75" << std::endl;
        transform(points, points, tform);
        std::cout << "87" << std::endl;
        // Calculate average landmark locations
        for (size_t j = 0; j < points.size(); j++) {
            pointsAvg[j] += points[j] * ( 1.0 / numImages);
        }
        std::cout << "92" << std::endl;
        // Append boundary points. Will be used in Delaunay Triangulation
        for (size_t j = 0; j < boundaryPts.size(); j++) {
            points.push_back(boundaryPts[j]);
        }

        pointsNorm.push_back(points);
        imagesNorm.push_back(img);
    }
    std::cout << "103" << std::endl;

    // Append boundary points to average points.
    for ( size_t j = 0; j < boundaryPts.size(); j++) {
        std::cout << "J: " << j <<std::endl;
        std::cout << boundaryPts.size() << std::endl;
        std::cout << pointsAvg.size() << std::endl;

        pointsAvg.push_back(boundaryPts[j]);
    }
    std::cout << "109" << std::endl;

    // Calculate Delaunay triangles
    cv::Rect rect(0, 0, FACE_MAX_SIZE_W, FACE_MAX_SIZE_H);
    std::vector<std::vector<int>> dt;
    calculateDelaunayTriangles(rect, pointsAvg, dt);
    std::cout << "115" << std::endl;

    // Space for output image
    cv::Mat output = cv::Mat::zeros(FACE_MAX_SIZE_H, FACE_MAX_SIZE_W, CV_32FC3);
    cv::Size size(FACE_MAX_SIZE_W,FACE_MAX_SIZE_H);

    // Warp input images to average image landmarks
    for(size_t i = 0; i < numImages; i++) {
        cv::Mat img = cv::Mat::zeros(FACE_MAX_SIZE_H, FACE_MAX_SIZE_W, CV_32FC3);
        // Transform triangles one by one
        for(size_t j = 0; j < dt.size(); j++) {
            // Input and output points corresponding to jth triangle
            std::vector<cv::Point2f> tin, tout;
            for(int k = 0; k < 3; k++) {
                cv::Point2f pIn = pointsNorm[i][dt[j][k]];
                constrainPoint(pIn, size);

                cv::Point2f pOut = pointsAvg[dt[j][k]];
                constrainPoint(pOut,size);
                std::cout << "134" << std::endl;

                tin.push_back(pIn);
                tout.push_back(pOut);
            }
            std::cout << "139" << std::endl;

            warpTriangle(imagesNorm[i], img, tin, tout);
        }

        // Add image intensities for averaging
        output = output + img;
        std::cout << "146" << std::endl;

    }

    // Divide by numImages to get average
    output = output / (double)numImages;
    std::cout << "finish makeAverageFace" << std::endl;
    return output;
}

void AverageFace::similarityTransform(std::vector<cv::Point2f> &inPoints, std::vector<cv::Point2f> &outPoints,
                                      cv::Mat &tform) {
    std::vector <cv::Point2f> inPts = inPoints;
    std::vector <cv::Point2f> outPts = outPoints;

    inPts.push_back(cv::Point2f(0,0));
    outPts.push_back(cv::Point2f(0,0));

    inPts[2].x =  c60 * (inPts[0].x - inPts[1].x) - s60 * (inPts[0].y - inPts[1].y) + inPts[1].x;
    inPts[2].y =  s60 * (inPts[0].x - inPts[1].x) + c60 * (inPts[0].y - inPts[1].y) + inPts[1].y;

    outPts[2].x =  c60 * (outPts[0].x - outPts[1].x) - s60 * (outPts[0].y - outPts[1].y) + outPts[1].x;
    outPts[2].y =  s60 * (outPts[0].x - outPts[1].x) + c60 * (outPts[0].y - outPts[1].y) + outPts[1].y;

    tform = cv::estimateRigidTransform(inPts, outPts, false);
}

void AverageFace::calculateDelaunayTriangles(cv::Rect rect, std::vector<cv::Point2f> &points,
                                             std::vector<std::vector<int> > &delaunayTri) {
    std::cout << "start delaunay" << std::endl;
    // Create an instance of Subdiv2D
    cv::Subdiv2D subdiv(rect);

    // Insert points into subdiv
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

void AverageFace::applyAffineTransform(cv::Mat &warpImage, cv::Mat &src, std::vector<cv::Point2f> &srcTri,
                                       std::vector<cv::Point2f> &dstTri) {
    // Given a pair of triangles, find the affine transform.
    cv::Mat warpMat = getAffineTransform(srcTri, dstTri);

    // Apply the Affine Transform just found to the src image
    warpAffine(src, warpImage, warpMat, warpImage.size(), cv::INTER_LINEAR, cv::BORDER_REFLECT_101);
}

void AverageFace::warpTriangle(cv::Mat &img1, cv::Mat &img2, std::vector<cv::Point2f> t1,
                               std::vector<cv::Point2f> t2) {
    // Find bounding rectangle for each triangle
    cv::Rect r1 = boundingRect(t1);
    cv::Rect r2 = boundingRect(t2);

    // Offset points by left top corner of the respective rectangles
    std::vector<cv::Point2f> t1Rect;
    std::vector<cv::Point2f> t2Rect;
    std::vector<cv::Point> t2RectInt;
    for(int i = 0; i < 3; i++) {
        //tRect.push_back( Point2f( t[i].x - r.x, t[i].y -  r.y) );
        t2RectInt.push_back(cv::Point((int)(t2[i].x - r2.x), (int)(t2[i].y - r2.y)) ); // for fillConvexPoly

        t1Rect.push_back(cv::Point2f( t1[i].x - r1.x, t1[i].y -  r1.y) );
        t2Rect.push_back(cv::Point2f( t2[i].x - r2.x, t2[i].y - r2.y) );
    }

    // Get mask by filling triangle
    cv::Mat mask = cv::Mat::zeros(r2.height, r2.width, CV_32FC3);
    fillConvexPoly(mask, t2RectInt, cv::Scalar(1.0, 1.0, 1.0), 16, 0);

    // Apply warpImage to small rectangular patches
    cv::Mat img1Rect, img2Rect;
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
