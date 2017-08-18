#include "videolabel.h"

void VideoLabel::mouseMoveEvent(QMouseEvent* event){

    if(event->type() == QEvent::MouseMove){
        QPoint point = event->pos();
        painter->setP2(point);
    }
    update();
}

void VideoLabel::mouseReleaseEvent(QMouseEvent *event){
    mousePressed = false;
    update();
}
