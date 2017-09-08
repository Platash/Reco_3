#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <QWidget>
#include <QResource>
#include <QIcon>
#include <QPixmap>
#include <QMessageBox>

//#include <opencv2/core/utility.hpp>
//#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
//#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include "opencv2/opencv.hpp"

#include <thread>
#include <atomic>
#include <time.h>

#include "videolabel.h"
#include "imageprocessor.h"
#include "peopletracker.h"
#include "facedetector.h"
#include "preprocessing.h"
#include "common/common.h"
#include "imageprocessor.h"
#include "recowindow.h"
#include "reco/facerecognition.h"

namespace Ui {
class VideoWindow;
}

class VideoLabel;
//class ImageProcessor;

enum class State:uint8_t {
    STOPPED = 0,
    PAUSED = 1,
    PLAYING = 2,
    REWINDING_F = 3,
    REWINDING_B = 4,
};

class VideoWindow : public QWidget {
    Q_OBJECT

public:
    explicit VideoWindow(std::string fileName, FaceRecognition* reco_, QWidget *parent = 0);
    ~VideoWindow();

    void setLabel();

    friend class VideoLabel;

public slots:
    void setSelection(QPoint p1, QPoint p2);

private slots:
    void on_b_rewind_b_pressed();

    void on_b_rewind_b_released();

    void on_b_stop_clicked();

    void on_b_previous_clicked();

    void on_b_play_clicked();

    void on_b_pause_clicked();

    void on_b_next_clicked();

    void on_b_unselect_clicked();

    void on_b_rewind_f_pressed();

    void on_b_rewind_f_released();

private:

    void play();
    void stop();
    void msleep(int ms);
    void updateImage();

    void setIcons();
    void setFailedScreen();
    bool askForAverageFace();
    void updateSmallFaces(std::vector<Face>& faces);
    void showRecoWindow(QImage qimage, int id);

    Ui::VideoWindow *ui;
    std::string fileName;
    cv::VideoCapture* capture;
    std::atomic<State> state;
    std::atomic<bool> isTracking;
    PeopleTracker myTracker;
    int frameRate;
    std::thread* playThread;
    cv::Mat currentFrame;
    QPixmap currentPixmap;
    time_t start;
    time_t end;
    VideoLabel* l_video;
    RecoWindow* recoWindow;
    cv::Point p1;
    cv::Point p2;
    Preprocessing prep;
    ImageProcessor processor;
    std::vector<QLabel> smallFaces;
    FaceRecognition* reco;

};

#endif // VIDEOWINDOW_H
