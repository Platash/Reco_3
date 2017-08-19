#ifndef VIDEOLABEL_H
#define VIDEOLABEL_H

#include <QLabel>
#include <QPoint>
#include <QRubberBand>
#include <QPixmap>
#include <QMouseEvent>

//#include <opencv2/core/utility.hpp>
//#include <opencv2/tracking.hpp>
//#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

class VideoLabel : public QLabel {
    Q_OBJECT
signals:
    void mousePressed(const QPoint&);
public:
    VideoLabel(QWidget* parent = 0, Qt::WindowFlags f = 0):
        QLabel(parent) {

    }

    void mousePressEvent( QMouseEvent* ev );
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);

private:

    QPoint p1;
    QPoint p2;
    QRubberBand* rubberBand;
};

#endif // VIDEOLABEL_H
