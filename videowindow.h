#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <QWidget>
#include <QResource>
#include <QIcon>
#include <QPixmap>

#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/opencv.hpp"

#include <thread>
#include <atomic>
#include <time.h>

#include "videolabel.h"

namespace Ui {
class VideoWindow;
}

enum class State:uint8_t {
    STOPPED = 0,
    PAUSED = 1,
    PLAYING = 2,
    REWINDING_F = 3,
    REWINDING_B = 4,
};

class VideoWindow : public QWidget
{
    Q_OBJECT

public:
    explicit VideoWindow(std::string fileName = "", QWidget *parent = 0);
    ~VideoWindow();


private slots:
    void on_b_rewind_b_pressed();

    void on_b_rewind_b_released();

    void on_b_stop_clicked();

    void on_b_previous_clicked();

    void on_b_play_clicked();

    void on_b_pause_clicked();

    void on_b_next_clicked();

    void on_b_select_clicked();

    void on_b_unselect_clicked();

    void on_b_rewind_f_pressed();

    void on_b_rewind_f_released();

private:

    void play();
    void stop();
    QPixmap mat2Pixmap(cv::Mat matImg);
    void msleep(int ms);
    void updateImage();
    void setSelection(bool enabled) ;
    void setLabel();
    void setIcons();
    void setFailedScreen();
    cv::Rect2d selectRoi();

    Ui::VideoWindow *ui;
    std::string fileName;
    cv::VideoCapture* capture;
    std::atomic<State> state;
    std::atomic<bool> isTracking;
    int frameRate;
    std::thread* playThread;
    cv::Mat currentFrame;
    QPixmap currentPixmap;
    time_t start;
    time_t end;
    cv::Ptr<cv::Tracker> tracker;
    VideoLabel* l_video;
};

#endif // VIDEOWINDOW_H
