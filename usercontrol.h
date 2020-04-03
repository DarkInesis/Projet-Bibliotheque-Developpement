#ifndef USERCONTROL_H
#define USERCONTROL_H

#include "webcam.h"
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
      bool getNeedToPaint2DLabyrinthe();
};

#endif  // USERCONTROL_H
