#ifndef PLAYER_H
#define PLAYER_H

#include <QMutex>
#include <QThread>
#include <QImage>
#include <QWaitCondition>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/videoio.hpp"

#include <thread>
#include <atomic>

using namespace cv;

enum class State:uint8_t {
    STOPPED = 0,
    PAUSED = 1,
    PLAYING = 2,
    REWINDING_F = 3,
    REWINDING_B = 4,
};

class Player {

 private:
    Mutex mutex;
    QWaitCondition condition;
    Mat frame;
    int frameRate;
    VideoCapture capture;
    Mat RGBframe;
    QImage img;

 public:
    Player();
    ~Player();

    void play();
    void stop();

    std::atomic<State> state;
};

#endif // PLAYER_H
