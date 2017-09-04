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
    videoWindow->setLabel();
    videoWindow->show();
}


void MainWindow::on_b_open_video_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Video File"), "/home/siobhan/UJ/Masters_stuff/video",
                                                    tr("Video Files (*.mpeg4 *.mp4 *.avi *.3gp)"));

    if(videoWindow != nullptr) {
        delete videoWindow;
        videoWindow = nullptr;
    }
    videoWindow = new VideoWindow(fileName.toStdString());
    videoWindow->setLabel();
    videoWindow->show();
}


void MainWindow::on_b_prepare_db_clicked() {
    QString pathSrc = QFileDialog::getExistingDirectory(this, tr("Choose Source Directory"), "/home/siobhan/UJ/Masters_stuff/db",
                                                 QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QString pathDst = QFileDialog::getExistingDirectory(this, tr("Choose Source Directory"), "/home/siobhan/UJ/Masters_stuff/db",
                                                 QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    recognizer.prepareDatabase(pathSrc.toStdString(), pathDst.toStdString());
}


void MainWindow::on_b_train_reco_clicked() {

}
