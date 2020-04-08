#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <iostream>
#include "webcam.h"
#include "gamewidget.h"
namespace Ui {
class Window;
}

class Window : public QWidget {
    Q_OBJECT

   public:
    explicit Window(QWidget* parent = nullptr);
    ~Window();

   private:
    Ui::Window* ui;
    GameWidget* gameWidget;
    class UserControl* userControler;
    Webcam* myWebCam;
    bool isWebcamNeedsInitialization;

    // ThreadWebcam threadWebcam;
  signals:
    void restart();
   private slots:
    void update(cv::Mat*);
    void on_buttonInitWebCam_clicked();
    void updateStateInitialisationButton(bool);
    void on_restart_button_2_clicked();
    void enableMenu(QString);
};

#endif  // WINDOW_H
