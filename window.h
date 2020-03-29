#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <thread>
#include <iostream>
#include "webcam.h"
#include "myglwidget.h"

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
    MyGLWidget* glWidget;
private slots:
    void update();
    void on_buttonInitWebCam_clicked();
};

#endif // WINDOW_H
