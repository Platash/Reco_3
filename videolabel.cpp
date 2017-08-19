#include "videolabel.h"

VideoLabel::VideoLabel(QWidget* parent, Qt::WindowFlags f):QLabel(parent) {
    painter = nullptr;
}

VideoLabel::~VideoLabel() {
    if(painter != nullptr) {
        delete painter;
    }
}


//========================================================================

//          EVENTS

//========================================================================

void VideoLabel::mouseMoveEvent(QMouseEvent* event){
    if(buttonPressed) {
        p2_x = event->x();
        p2_y = event->y();

    }

   // rubberBand->setGeometry(QRect(p1, p2).normalized());


}

void VideoLabel::mousePressEvent(QMouseEvent *event) {
    if(painter == nullptr) {
        painter = new QPainter();
        painter->begin(this);
    }
    buttonPressed = true;
    p1_x = event->x();
    p1_y = event->y();

    p2_x = event->x();
    p2_y = event->y();
    //rubberBand->setGeometry(QRect(p1, QSize()));
    //rubberBand->show();
}

void VideoLabel::mouseReleaseEvent(QMouseEvent *event){
    p2_x = event->x();
    p2_y = event->y();
    buttonPressed = false;
    //rubberBand->setGeometry(QRect(p1, p2).normalized());


    QObject* object = this->parent();
    dynamic_cast<VideoWindow*>(object)->isTracking = true;
}

void VideoLabel::paintEvent(QPaintEvent *event) {
    QLabel::paintEvent(event);
    if(buttonPressed) {
         painter->drawRect(QRect(QPoint(p1_x, p1_y), QPoint(p2_x, p2_y)));
    }

}
