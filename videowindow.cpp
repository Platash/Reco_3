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
        frameRate = (int) capture->get(CV_CAP_PROP_FPS);
        state = State::STOPPED;
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
    ui->b_play->setChecked(false);
    (*capture) >> currentFrame;
    updateImage();
    isTracking = false;
    if(processor.getFaceCount() > 0) {
            askForAverageFace();
    }
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
    l_video->setText("Failed opening video");
}

void VideoWindow::play() {
    int delay = (1000/frameRate);
    // cv::Mat equalized;
    // cv::Mat equalized_color;
    while (state == State::PLAYING) {
        (*capture) >> currentFrame;
        if(currentFrame.rows==0 || currentFrame.cols==0) {
            on_b_stop_clicked();
            return;
        };
        if(isTracking) {
            //equalized = prep.equalize(currentFrame);
            //cvtColor(equalized, equalized_color,CV_GRAY2RGB);
            if(myTracker.track(&currentFrame)) {
                //if(processor.pickFace(currentFrame, myTracker.getRoi())) {
                    //updateSmallFaces(processor.getFaces());
                //}
            }
        } else {
            this->msleep(delay);
        }
        updateImage();
    }
}

void VideoWindow::askForAverageFace() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Average Face");
    msgBox.setText("Create Average Face from picked face images?");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    if(msgBox.exec() == QMessageBox::Yes){
        processor.processAverageFace();
    }
}

/*void VideoWindow::updateSmallFaces(std::vector<Face> &faces) {
    for(int i = 0; i < faces.size(); ++i) {
        QPixmap pixmap = mat2Pixmap(faces.at(i).face);
        if(NULL == smallFaces.at(i)) {
            QLabel temp;
            temp.setFixedHeight(SMALL_FACE_SIZE);
            temp.setFixedWidth(SMALL_FACE_SIZE);
            temp.setPixmap(pixmap);
            smallFaces.at(i) = temp;
            QLayout* la = this->layout();
            dynamic_cast<QGridLayout*>(la)->addWidget(smallFaces.at(i), 2, i);

        } else {

        }
    }
}*/

void VideoWindow::showRecoWindow(QPixmap pixmap) {
    if(recoWindow != nullptr) {
        delete recoWindow;
        recoWindow = nullptr;
    }
    recoWindow = new RecoWindow();
    recoWindow->setLabel(pixmap);
    recoWindow->show();
}

void VideoWindow::updateImage() {
    QPixmap pixmap = mat2Pixmap(currentFrame);
    if(!pixmap.isNull()) {
        l_video->setPixmap(pixmap);
    }
}

void VideoWindow::msleep(int ms) {
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}




