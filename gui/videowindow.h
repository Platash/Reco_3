#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <QWidget>
#include <QPixmap>
#include <QString>
#include <QLabel>

#include <opencv2/videoio.hpp>
#include <opencv2/core/core.hpp>

#include <thread>
#include <atomic>
#include <time.h>

#include "videolabel.h"
#include "peopletracker.h"
#include "facedetector.h"
#include "preprocessing.h"
#include "common/common.h"
#include "recowindow.h"
#include "reco/facerecognition.h"
#include "qualityassessment.h"

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
    explicit VideoWindow(std::string fileName, FaceRecognition* reco_,
                         QString pathToDB_, QWidget *parent = 0);
    ~VideoWindow();

    void setLabel();
    void setInfo(string text);
    friend class VideoLabel;

    bool processAverageFace();
    bool pickFace(cv::Mat image, cv::Rect2d &roi, cv::Rect2d &bestRoi);

public slots:
    void setSelection(QPoint p1, QPoint p2);

private slots:
    void on_b_rewind_b_pressed();

    void on_b_rewind_b_released();

    void on_b_stop_clicked();

    void on_b_play_clicked();

    void on_b_pause_clicked();

    void on_b_rewind_f_pressed();

    void on_b_rewind_f_released();

    void on_b_stop_tracking_clicked();

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
    int getPrediction(cv::Mat &faceToShow);

    Ui::VideoWindow *ui;
    QPixmap currentPixmap;
    QString pathToDB;
    QLabel faceFromDB;
    cv::VideoCapture* capture;
    cv::Mat currentFrame;
    cv::Point p1;
    cv::Point p2;
    cv::Mat averageFace;

    std::vector<QLabel> smallFaces;

    std::vector<Face> faces;
    std::atomic<State> state;
    std::atomic<bool> isTracking;
    std::thread* playThread;
    std::string fileName;
    std::atomic<int> faceCount;
    time_t start;
    time_t end;
    int frameRate;
    double worstScore;

    AverageFace averageFaceCreator;
    Preprocessing prep;
    //ImageProcessor processor;
    FaceRecognition* reco;
    PeopleTracker myTracker;
    FaceDetector faceDetector;
    QualityAssessment qualityAssessment;
    RecoWindow* recoWindow;
    VideoLabel* l_video;



};

#endif // VIDEOWINDOW_H
