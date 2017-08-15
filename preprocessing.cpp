#include "preprocessing.h"

int Preprocessing::readFileNames(vector<string> &filenames, const string &directory) {
#ifdef WINDOWS
    HANDLE dir;
    WIN32_FIND_DATA file_data;

    if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
        return; /* No files found */

    do {
        const string file_name = file_data.cFileName;
        const string full_file_name = directory + "/" + file_name;
        const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        if (file_name[0] == '.')
            continue;

        if (is_directory)
            continue;

        filenames.push_back(full_file_name);
    } while (FindNextFile(dir, &file_data));

    FindClose(dir);
#else
    DIR *dir;
    class dirent *ent;
    class stat st;

    dir = opendir(directory.c_str());
    while ((ent = readdir(dir)) != NULL) {
        const string file_name = ent->d_name;
        const string full_file_name = directory + "/" + file_name;

        if (file_name[0] == '.')
            continue;

        if (stat(full_file_name.c_str(), &st) == -1)
            continue;

        const bool is_directory = (st.st_mode & S_IFDIR) != 0;

        if (is_directory)
            continue;

//        filenames.push_back(full_file_name); // returns full path
        filenames.push_back(file_name); // returns just filename
    }
    closedir(dir);
#endif
    std::sort (filenames.begin(), filenames.end()); //optional, sort the filenames
    return(filenames.size()); //Return how many we found
}

void Preprocessing::prepareFaces() {
    vector<string> filenames;
    readFileNames(filenames, directory);
    readFiles();
    detectAndCropFaces();
}

void Preprocessing::readFiles() {
    vector<string> fileNames;

    int num_files = readFileNames(fileNames, directory);
    cout << "Number of files = " << num_files << endl;
    for (size_t i = 0; i < fileNames.size(); ++i) {
        cout << directory + fileNames[i] << " #" << i << endl;
        Mat src = imread(directory + fileNames[i]);

        if (!src.data) { //Protect against no file
            cerr << directory + fileNames[i] << ", file #" << i << ", is not an image" << endl;
            continue;
        }
        images.push_back(src);
    }
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

void Preprocessing::detectAndCropFaces() {
    for(size_t i = 0; i < images.size(); ++i) {
        detectAndCropFace(images[i]);
    }

}

void Preprocessing::detectAndCropFace(const Mat& src) {
    vector<Rect> detected_faces;
    //Mat frame_gray;
    //cvtColor(src, frame_gray, COLOR_BGR2GRAY);

    face_cascade.detectMultiScale(src, detected_faces);
    if(detected_faces.size() != 1)
        return;

    namedWindow("bw", 1);
    int x_left = detected_faces[0].x - CROP_THRESHOLD >= 0 ? detected_faces[0].x - CROP_THRESHOLD : 0;
    int x_right = detected_faces[0].size().width + CROP_THRESHOLDx2 < src.size().width ?
                  detected_faces[0].width + CROP_THRESHOLDx2 :
                  src.size().width;
    int y_up = detected_faces[0].y - CROP_THRESHOLD >= 0 ? detected_faces[0].y - CROP_THRESHOLD : 0;
    int y_down = detected_faces[0].size().height + CROP_THRESHOLDx2 < src.size().height ?
                 detected_faces[0].size().height + CROP_THRESHOLDx2 :
                 src.size().height;

    Rect rect(x_left, y_up, x_right, y_down);
    Mat crop = src(rect);
    Mat dst = rescale(crop);
    (*faces).push_back(dst);

    return;

}

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

    //namedWindow(source_window, CV_WINDOW_AUTOSIZE);
    //namedWindow(equalized_window, CV_WINDOW_AUTOSIZE);
    //namedWindow(equalized_window_cl, CV_WINDOW_AUTOSIZE);

    //imshow(source_window, im_gray);
    //imshow(equalized_window, dst_eq);
    //imshow(equalized_window_cl, dst_clahe);

    //waitKey(0);
    return dst_clahe;
}

void Preprocessing::writeImages(vector<Mat> &images) const {
    string path = FACE_RESULT_PATH;
    for(size_t i = 0; i < images.size(); ++i) {
        cv::imwrite(path + "BW/img_"+ to_string(i) + ".jpg", images.at(i));
    }
}

