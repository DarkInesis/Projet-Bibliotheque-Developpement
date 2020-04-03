#ifndef USERCONTROL_H
#define USERCONTROL_H

#include "webcam.h"
#include <QWidget>
#include <iostream>

class UserControl : public QObject{
    Q_OBJECT
     private:
      Webcam *myWebCam;
      bool move_left;

     public:
      UserControl(QWidget *parent = nullptr, Webcam * myWebCam=nullptr);
      QPixmap getWebcamFrameQPixMap();
      bool* updateMove();
      bool getNeedWebcamInitialization();
      void resetAbsurdsDetectionStates();
      bool getNeedToPaint2DLabyrinthe();
};

#endif  // USERCONTROL_H
