#include "dbcreationwindow.h"
#include "ui_dbcreationwindow.h"
#include <QFileDialog>

DBCreationWindow::DBCreationWindow(FaceRecognition *reco_, QWidget *parent) :
    reco(reco_),QWidget(parent), ui(new Ui::DBCreationWindow) {
    ui->setupUi(this);
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
        int average_count = ui->s_average_count->value();
        int image_count = ui->s_image_count->value();
        reco->prepareDatabase(pathSrc.toStdString(), pathDst.toStdString(),
                              image_count, average_count);

        ui->l_info->setText("DB is ready");
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
