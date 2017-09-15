#include "recowindow.h"
#include "ui_recowindow.h"

RecoWindow::RecoWindow(QWidget *parent):QWidget(parent), ui(new Ui::RecoWindow) {
    ui->setupUi(this);
    averageFaceLabel = nullptr;
    idFaceLabel = nullptr;
    idNameLabel = nullptr;
    setLabels();
}

RecoWindow::~RecoWindow() {
    delete ui;
    for(auto similar: similars) {
        delete similar;
    }
    delete averageFaceLabel;
    delete idFaceLabel;
    delete idNameLabel;
    delete labelDB;
    delete labelVideo;
}

void RecoWindow::setLabels() {
    labelDB = new QLabel;
    labelDB->setText("Image from database:");
    labelVideo = new QLabel;
    labelVideo->setText("Image from video: ");
    QLayout* la = this->layout();
    dynamic_cast<QGridLayout*>(la)->addWidget(labelVideo, 0, 0);
    dynamic_cast<QGridLayout*>(la)->addWidget(labelDB, 0, 1);
}

void RecoWindow::setAverageFaceLabel(QPixmap pixmap) {
    averageFaceLabel = new QLabel;
    averageFaceLabel->setPixmap(pixmap);
    QLayout* la = this->layout();
    dynamic_cast<QGridLayout*>(la)->addWidget(averageFaceLabel, 1, 0);
}

void RecoWindow::addIdNameLabel(int id) {
    idNameLabel = new QLabel;
    idNameLabel->setText("ID: " + QString::number(id));
    QLayout* la = this->layout();
    dynamic_cast<QGridLayout*>(la)->addWidget(idNameLabel, 2, 0);
}

void RecoWindow::addIdFaceLabel(QPixmap pixmap) {
    idFaceLabel = new QLabel;
    idFaceLabel->setPixmap(pixmap);
    QLayout* la = this->layout();
    dynamic_cast<QGridLayout*>(la)->addWidget(idFaceLabel, 1, 1);
}

