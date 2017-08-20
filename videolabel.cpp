#include "videolabel.h"

VideoLabel::VideoLabel(QWidget* parent, Qt::WindowFlags f):QLabel(parent) {

    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
}

VideoLabel::~VideoLabel() {
    delete rubberBand;
}


//========================================================================

//          EVENTS

//========================================================================

void VideoLabel::mouseMoveEvent(QMouseEvent* event){
    if(buttonPressed) {
        p2 = event->pos();
        rubberBand->setGeometry(QRect(p1, p2).normalized());
    }

}

void VideoLabel::mousePressEvent(QMouseEvent *event) {
    buttonPressed = true;
    p1 = event-> pos();
    p2 = event->pos();
    rubberBand->setGeometry(QRect(p1, QSize()));
    rubberBand->show();
}

void VideoLabel::mouseReleaseEvent(QMouseEvent *event){
    p2 = event->pos();
    buttonPressed = false;
    rubberBand->hide();
    emit selectionSet(p1, p2);
}


