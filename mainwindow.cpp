#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    videoWindow = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_Video_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Video File"), "/home/siobhan/UJ/Masters_stuff/video",
                                                    tr("Video Files (*.mpeg4 *.mp4 *.avi *.3gp)"));

    if(videoWindow != nullptr) {
        delete videoWindow;
        videoWindow = nullptr;
    }
    videoWindow = new VideoWindow(fileName.toStdString());
    videoWindow->show();
}
