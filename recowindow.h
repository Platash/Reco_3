#ifndef RECOWINDOW_H
#define RECOWINDOW_H

#include <QWidget>
#include <QPixmap>
#include <QLabel>

namespace Ui {
class RecoWindow;
}

class RecoWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RecoWindow(QWidget *parent = 0);
    ~RecoWindow();

    void setLabel(QPixmap pixmap);

private:
    Ui::RecoWindow *ui;
    QLabel* averageLabel;
    std::vector<QLabel*> similars;
};

#endif // RECOWINDOW_H
