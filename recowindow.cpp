#include "recowindow.h"
#include "ui_recowindow.h"

RecoWindow::RecoWindow(QWidget *parent):QWidget(parent), ui(new Ui::RecoWindow) {
    ui->setupUi(this);
}

RecoWindow::~RecoWindow()
{
    delete ui;
}

void RecoWindow::setLabel(QPixmap pixmap) {

}

