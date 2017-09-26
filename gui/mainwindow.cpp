#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    videoWindow = nullptr;
    prepareDBThread = nullptr;
    trainModelThread = nullptr;
    pathToDB = QString::fromStdString(DEFAULT_DB);
    ui->l_db->setText("Current DB:" + pathToDB);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete videoWindow;
    delete dbWindow;
}

void MainWindow::on_b_open_video_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Video File"),
                                                    "/home/siobhan/UJ/Masters_stuff/db/vid",
                                                    tr("Video Files (*.mpeg4 *.mp4 *.avi *.3gp)"));

    if(!fileName.isEmpty() && !fileName.isNull()) {
        if(videoWindow != nullptr) {
            delete videoWindow;
            videoWindow = nullptr;
        }
        videoWindow = new VideoWindow(fileName.toStdString(),
                                      &recognizer, pathToDB);
        videoWindow->setLabel();
        videoWindow->show();
    }
}


void MainWindow::on_b_prepare_db_clicked() {    
    //delete dbWindow;

    dbWindow = new DBCreationWindow();
    dbWindow->show();
}


void MainWindow::on_b_train_reco_clicked() {
    QString pathSrc = QFileDialog::getExistingDirectory(this, tr("Choose Sourse Directory"),
                                                        "/home/siobhan/UJ/Masters_stuff/db",
                                                        QFileDialog::ShowDirsOnly |
                                                        QFileDialog::DontResolveSymlinks);

    if(!pathSrc.isEmpty() && !pathSrc.isNull()) {
        QLabel* info = findChild<QLabel*>("l_info");
        info->setText("Training the model...");
        info->repaint();
        recognizer.train(pathSrc.toStdString());
        info->setText("Model is ready");
    }
}

void MainWindow::on_b_save_reco_clicked() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Open Video File"),
                                                    "/home/siobhan/UJ/Masters_stuff/db");
    if(!fileName.isEmpty() && ! fileName.isNull()) {
        recognizer.saveModel(fileName.toStdString());
    }
}

void MainWindow::on_b_load_reco_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Video File"),
                                                    "/home/siobhan/UJ/Masters_stuff/db");
    if(!fileName.isEmpty() && !fileName.isNull()) {
        recognizer.readModelFromFile(fileName.toStdString());
    }
}

void MainWindow::on_b_choose_db_clicked() {
    QString pathSrc = QFileDialog::getExistingDirectory(this, tr("Choose Database Directory"),
                                                        "/home/siobhan/UJ/Masters_stuff/db",
                                                        QFileDialog::ShowDirsOnly |
                                                        QFileDialog::DontResolveSymlinks);
    if(!pathSrc.isEmpty() && !pathSrc.isNull()) {
        pathToDB = pathSrc;
        ui->l_db->setText("Current DB: " + pathToDB);
    }
}
