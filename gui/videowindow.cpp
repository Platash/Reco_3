#include "videowindow.h"
#include "ui_videowindow.h"
#include "common/common.h"
#include "map"

#include <QMessageBox>
#include <QResource>
#include <QIcon>

VideoWindow::VideoWindow(std::string fileName_, FaceRecognition* reco_,
                         QString pathToDB_, QWidget* parent):
    QWidget(parent), ui(new Ui::VideoWindow), fileName(fileName_), reco(reco_),
    pathToDB(pathToDB_), faceCount(20), worstScore(0){

    capture = new cv::VideoCapture(fileName_);
    recoWindow = nullptr;
    //capture->set(CV_CAP_PROP_FRAME_WIDTH, 640);
    // capture->set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    ui->setupUi(this);
    setIcons();
    l_video = new VideoLabel();
    setInfo("");
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
    if(recoWindow != nullptr) {
        delete recoWindow;
    }
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

void VideoWindow::setInfo(std::string text) {
    ui->l_info->setText(QString::fromStdString(text));
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
    if(faces.size() > 0) {
        if(askForAverageFace()) {
            setInfo("Creating Average Face...");
            if(processAverageFace()) {
                setInfo("");
                if(reco->isTrained) {
                    int id = reco->predict(averageFace);
                    showRecoWindow(cvMat2qImage(averageFace), id);
                } else {
                    setInfo("Recognizer is not trained. Train it or load from a file.");
                }
            }
        } else {
            if(reco->isTrained) {
                cv::Mat faceToShow;
                int id = getPrediction(faceToShow);
                showRecoWindow(cvMat2qImage(faceToShow), id);
            } else {
                setInfo("Recognizer is not trained. Train it or load from a file.");
            }
        }
    }
    faces.clear();
}


void VideoWindow::on_b_play_clicked() {
    state = State::PLAYING;
    playThread = new std::thread(&VideoWindow::play, this);
    setInfo("");
}

void VideoWindow::on_b_pause_clicked() {
    state = State::PAUSED;

}

void VideoWindow::on_b_stop_tracking_clicked() {
    isTracking = false;
}


//==================================================================

//          SELECTION

//==================================================================

void VideoWindow::setSelection(QPoint p1_, QPoint p2_) {
    write_log("set selection");
    p1.x = p1_.x();
    p1.y = p1_.y();
    p2.x = p2_.x();
    p2.y = p2_.y();
    TrackerType trackerType;
    QList<QRadioButton *> allButtons = ui->gb_tracker_type->findChildren<QRadioButton *>();
    for(auto button: allButtons) {
        if(button->isChecked()) {
            QString name = button->objectName();

            if(name == "KCF") {
                trackerType = TrackerType::KCF;
            } else if(name == "GOTURN") {
                trackerType = TrackerType::GOTURN;
            } else if(name == "TLD") {
                trackerType = TrackerType::TLD;
            } else if(name == "MEDIANFLOW") {
                trackerType = TrackerType::MEDIANFLOW;
            } else if(name == "MIL") {
                trackerType = TrackerType::MIL;
            } else if(name == "BOOSTING") {
                trackerType = TrackerType::BOOSTING;
            } else {
                trackerType = TrackerType::KCF;
            }
            break;
        }
    }

    if(!isTracking) {
        isTracking = true;
        myTracker.startTracking(trackerType, currentFrame,
                                p1_.x(), p1_.y(), p2_.x(), p2_.y());
    } else {
        myTracker.modifyTracking(p1_.x(), p1_.y(), p2_.x(), p2_.y());
    }

}

//==================================================================

//          PRIVATE METHODS

//==================================================================

int VideoWindow::getPrediction(cv::Mat& faceToShow) {
    std::vector<cv::Mat> croppedFaces;
    std::map<int, int> ids;
    for(auto& face: faces) {
        averageFaceCreator.getLandmarks(face);
        averageFaceCreator.alignFace(face);
        cv::Mat croppedFace = cropFace(face.face);
        int id = reco->predict(croppedFace);
        face.id = id;
        write_log("getPredition: " + std::to_string(id));
        croppedFaces.push_back(croppedFace);
        std::map<int, int>::iterator it = ids.find(id);
        if(it == ids.end()) {
            ids.insert(std::pair<int, int>(id, 1));
        } else {
            it->second = it->second + 1;
        }

    }
    int maxID = 0;
    int maxCount = 0;
    for(auto element: ids) {
        if(element.second > maxCount) {
            maxID = element.first;
            maxCount = element.second;
        }
        write_log("second: " + std::to_string(element.second));
        write_log("first: " + std::to_string(element.first));
    }
    for(auto face: faces) {
        if(face.id == maxID) {
            faceToShow = cropFace(face.face);
            break;
        }
    }
    write_log("max_id: " + std::to_string(maxID));
    return maxID;
}



void VideoWindow::setIcons()
{
    ui->b_rewind_b->setIcon(QIcon(":/icons/rewind_b"));
    ui->b_rewind_f->setIcon(QIcon(":/icons/rewind_f"));
    ui->b_stop->setIcon(QIcon(":/icons/stop"));
    ui->b_play->setIcon(QIcon(":/icons/play"));
    ui->b_pause->setIcon(QIcon(":/icons/pause"));
}

void VideoWindow::setFailedScreen()
{
    ui->b_rewind_b->setEnabled(false);
    ui->b_rewind_f->setEnabled(false);
    ui->b_stop->setEnabled(false);
    ui->b_play->setEnabled(false);
    ui->b_pause->setEnabled(false);
    l_video->setText("Failed opening video");
}

void VideoWindow::play() {
    int delay = (1000/frameRate);
    cv::Mat equalized;
    cv::Rect2d bestRoi;
    while (state == State::PLAYING) {
        (*capture) >> currentFrame;
        if(currentFrame.rows==0 || currentFrame.cols==0) {
            on_b_stop_clicked();
            return;
        };
        if(isTracking) {
            equalized = prep.equalizeColor(currentFrame);
            //cvtColor(equalized, equalized_color,CV_GRAY2RGB);
            if(myTracker.track(equalized)) {
                if(pickFace(equalized.clone(), myTracker.getRoi(), bestRoi)) {
                    rectangle(currentFrame, bestRoi, cv::Scalar(255, 255, 0), 2, 1);
                }
                rectangle(currentFrame, myTracker.getRoi(), cv::Scalar(255, 0, 0), 2, 1);
            }
        } else {
            this->msleep(delay);
        }
        updateImage();
    }
}

bool VideoWindow::pickFace(cv::Mat image, cv::Rect2d& roi, cv::Rect2d& bestRoi) {
    cv::Rect bestFaceRoi;
    if(faceDetector.getBestFace(image, roi, bestFaceRoi)) {
        write_log("got best fase ");
        cv::Mat crop = image(bestFaceRoi);
        // cv::imwrite("/home/siobhan/UJ/Masters_stuff/results/tracked_faces/img_" + to_string(rand() % 300 + 19850) + ".jpg", crop);

        bestRoi = bestFaceRoi;
        double score = qualityAssessment.getScore(crop);
        write_log("quality: " + std::to_string(score));
        if(faces.size() < faceCount) {
            faces.push_back(Face(crop, score));
            //cv::imwrite("/home/siobhan/UJ/Masters_stuff/results/tracked_faces/img1_" + to_string(rand() % 300 + 19850) + ".jpg", crop);
            return true;

        } else if(score > worstScore) {
            std::sort(faces.begin(), faces.end());
            faces.at(faceCount - 1) = Face(crop, score);
            //cv::imwrite("/home/siobhan/UJ/Masters_stuff/results/tracked_faces/img2_" + to_string(rand() % 300 + 19850) + ".jpg", crop);
            return true;
        }
        return true;
    }
    return false;
}

bool VideoWindow::processAverageFace() {
    int i = 0;
    for(auto& face: faces) {
        cv::imwrite("/home/siobhan/UJ/Masters_stuff/results/best/img_" + to_string(i) + ".jpg", face.face);
        averageFaceCreator.getLandmarks(face);
        averageFaceCreator.alignFace(face);
        ++i;
    }
    averageFace = averageFaceCreator.makeAverageFace(faces);
    cv::imwrite("/home/siobhan/UJ/Masters_stuff/results/best/img_best.jpg", averageFace);
    std::cout << "finish processAverageFace" << std::endl;
    return true;
}

bool VideoWindow::askForAverageFace() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Average Face");
    msgBox.setText("Create Average Face from picked face images?");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes);
    msgBox.addButton(QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    return(msgBox.exec() == QMessageBox::Yes);

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

void VideoWindow::showRecoWindow(QImage qimage, int id) {
    write_log("showRecoWindow, id: " + std::to_string(id));

    if(recoWindow != nullptr && recoWindow != NULL) {
        delete recoWindow;
        recoWindow = nullptr;
    }
    QPixmap faceFromDB;
    std::string fullPath = pathToDB.toStdString()+std::to_string(id)+ "/" +DEFAULT_IMG_NAME+ "0.jpg";
    write_log(fullPath);
    bool result = faceFromDB.load(QString::fromStdString(fullPath));
    if(result) {
        write_log("Loadede image");
    } else {
        write_log("NO image");
    }

    recoWindow = new RecoWindow();
    recoWindow->setAverageFaceLabel(QPixmap::fromImage(qimage));
    recoWindow->addIdNameLabel(id);
    recoWindow->addIdFaceLabel(faceFromDB);
    recoWindow->show();
}

void VideoWindow::updateImage() {
    QPixmap pixmap;
    pixmap = mat2Pixmap(currentFrame);
    if(!pixmap.isNull()) {
        l_video->setPixmap(pixmap);
    }
}

void VideoWindow::msleep(int ms) {
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
}
