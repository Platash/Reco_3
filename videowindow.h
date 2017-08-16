#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <QWidget>
#include <QResource>
#include <QIcon>
#include <QPixmap>

#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include "player.h"

namespace Ui {
class VideoWindow;
}

class VideoWindow : public QWidget
{
    Q_OBJECT

public:
    explicit VideoWindow(std::string fileName = "", QWidget *parent = 0);
    ~VideoWindow();

    void updatePlayerVideo(QPixmap img);

private slots:
    void on_b_rewind_b_pressed();

    void on_b_rewind_b_released();

    void on_b_stop_clicked();

    void on_b_previous_clicked();

    void on_b_play_clicked();

    void on_b_pause_clicked();

    void on_b_next_clicked();

    void on_b_select_clicked();

    void on_b_unselect_clicked();

private:
    Ui::VideoWindow *ui;
    Player* player;
    std::string fileName;
    cv::VideoCapture* capture;
};

#endif // VIDEOWINDOW_H
