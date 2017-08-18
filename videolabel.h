#ifndef VIDEOLABEL_H
#define VIDEOLABEL_H

#include <QLabel>
#include <QPoint>

class VideoLabel : public QLabel {
    Q_OBJECT
signals:
    void mousePressed(const QPoint&);
public:
    VideoLabel(QWidget* parent = 0, Qt::WindowFlags f = 0);
    VideoLabel(const QString& text, QWidget* parent = 0, Qt::WindowFlags f = 0);

     void mousePressEvent( QMouseEvent* ev );

private:
     QPoint p1;
     QPoint p2;
};

#endif // VIDEOLABEL_H
