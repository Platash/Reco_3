#include "videowindow.h"
#include "ui_videowindow.h"

VideoWindow::VideoWindow(std::string fileName_, QWidget *parent):QWidget(parent), ui(new Ui::VideoWindow), fileName(fileName_) {

    player = new Player();
    capture = new VideoCapture(fileName_);
    ui->setupUi(this);
    ui->b_rewind_b->setIcon(QIcon(":/icons/rewind_b"));
    ui->b_rewind_f->setIcon(QIcon(":/icons/rewind_f"));
    ui->b_previous->setIcon(QIcon(":/icons/previous"));
    ui->b_next->setIcon(QIcon(":/icons/next"));
    ui->b_stop->setIcon(QIcon(":/icons/stop"));
    ui->b_play->setIcon(QIcon(":/icons/play"));
    ui->b_pause->setIcon(QIcon(":/icons/pause"));


    if(capture->isOpened()) {
         //ui->slider->setMaximum(player->getFrameCount());
        //ui->l_video->setPixmap();
    } else {
        ui->b_rewind_b->setEnabled(false);
        ui->b_rewind_f->setEnabled(false);
        ui->b_previous->setEnabled(false);
        ui->b_next->setEnabled(false);
        ui->b_stop->setEnabled(false);
        ui->b_play->setEnabled(false);
        ui->b_pause->setEnabled(false);
        ui->slider->setEnabled(false);
        ui->l_video->setText("Failed opening video");

    }
}


VideoWindow::~VideoWindow()
{
    delete ui;
    delete player;
    delete capture;
}

void VideoWindow::updatePlayerVideo(QPixmap img) {
    if(img.isNull()) {
        ui->l_video->setPixmap(img);
    }
}



void VideoWindow::on_b_rewind_b_pressed() {

}

void VideoWindow::on_b_rewind_b_released() {

}

void VideoWindow::on_b_stop_clicked() {

}

void VideoWindow::on_b_previous_clicked() {

}

void VideoWindow::on_b_play_clicked() {
    player->play();
}

void VideoWindow::on_b_pause_clicked() {

}

void VideoWindow::on_b_next_clicked() {

}

void VideoWindow::on_b_select_clicked() {

}

void VideoWindow::on_b_unselect_clicked() {
    ui->b_select->setChecked(false);
}
