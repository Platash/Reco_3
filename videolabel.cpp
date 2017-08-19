#include "videolabel.h"

void VideoLabel::mouseMoveEvent(QMouseEvent* event){
    p2 = event->pos();
    rubberBand->setGeometry(QRect(p1, p2).normalized());


}

void VideoLabel::mousePressEvent(QMouseEvent *event) {
    p1 = event->pos();
    p2 = event->pos();
    if(!rubberBand) {
        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    }
    rubberBand->setGeometry(QRect(p1, QSize()));
    rubberBand->show();
}

void VideoLabel::mouseReleaseEvent(QMouseEvent *event){
    p2 = event->pos();
    rubberBand->setGeometry(QRect(p1, p2).normalized());
    this->parent()->
}

