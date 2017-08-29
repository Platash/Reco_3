#include "averageface.h"

void AverageFace::init(std::string path) {
    dlib::deserialize(path) >> shapePredictor;
}

void AverageFace::getLandmarks(Face face) {
    //array2d<rgb_pixel> img;
    if(!isInitialized) {
        init(LANDMARKS_PREDICTOR_PATH);
    }
    dlib::array2d<dlib::bgr_pixel> dlibImage;
    dlib::assign_image(dlibImage, dlib::cv_image<dlib::bgr_pixel>(face.face));
    dlib::rectangle rec(dlibImage.nc(), dlibImage.nr());

    dlib::full_object_detection shape = shapePredictor(dlibImage, rec);

    for(int i = 0; i < shape.num_parts(); ++i) {
        face.landmarks.push_back(cv::Point2f(shape.part(i).x(), shape.part(i).y()));
    }
}

void AverageFace::makeAverageFace(std::vector<Face>& faces) {

    int numImages = faces.size();

    // Eye corners
    std::vector<cv::Point2f> eyecornerDst, eyecornerSrc;
    eyecornerDst.push_back(cv::Point2f( 0.3*FACE_MAX_SIZE_W, FACE_MAX_SIZE_H/3));
    eyecornerDst.push_back(cv::Point2f( 0.7*FACE_MAX_SIZE_W, FACE_MAX_SIZE_H/3));

    eyecornerSrc.push_back(cv::Point2f(0,0));
    eyecornerSrc.push_back(cv::Point2f(0,0));

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

    for(auto face: faces) {
        std::vector <cv::Point2f> points = face.landmarks;
        cv::Mat img_face = face.face;
        img_face.convertTo(img_face, CV_32FC3, 1/255.0);
        // The corners of the eyes are the landmarks number 36 and 45
        eyecornerSrc[0] = face.landmarks.at(36);
        eyecornerSrc[1] = face.landmarks.at(45);

        // Calculate similarity transform
        cv::Mat tform;
        similarityTransform(eyecornerSrc, eyecornerDst, tform);

        // Apply similarity transform to input image and landmarks
        cv::Mat img = cv::Mat::zeros(FACE_MAX_SIZE_H, FACE_MAX_SIZE_W, CV_32FC3);
        warpAffine(img_face, img, tform, img.size());
        transform(points, points, tform);

        // Calculate average landmark locations
        for (size_t j = 0; j < points.size(); j++) {
            pointsAvg[j] += points[j] * ( 1.0 / numImages);
        }

        // Append boundary points. Will be used in Delaunay Triangulation
        for (size_t j = 0; j < boundaryPts.size(); j++) {
            points.push_back(boundaryPts[j]);
        }

        pointsNorm.push_back(points);
        imagesNorm.push_back(img);
    }

    // Append boundary points to average points.
    for ( size_t j = 0; j < boundaryPts.size(); j++) {
        pointsAvg.push_back(boundaryPts[j]);
    }

    // Calculate Delaunay triangles
    cv::Rect rect(0, 0, FACE_MAX_SIZE_W, FACE_MAX_SIZE_H);
    std::vector<std::vector<int>> dt;
    calculateDelaunayTriangles(rect, pointsAvg, dt);

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

                tin.push_back(pIn);
                tout.push_back(pOut);
            }

            warpTriangle(imagesNorm[i], img, tin, tout);
        }

        // Add image intensities for averaging
        output = output + img;

    }

    // Divide by numImages to get average
    output = output / (double)numImages;
}

