#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <thread>
#include <iostream>
#include "usercontrol.h"
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
      void updateStateInitialisationButton();
     private:
      Ui::Window* ui;
     GameWidget* gameWidget;
     class UserControl* userControler;
     bool isWebcamNeedsInitialization;
     private slots:
      void update();
      void on_buttonInitWebCam_clicked();
};

#endif  // WINDOW_H
