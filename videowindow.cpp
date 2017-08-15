#include "videowindow.h"
#include "ui_videowindow.h"

VideoWindow::VideoWindow(QWidget *parent):QWidget(parent), ui(new Ui::VideoWindow) {

    ui->setupUi(this);
    ui->b_rewind_b->setIcon(QIcon(":/icons/rewind_b"));
    ui->b_rewind_f->setIcon(QIcon(":/icons/rewind_f"));
    ui->b_previous->setIcon(QIcon(":/icons/previous"));
    ui->b_next->setIcon(QIcon(":/icons/next"));
    ui->b_stop->setIcon(QIcon(":/icons/stop"));
    ui->b_play->setIcon(QIcon(":/icons/play"));
    ui->b_pause->setIcon(QIcon(":/icons/pause"));
}


VideoWindow::~VideoWindow()
{
    delete ui;
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
