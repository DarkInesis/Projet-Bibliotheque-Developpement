#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <thread>
#include <iostream>
#include "webcam.h"

namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    ~Window();
    void updateVisage(cv::Mat img);

private:
    Ui::Window *ui;
    class webcam* myWebCam;
private slots:
    void update();
};

#endif // WINDOW_H