void AverageFace::similarityTransform(std::vector<cv::Point2f> &inPoints, std::vector<cv::Point2f> &outPoints,
                                      cv::Mat &tform) {
    vector <Point2f> inPts = inPoints;
    vector <Point2f> outPts = outPoints;

    inPts.push_back(Point2f(0,0));
    outPts.push_back(Point2f(0,0));

    inPts[2].x =  c60 * (inPts[0].x - inPts[1].x) - s60 * (inPts[0].y - inPts[1].y) + inPts[1].x;
    inPts[2].y =  s60 * (inPts[0].x - inPts[1].x) + c60 * (inPts[0].y - inPts[1].y) + inPts[1].y;

    outPts[2].x =  c60 * (outPts[0].x - outPts[1].x) - s60 * (outPts[0].y - outPts[1].y) + outPts[1].x;
    outPts[2].y =  s60 * (outPts[0].x - outPts[1].x) + c60 * (outPts[0].y - outPts[1].y) + outPts[1].y;

    tform = cv::estimateRigidTransform(inPts, outPts, false);
}

void AverageFace::calculateDelaunayTriangles(cv::Rect rect, std::vector<cv::Point2f> &points,
                                             std::vector<std::vector<int> > &delaunayTri) {
    // Create an instance of Subdiv2D
    Subdiv2D subdiv(rect);

    // Insert points into subdiv
    for( vector<Point2f>::iterator it = points.begin(); it != points.end(); it++) {
        subdiv.insert(*it);
    }

    vector<Vec6f> triangleList;
    subdiv.getTriangleList(triangleList);
    vector<Point2f> pt(3);
    vector<int> ind(3);

    for(size_t i = 0; i < triangleList.size(); ++i) {
        Vec6f t = triangleList[i];
        pt[0] = Point2f(t[0], t[1]);
        pt[1] = Point2f(t[2], t[3]);
        pt[2] = Point2f(t[4], t[5 ]);

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
}

void AverageFace::applyAffineTransform(cv::Mat &warpImage, cv::Mat &src, std::vector<cv::Point2f> &srcTri,
                                       std::vector<cv::Point2f> &dstTri) {
    // Given a pair of triangles, find the affine transform.
    Mat warpMat = getAffineTransform( srcTri, dstTri );

    // Apply the Affine Transform just found to the src image
    warpAffine( src, warpImage, warpMat, warpImage.size(), INTER_LINEAR, BORDER_REFLECT_101);
}

void AverageFace::warpTriangle(cv::Mat &img1, cv::Mat &img2, std::vector<cv::Point2f> t1,
                               std::vector<cv::Point2f> t2) {
    // Find bounding rectangle for each triangle
    Rect r1 = boundingRect(t1);
    Rect r2 = boundingRect(t2);

    // Offset points by left top corner of the respective rectangles
    vector<Point2f> t1Rect, t2Rect;
    vector<Point> t2RectInt;
    for(int i = 0; i < 3; i++) {
        //tRect.push_back( Point2f( t[i].x - r.x, t[i].y -  r.y) );
        t2RectInt.push_back(Point((int)(t2[i].x - r2.x), (int)(t2[i].y - r2.y)) ); // for fillConvexPoly

        t1Rect.push_back(Point2f( t1[i].x - r1.x, t1[i].y -  r1.y) );
        t2Rect.push_back(Point2f( t2[i].x - r2.x, t2[i].y - r2.y) );
    }

    // Get mask by filling triangle
    Mat mask = Mat::zeros(r2.height, r2.width, CV_32FC3);
    fillConvexPoly(mask, t2RectInt, Scalar(1.0, 1.0, 1.0), 16, 0);

    // Apply warpImage to small rectangular patches
    Mat img1Rect, img2Rect;
    img1(r1).copyTo(img1Rect);

    Mat warpImage = Mat::zeros(r2.height, r2.width, img1Rect.type());

    applyAffineTransform(warpImage, img1Rect, t1Rect, t2Rect);

    // Copy triangular region of the rectangular patch to the output image
    multiply(warpImage,mask, warpImage);
    multiply(img2(r2), Scalar(1.0,1.0,1.0) - mask, img2(r2));
    img2(r2) = img2(r2) + warpImage;
}

void AverageFace::constrainPoint(cv::Point2f &p, cv::Size sz) {
    p.x = min(max( (double)p.x, 0.0), (double)(sz.width - 1));
    p.y = min(max( (double)p.y, 0.0), (double)(sz.height - 1));
}
