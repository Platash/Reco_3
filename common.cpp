#include "common.h"

using namespace std;

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

void readImages(std::string& directory, std::vector<cv::Mat>& images) {
    vector<string> fileNames;

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

void writeImages(std::vector<cv::Mat> &images, std::string path) {
    for(size_t i = 0; i < images.size(); ++i) {
        cv::imwrite(path + "BW/img_"+ to_string(i) + ".jpg", images.at(i));
    }
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
