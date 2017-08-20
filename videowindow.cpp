#include "videowindow.h"
#include "ui_videowindow.h"

VideoWindow::VideoWindow(std::string fileName_, QWidget *parent):QWidget(parent), ui(new Ui::VideoWindow), fileName(fileName_) {
    capture = new cv::VideoCapture(fileName_);
    capture->set(CV_CAP_PROP_FRAME_WIDTH, 640);
    capture->set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    ui->setupUi(this);
    setIcons();
    l_video = new VideoLabel();
    if(capture->isOpened()) {
        ui->slider->setMaximum((int)capture->get(CV_CAP_PROP_FRAME_COUNT));
        frameRate = (int) capture->get(CV_CAP_PROP_FPS);
        state = State::STOPPED;
        tracker = cv::Tracker::create("BOOSTING");
        isTracking = false;
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
    l_video->setParent(this);
    l_video->setFixedHeight(capture->get(CV_CAP_PROP_FRAME_HEIGHT));
    l_video->setFixedWidth(capture->get(CV_CAP_PROP_FRAME_WIDTH));
    // l_video->setAttribute(Qt::WA_TransparentForMouseEvents);
    QLayout* la = this->layout();
    dynamic_cast<QGridLayout*>(la)->addWidget(l_video, 0, 0);
    connect(l_video, &VideoLabel::selectionSet,
            this, &VideoWindow::setSelection);
    (*capture) >> currentFrame;
    updateImage();
}

//=================================================================

//              FLOW CONTROL

//================================================================

void VideoWindow::on_b_rewind_b_pressed() {

}

void VideoWindow::on_b_rewind_b_released() {
    ui -> b_pause -> setChecked(true);
    on_b_pause_clicked();
}

void VideoWindow::on_b_rewind_f_pressed() {

}

void VideoWindow::on_b_rewind_f_released() {
    ui->b_pause -> setChecked(true);
    ui->b_stop -> setChecked(false);
    on_b_pause_clicked();
}

void VideoWindow::on_b_stop_clicked() {
    state = State::STOPPED;
    capture -> set(CV_CAP_PROP_POS_MSEC, 0);
    (*capture) >> currentFrame;
    updateImage();
    isTracking = false;

}

void VideoWindow::on_b_previous_clicked() {
    if(state == State::PAUSED || state == State::STOPPED) {

    }
}

void VideoWindow::on_b_play_clicked() {
    state = State::PLAYING;
    playThread = new std::thread(&VideoWindow::play, this);

}

void VideoWindow::on_b_pause_clicked() {
    state = State::PAUSED;

}

void VideoWindow::on_b_next_clicked() {
    if(state == State::PAUSED || state == State::STOPPED) {

    }
}


//==================================================================

//          SELECTION

//==================================================================


void VideoWindow::on_b_unselect_clicked() {
    isTracking = false;
}

void VideoWindow::setSelection(QPoint p1_, QPoint p2_) {
    std::cout << "set selection" << std::endl;
    p1.x = p1_.x();
    p1.y = p1_.y();
    p2.x = p2_.x();
    p2.y = p2_.y();
    if(!isTracking) {
        isTracking = true;
        myTracker.startTracking("BOOSTING", &currentFrame,
                                p1_.x(), p1_.y(), p2_.x(), p2_.y());
    } else {
        myTracker.modifyTracking(p1_.x(), p1_.y(), p2_.x(), p2_.y());
    }

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

void VideoWindow::play() {
    int delay = (1000/frameRate);

    while (state == State::PLAYING) {
        (*capture) >> currentFrame;
        if(currentFrame.rows==0 || currentFrame.cols==0) {
            on_b_stop_clicked();
            return;
        }
        if(isTracking) {
            myTracker.track(&currentFrame);
        } else {
            this->msleep(delay);
        }
        updateImage();

    }
}

void VideoWindow::updateImage() {
    QPixmap pixmap;
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




