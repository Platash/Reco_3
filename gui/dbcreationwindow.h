#ifndef DBCREATIONWINDOW_H
#define DBCREATIONWINDOW_H

#include <QWidget>
#include <QString>

#include "../reco/facerecognition.h"

namespace Ui {
class DBCreationWindow;
}

class DBCreationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DBCreationWindow(FaceRecognition* reco_, QWidget *parent = 0);
    ~DBCreationWindow();

private slots:
    void on_b_create_db_clicked();

    void on_b_choose_src_clicked();

    void on_b_choose_dst_clicked();

private:
    Ui::DBCreationWindow *ui;
    FaceRecognition* reco;

    bool srcChosen;
    bool dstChosen;
    QString pathSrc;
    QString pathDst;
};

#endif // DBCREATIONWINDOW_H
