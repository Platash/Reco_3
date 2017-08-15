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
    if(videoWindow != nullptr) {
        delete videoWindow;
        videoWindow = nullptr;
    }
    videoWindow = new VideoWindow;
    videoWindow->show();
}
