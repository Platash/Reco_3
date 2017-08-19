#ifndef VIDEOLABEL_H
#define VIDEOLABEL_H

#include <QLabel>
#include <QPoint>
#include <QRubberBand>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>

//#include <opencv2/core/utility.hpp>
//#include <opencv2/tracking.hpp>
//#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <thread>
#include <atomic>
#include <time.h>

#include "videowindow.h"

class VideoLabel : public QLabel {
    Q_OBJECT
signals:
    void mousePressed(const QPoint&);
public:
    explicit VideoLabel(QWidget* parent =0, Qt::WindowFlags f = Qt::WindowFlags());

    ~VideoLabel();

    void mousePressEvent( QMouseEvent* ev ) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent *event) override;

private:

    std::atomic<int> p1_x;
    std::atomic<int> p1_y;
    std::atomic<int> p2_x;
    std::atomic<int> p2_y;
    std::atomic<bool> drawSelection{false};
    QPainter* painter;
    bool buttonPressed;
    //QRubberBand* rubberBand;
};

#endif // VIDEOLABEL_H
