#include "videowindow.h"
#include "ui_videowindow.h"

VideoWindow::VideoWindow(std::string fileName_, QWidget *parent):QWidget(parent), ui(new Ui::VideoWindow), fileName(fileName_) {
    capture = new cv::VideoCapture(fileName_);
    ui->setupUi(this);
    setIcons();

    if(capture->isOpened()) {
        ui->slider->setMaximum((int)capture->get(CV_CAP_PROP_FRAME_COUNT));
        frameRate = (int) capture->get(CV_CAP_PROP_FPS);
        state = State::STOPPED;
        setLabel();
        updateImage();
        tracker = cv::Tracker::create("BOOSTING");
    } else {
        setFailedScreen();
    }
}


VideoWindow::~VideoWindow()
{
    delete ui;
    capture -> release();
    delete capture;
}

void VideoWindow::setLabel() {
    l_video = new VideoLabel();
    l_video->setParent(this);
    l_video->setFixedHeight(capture->get(CV_CAP_PROP_FRAME_HEIGHT));
    l_video->setFixedWidth(capture->get(CV_CAP_PROP_FRAME_WIDTH));
    l_video->setAttribute(Qt::WA_TransparentForMouseEvents);
    QLayout* la = this->layout();
    dynamic_cast<QGridLayout*>(la)->addWidget(l_video, 0, 0);
}

//=================================================================

//              FLOW CONTROL

//================================================================

void VideoWindow::on_b_rewind_b_pressed() {
    setSelection(false);
}

void VideoWindow::on_b_rewind_b_released() {
    ui -> b_pause -> setChecked(true);
    on_b_pause_clicked();
}

void VideoWindow::on_b_rewind_f_pressed() {
    setSelection(false);
}

void VideoWindow::on_b_rewind_f_released() {
    ui->b_pause -> setChecked(true);
    ui->b_stop -> setChecked(false);
    on_b_pause_clicked();
}

void VideoWindow::on_b_stop_clicked() {
    state = State::STOPPED;
    capture -> set(CV_CAP_PROP_POS_MSEC, 0);
    updateImage();
    setSelection(true);

}

void VideoWindow::on_b_previous_clicked() {
    if(state == State::PAUSED || state == State::STOPPED) {

    }
}

void VideoWindow::on_b_play_clicked() {
    state = State::PLAYING;
    playThread = new std::thread(&VideoWindow::play, this);
    setSelection(false);
    //playThread.join();
    //play();
}

void VideoWindow::on_b_pause_clicked() {
    state = State::PAUSED;
    setSelection(true);
}

void VideoWindow::on_b_next_clicked() {
    if(state == State::PAUSED || state == State::STOPPED) {

    }
}


//==================================================================

//          SELECTION

//==================================================================


void VideoWindow::on_b_select_clicked() {
   l_video->setAttribute(Qt::WA_TransparentForMouseEvents);
}

void VideoWindow::on_b_unselect_clicked() {
    ui->b_select->setChecked(false);
}

//==================================================================

//          PRIVATE METHODS

//==================================================================


void VideoWindow::setIcons()
{
    ui->b_rewind_b->setIcon(QIcon(":/icons/rewind_b"));
    ui->b_rewind_f->setIcon(QIcon(":/icons/rewind_f"));
    ui->b_previous->setIcon(QIcon(":/icons/previous"));
    ui->b_next->setIcon(QIcon(":/icons/next"));
    ui->b_stop->setIcon(QIcon(":/icons/stop"));
    ui->b_play->setIcon(QIcon(":/icons/play"));
    ui->b_pause->setIcon(QIcon(":/icons/pause"));
}

void VideoWindow::setFailedScreen()
{
    ui->b_rewind_b->setEnabled(false);
    ui->b_rewind_f->setEnabled(false);
    ui->b_previous->setEnabled(false);
    ui->b_next->setEnabled(false);
    ui->b_stop->setEnabled(false);
    ui->b_play->setEnabled(false);
    ui->b_pause->setEnabled(false);
    ui->slider->setEnabled(false);
    l_video->setText("Failed opening video");
}

void VideoWindow::setSelection(bool selection) {
    ui->b_select->setEnabled(selection);
    ui->b_unselect->setEnabled(selection);
}

void VideoWindow::play() {
    cv::Rect2d roi;
    cv::Mat dst;
    cv::Scalar scalar(255, 0, 0);
    int delay = (1000/frameRate);
    //Ptr<Tracker> tracker = Tracker::create( "BOOSTING" );


    while (state == State::PLAYING) {
        updateImage();
        if(isTracking) {

        }
        this->msleep(delay);
    }
}



void VideoWindow::updateImage() {
    QPixmap pixmap;
    (*capture) >> currentFrame;
    if(currentFrame.rows==0 || currentFrame.cols==0) {
        state = State::STOPPED;
    }
    pixmap = mat2Pixmap(currentFrame);
    if(!pixmap.isNull()) {
        l_video->setPixmap(pixmap);
    }
}

QPixmap VideoWindow::mat2Pixmap(cv::Mat matImg) {
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

void VideoWindow::msleep(int ms) {
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}




