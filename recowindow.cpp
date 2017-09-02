#include "recowindow.h"
#include "ui_recowindow.h"

RecoWindow::RecoWindow(QWidget *parent):QWidget(parent), ui(new Ui::RecoWindow) {
    ui->setupUi(this);
    averageLabel = nullptr;
}

RecoWindow::~RecoWindow() {
    delete ui;
    for(auto similar: similars) {
        delete similar;
    }
}

void RecoWindow::setLabel(QPixmap pixmap) {
    averageLabel = new QLabel;
    averageLabel->setPixmap(pixmap);
}

