#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>

#include <thread>
#include <atomic>
//#include <time.h>

#include "videowindow.h"
#include "reco/facerecognition.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_b_open_video_clicked();

    void on_b_prepare_db_clicked();

    void on_b_train_reco_clicked();

private:
    Ui::MainWindow *ui;

    VideoWindow* videoWindow;
    FaceRecognition recognizer;
    std::thread* prepareDBThread;
    std::thread* trainModelThread;
};

#endif // MAINWINDOW_H
