#include "common.h"

using namespace std;

typedef struct stat Stat;

//======================= IO methods ==================================================

int readFileNames(vector<string> &filenames, const string &directory) {
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

int readSubdirNames(std::vector<string> &subdirnames, const string &directory) {
#ifdef WINDOWS
    HANDLE dir;
    WIN32_FIND_DATA file_data;

    if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
        return; /* No files found */

    do {
        const string file_name = file_data.cFileName;
        const string full_file_name = directory + "/" + file_name;
        const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        if (file_name[0] == '.') {
            continue;
        }
        if (is_directory) {
            subdirnames.push_back(full_file_name);
        }

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

        if (is_directory) {
            subdirnames.push_back(file_name);
            //        filenames.push_back(full_file_name);
        }

    }
    closedir(dir);
#endif
    std::sort (subdirnames.begin(), subdirnames.end()); //optional, sort the filenames
    return(subdirnames.size()); //Return how many we found
}


void readImages(std::string directory, std::vector<cv::Mat>& images) {
    vector<string> fileNames;
    std::cout << "starting readImages " << directory << std::endl;
    int num_files = readFileNames(fileNames, directory);
    cout << "Number of files = " << num_files << endl;
    for (size_t i = 0; i < fileNames.size(); ++i) {
        cout << directory + fileNames[i] << " #" << i << endl;
        cv::Mat src = cv::imread(directory + fileNames[i], CV_LOAD_IMAGE_GRAYSCALE);

        if (!src.data) { //Protect against no file
            cerr << directory + fileNames[i] << ", file #" << i << ", is not an image" << endl;
            continue;
        }
        images.push_back(src);
    }
}

void mkdir(std::string path) {
    struct stat st = {0};

    if (stat(path.c_str(), &st) == -1) {
        mkdir(path.c_str(), 0777);
    }
}

void writeImages(std::vector<cv::Mat> &images, std::string path) {
    std::cout << "Start: writeImages: " << path << " " << images.size() << std::endl;

    mkdir(path);

    for(size_t i = 0; i < images.size(); ++i) {
        cv::imwrite(path + "img_"+ to_string(i) + ".jpg", images.at(i));
    }
}

void read_csv(const string& filename, vector<cv::Mat>& images, vector<int>& labels, char separator) {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(cv::Error::StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(cv::imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

void writeShapeToFile(dlib::full_object_detection shape, string path) {
    ofstream dst;
    dst.open(path);
    for(int i = 0; i < shape.num_parts(); ++i) {
        dst << shape.part(i).x() - 1 << " " << shape.part(i).y() - 1 << std::endl;
    }
    dst.close();
}
#define LOGGING

void write_log(std::string text) {
#ifdef LOGGING
    std::cout << text << endl;
#endif
}

//========================= Image processing ============================================

QPixmap mat2Pixmap(cv::Mat matImg) {
    QImage img;
    cv::Mat RGBframe;
    if (matImg.channels()== 3){
        cv::cvtColor(matImg, RGBframe, CV_BGR2RGB);
        img = QImage((const unsigned char*)(RGBframe.data), RGBframe.cols, RGBframe.rows, QImage::Format_RGB888);
    } else {
        img = QImage((const unsigned char*)(matImg.data), matImg.cols, matImg.rows, QImage::Format_Indexed8);
    }
    return QPixmap::fromImage(img);
}

cv::Mat norm_0_255(cv::InputArray _src) {
    cv::Mat src = _src.getMat();
    cv::Mat dst;
    switch(src.channels()) {
    case 1:
        cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(_src, dst, 0, 255, cv::NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}

QImage cvMat2qImage(cv::Mat mat) {

    if(mat.type()==CV_8UC1) {
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++) {
            colorTable.push_back(qRgb(i,i,i));
        }
        QImage img((const uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    } else if(mat.type()==CV_8UC3) {
        QImage img((const uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img;
    } else if(mat.type()==CV_16UC1) {
        mat.convertTo(mat, CV_8UC1, 1.0/256.0);
        return cvMat2qImage(mat);
    } else if(mat.type()==CV_16UC3) {
        mat.convertTo(mat, CV_8UC3, 1.0/256.0);
        return cvMat2qImage(mat);
    } else if(mat.type()==CV_32FC1) {
        cv::Mat rgb(mat.size(), CV_32FC3);
        rgb.addref();
        cv::cvtColor(mat, rgb, cv::COLOR_GRAY2RGB);
        QImage img((const uchar*)rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB32);
        rgb.release();
        return img;
    } else if(mat.type()==CV_32FC3) {
        mat.convertTo(mat, CV_8UC3);
        return cvMat2qImage(mat);
    } else {
        std::cerr << "SMVideoWidget: cv::Mat could not be converted to QImage!";
        return QImage();
    }
}


void drawMask(cv::Mat& src, cv::Point2f left, cv::Point2f right,
                 cv::Point2f center, cv::Point2f down) {

    cv::Mat mask (src.rows, src.cols, CV_8U);
    cv::Mat blurredMask;
    cv::Mat output = src.clone();
    cv::Point2f topLeft (left.x - 20 , (left.y - down.y) - 20);
    cv::Point2f downLeft (left.x - 20, down.y + 20);
    cv::Point2f downRight (right.x + 20, down.y + 20);
   // cv::Point2f center = right - left;
    cv::RotatedRect rect = cv::RotatedRect(topLeft, downLeft, downRight);
    cv::ellipse(mask, rect, cv::Scalar(255,255,255), CV_FILLED);

    blur(mask, blurredMask, cv::Size(10, 10));
    src.copyTo(output, blurredMask);
    src = output;

}
