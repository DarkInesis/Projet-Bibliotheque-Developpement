#ifndef USERCONTROL_H
#define USERCONTROL_H

#include "webcam.h"
#include "window.h"
#include "ui_window.h"
#include <QTimer>
#include <QWidget>
#include <iostream>

class UserControl {
     private:
      Webcam *myWebCam;
      bool move_left;

     public:
      UserControl();
      QPixmap getWebcamFrameQPixMap();
      bool* updateMove();
      bool getNeedWebcamInitialization();
      void resetAbsurdsDetectionStates();
};

#endif  // USERCONTROL_H
