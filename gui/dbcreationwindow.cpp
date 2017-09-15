#include "dbcreationwindow.h"
#include "ui_dbcreationwindow.h"
#include <QFileDialog>
#include "common/common.h"

DBCreationWindow::DBCreationWindow(QWidget *parent):QWidget(parent), ui(new Ui::DBCreationWindow) {
    ui->setupUi(this);
    averageFace.init(LANDMARKS_PREDICTOR_PATH);
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

        if(prepareDatabase(image_count, makeMainAverage, makeAdditionalAverages, coef)){
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
    pathDst = QFileDialog::getExistingDirectory(this, tr("Choose Destination Directory"),
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
        if(faceDetector.detectAndCropFaces(pathSrc.toStdString() + "/" + subdirname + "/", images)) {
            for(auto& image: images) {
                Face face(image, 0);
                averageFace.getLandmarks(face);
                averageFace.alignFace(face);

                faces.push_back(face);
                croppedFaces.push_back(cropFace(face.face));
            }
            std::string path = pathDst.toStdString() + "/" + subdirname + "/";
            if(croppedFaces.size() <= maxFaceCount) {
                writeImages(croppedFaces, path, "img");
            } else {
                std::vector<cv::Mat> selectedImages;
                for(u_int i = 0; i < maxFaceCount; ++i) {
                    selectedImages.push_back(croppedFaces.at(i));
                }
                writeImages(selectedImages, path, "img");
            }
            std::vector<cv::Mat> averages;
            if(makeAverage) {
                averages.push_back(averageFace.makeAverageFace(faces));
            }

            if(makeAdditionalAverages) {

                std::vector<Face> tempFaces;
                for(u_int i = 0; i < faces.size(); ++i) {
                    tempFaces.push_back(faces.at(i));
                    if(i > 0 && i % coef == 0) {
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
