#ifndef USERCONTROL_H
#define USERCONTROL_H

#include <webcam.h>
#include "window.h"
#include "ui_window.h"
#include <mutex>
#include <QTimer>
#include <QWidget>
#include <thread>
#include <iostream>
#include "webcam.h"
#include "gamewidget.h"

class UserControl {
     private:
      Webcam *myWebCam;
      bool move_left;

     public:
      UserControl();
      void updateVisage(cv::Mat img);
};

#endif  // USERCONTROL_H
