#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    videoWindow = nullptr;
    prepareDBThread = nullptr;
    trainModelThread = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete videoWindow;
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
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog);
    QString pathDst = QFileDialog::getExistingDirectory(this, tr("Choose Destination Directory"), "/home/siobhan/UJ/Masters_stuff/db",
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks | QFileDialog::DontUseNativeDialog);

    QLabel* info = findChild<QLabel*>("l_info");
    info->setText("Prepearing the database...");
    info->repaint();
    recognizer.prepareDatabase(pathSrc.toStdString(), pathDst.toStdString());
    info->setText("DB is ready");
    //prepareDBThread = new std::thread(&FaceRecognition::prepareDatabase, recognizer, pathSrc.toStdString(), pathDst.toStdString());

}


void MainWindow::on_b_train_reco_clicked() {
    QString pathSrc = QFileDialog::getExistingDirectory(this, tr("Choose Sourse Directory"),
                                                        "/home/siobhan/UJ/Masters_stuff/db",
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    QLabel* info = findChild<QLabel*>("l_info");
    info->setText("Training the model...");
    info->repaint();
    recognizer.train(pathSrc.toStdString());
    info->setText("Model is ready");
}

void MainWindow::on_b_save_reco_clicked() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Open Video File"),
                                                    "/home/siobhan/UJ/Masters_stuff/db");
    recognizer.saveModel(fileName.toStdString());
}

void MainWindow::on_b_load_reco_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Video File"),
                                                    "/home/siobhan/UJ/Masters_stuff/db");
    recognizer.readModelFromFile(fileName.toStdString());
}
