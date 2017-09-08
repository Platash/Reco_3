#include "recowindow.h"
#include "ui_recowindow.h"

RecoWindow::RecoWindow(QWidget *parent):QWidget(parent), ui(new Ui::RecoWindow) {
    ui->setupUi(this);
    averageFaceLabel = nullptr;
    idFaceLabel = nullptr;
    idNameLabel = nullptr;
}

RecoWindow::~RecoWindow() {
    delete ui;
    for(auto similar: similars) {
        delete similar;
    }
    delete averageFaceLabel;
    delete idFaceLabel;
    delete idNameLabel;
    averageFaceLabel = nullptr;
    idFaceLabel = nullptr;
    idNameLabel = nullptr;
}

void RecoWindow::setAverageFaceLabel(QPixmap pixmap) {
    averageFaceLabel = new QLabel;
    averageFaceLabel->setPixmap(pixmap);
    QLayout* la = this->layout();
    dynamic_cast<QGridLayout*>(la)->addWidget(averageFaceLabel, 0, 0);
}

void RecoWindow::addIdNameLabel(int id) {
    idNameLabel = new QLabel;
    idNameLabel->setText(QString::number(id));
    QLayout* la = this->layout();
    dynamic_cast<QGridLayout*>(la)->addWidget(idNameLabel, 0, 1);
}

void RecoWindow::addIdFaceLabel(QPixmap pixmap) {
    idFaceLabel = new QLabel;
    idFaceLabel->setPixmap(pixmap);
    QLayout* la = this->layout();
    dynamic_cast<QGridLayout*>(la)->addWidget(idFaceLabel, 1, 1);
}

