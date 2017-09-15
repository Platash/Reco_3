#include "common.h"

#include <dirent.h>     // for linux systems
#include <sys/stat.h>   // for linux systems
#include <algorithm>    // std::sort

using namespace std;

typedef struct stat Stat;

//======================= IO methods ==================================================

int readFileNames(vector<string> &filenames, const string &directory) {

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

    std::sort (filenames.begin(), filenames.end()); //optional, sort the filenames
    return(filenames.size()); //Return how many we found
}

int readSubdirNames(std::vector<string> &subdirnames, const string &directory) {

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
        cv::Mat src = cv::imread(directory + fileNames[i]);

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

void writeImage(cv::Mat image, std::string path, std::string name) {
    mkdir(path);
    cv::imwrite(path + name + ".jpg", image);
}

void writeImages(std::vector<cv::Mat> &images, std::string path, std::string name) {
    std::cout << "Start: writeImages: " << path << " " << images.size() << std::endl;

    mkdir(path);

    for(size_t i = 0; i < images.size(); ++i) {
        cv::imwrite(path + name + to_string(i) + ".jpg", images.at(i));
    }
}

void writeImages(std::vector<Face> &faces, std::string path, std::string name) {
    std::cout << "Start: writeImages: " << path << " " << faces.size() << std::endl;

    mkdir(path);

    for(size_t i = 0; i < faces.size(); ++i) {
        cv::imwrite(path + name + to_string(i) + ".jpg", faces.at(i).face);
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

cv::Mat normalize(cv::InputArray _src) {
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


void drawMask(cv::Mat& src, cv::Point2f left, cv::Point2f right, cv::Point2f down) {

    cv::Mat mask(src.rows, src.cols, src.type(), cv::Scalar(255,255,255));
    cv::Mat blurredMask;
    cv::Point2f topLeft(left.x - 26 , (left.y - down.y) - 26);
    cv::Point2f downLeft(left.x - 26, down.y + 26);
    cv::Point2f downRight(right.x + 26, down.y + 26);

    cv::RotatedRect rect = cv::RotatedRect(topLeft, downLeft, downRight);
    cv::ellipse(mask, rect, cv::Scalar(0,0,0), CV_FILLED);
    blur(mask, blurredMask, cv::Size(50, 50));
    src = src + blurredMask;

    cv::imwrite("/home/siobhan/UJ/Masters_stuff/results/best/img_mask.jpg", mask);
    cv::imwrite("/home/siobhan/UJ/Masters_stuff/results/best/img_blurred_mask.jpg", blurredMask);
    cv::imwrite("/home/siobhan/UJ/Masters_stuff/results/best/img_masked_face.jpg", src);

}

cv::Mat cropFace(cv::Mat img) {
    cv::Size size(FACE_SIZE_H, FACE_SIZE_W);
    cv::Mat scaled;
    resize(img, scaled, size);

    cv::Rect roi;
    roi.x = 50;
    roi.y = 20;
    roi.width = scaled.size().width - 100;
    roi.height = scaled.size().height - 80;

    cv::Mat cropped = scaled(roi);

    return cropped;
}
