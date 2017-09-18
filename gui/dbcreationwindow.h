#ifndef DBCREATIONWINDOW_H
#define DBCREATIONWINDOW_H

#include <QWidget>
#include <QString>

#include "averageface.h"
#include "preprocessing.h"
#include "facedetector.h"

namespace Ui {
class DBCreationWindow;
}

class DBCreationWindow : public QWidget {
    Q_OBJECT

public:
    explicit DBCreationWindow(QWidget *parent = 0);
    ~DBCreationWindow();

private slots:
    void on_b_create_db_clicked();

    void on_b_choose_src_clicked();

    void on_b_choose_dst_clicked();

private:
    bool prepareDatabase(u_int maxFaceCount, bool makeAverage,
                         bool makeAdditionalAverages, int coef);


    Ui::DBCreationWindow *ui;
    AverageFace averageFace;
    //Preprocessing prep;
    FaceDetector faceDetector;

    bool srcChosen;
    bool dstChosen;
    QString pathSrc;
    QString pathDst;
};

#endif // DBCREATIONWINDOW_H
