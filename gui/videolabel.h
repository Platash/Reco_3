#ifndef VIDEOLABEL_H
#define VIDEOLABEL_H

#include <QLabel>
#include <QPoint>
#include <QRubberBand>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>

#include <opencv2/highgui.hpp>

#include "videowindow.h"

class VideoLabel : public QLabel {
    Q_OBJECT
signals:
    void mousePressed(const QPoint&);
    void selectionSet(QPoint p1, QPoint p2);

public:
    explicit VideoLabel(QWidget* parent =0, Qt::WindowFlags f = Qt::WindowFlags());

    ~VideoLabel();
    friend class VideoWindow;

    void mousePressEvent( QMouseEvent* ev ) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private:

    QPoint p1;
    QPoint p2;
   // std::atomic<bool> drawSelection{false};
    //QPainter* painter;
   bool buttonPressed;
    QRubberBand* rubberBand;
};

#endif // VIDEOLABEL_H
