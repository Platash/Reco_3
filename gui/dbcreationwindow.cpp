#include "dbcreationwindow.h"
#include "ui_dbcreationwindow.h"
#include <QFileDialog>
#include "common/common.h"

#include <algorithm>
#include <random>

DBCreationWindow::DBCreationWindow(QWidget *parent):QWidget(parent),
    ui(new Ui::DBCreationWindow) {

    ui->setupUi(this);
    averageFace.init(getLocalPath() + LANDMARKS_PREDICTOR_PATH);
    dstChosen = false;
    srcChosen = false;
}

DBCreationWindow::~DBCreationWindow() {
    delete ui;
}

void DBCreationWindow::on_b_create_db_clicked() {
    if(srcChosen && dstChosen) {
        ui->l_info->setText(QString::fromStdString("Preparing the database..."));
        repaint();
        int image_count = ui->s_image_count->value();
        bool makeMainAverage = ui->c_create_average->isChecked();
        bool makeAdditionalAverages = ui->c_create_averages->isChecked();
        int coef = ui->s_average_count->value();

        if(prepareDatabase(image_count, makeMainAverage,
                           makeAdditionalAverages, coef)){

            ui->l_info->setText("DB is ready");
        }

        close();
    }
}

void DBCreationWindow::on_b_choose_src_clicked() {
    pathSrc = QFileDialog::getExistingDirectory(this, tr("Choose Source Directory"),
                                                "/home/siobhan/UJ/Masters_stuff/db",
                                                QFileDialog::ShowDirsOnly |
                                                QFileDialog::DontResolveSymlinks |
                                                QFileDialog::DontUseNativeDialog);

    if(!pathSrc.isEmpty() && !pathSrc.isNull()) {
        ui->l_src->setText(pathSrc);
        srcChosen = true;
    }
}

void DBCreationWindow::on_b_choose_dst_clicked() {
    pathDst = QFileDialog::getExistingDirectory(this,
                                                tr("Choose Destination Directory"),
                                                "/home/siobhan/UJ/Masters_stuff/db",
                                                QFileDialog::ShowDirsOnly |
                                                QFileDialog::DontResolveSymlinks |
                                                QFileDialog::DontUseNativeDialog);

    if(!pathDst.isEmpty() && !pathDst.isNull()) {
        ui->l_dst->setText(pathDst);
        dstChosen = true;
    }
}

bool DBCreationWindow::prepareDatabase(u_int maxFaceCount, bool makeAverage,
                                       bool makeAdditionalAverages, int coef) {
    write_log("PrepareImages() ");
    std::vector<std::string> subdirnames ;
    int subdirCount = readSubdirNames(subdirnames, pathSrc.toStdString());
    std::cout << subdirCount << std::endl;
    std::cout << pathSrc.toStdString() << std::endl;

    for(auto subdirname: subdirnames) {
        write_log(subdirname);
        std::vector<cv::Mat> images;

        std::vector<Face> faces;
        std::vector<cv::Mat> croppedFaces;
        int max;
        if(faceDetector.detectAndCropFaces(pathSrc.toStdString() + "/" +
                                           subdirname + "/", images)) {
            auto rng = std::default_random_engine {};
            std::shuffle(std::begin(images), std::end(images), rng);
            max = images.size() > maxFaceCount ? maxFaceCount : images.size();
            for(u_int i = 0; i < max; ++i) {
                Face face(images.at(i), 0);
                averageFace.getLandmarks(face);
                averageFace.alignFace(face);

                faces.push_back(face);
                croppedFaces.push_back(cropFace(face.face));
            }
            std::string path = pathDst.toStdString() + "/" + subdirname + "/";
            writeImages(croppedFaces, path, "img");
            std::vector<cv::Mat> averages;
            if(makeAverage) {
                averages.push_back(averageFace.makeAverageFace(faces));
            }
            write_log("Done common average");

            if(makeAdditionalAverages) {
                std::vector<Face> tempFaces;
                for(u_int i = 0; i < faces.size(); ++i) {
                    tempFaces.push_back(faces.at(i));
                    write_log("weird");
                    if((i > 0) && ((i+1) % coef == 0)) {
                        averages.push_back(averageFace.makeAverageFace(tempFaces));
                        tempFaces.clear();
                    }
                }
            }
            if(averages.size() > 0) {
                writeImages(averages, path, "avg");
            }
        }
    }
    return true;
}
