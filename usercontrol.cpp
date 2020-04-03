#include "usercontrol.h"

using namespace cv;

UserControl::UserControl() { myWebCam = new Webcam(); }

QPixmap UserControl::getWebcamFrameQPixMap() {
    cv::Mat frame;
    if (myWebCam->getNeedWebcamInitialization()) {
          frame = myWebCam->initModel();
    } else {
          frame = myWebCam->captureOrientation();
    }
      cv::resize(frame, frame, Size(340, 255), 0, 0, INTER_LINEAR);
      cv::cvtColor(frame, frame, COLOR_BGR2RGB);  // Qt reads in RGB whereas CV in BGR
      QImage imdisplay((uchar *)frame.data, frame.cols, frame.rows, frame.step,
                       QImage::Format_RGB888);  // Converts the CV image into Qt standard format
      return QPixmap::fromImage(imdisplay);
}
bool* UserControl::updateMove(){
    //mettre à jour valeur direction gamewidget
    std::string direction="";
    direction=myWebCam->getDirection();
    std::cout<<direction<<std::endl;
     bool moveRight=false;
     bool moveLeft=false;
     bool moveForward=false;
     bool moveBackward=false;
    if(direction=="droite" ) {
          moveRight = true;
    }
    else if(direction=="gauche") {
          moveLeft = true;
    }
    else if(direction=="haut") {
          moveForward = true;
    }
    else if(direction=="bas") {
          moveBackward = true;
    }
    return new bool[4] {moveLeft, moveRight,moveForward,moveBackward};
}

bool UserControl::getNeedWebcamInitialization()
{
    return myWebCam->getNeedWebcamInitialization();
}

void UserControl::resetAbsurdsDetectionStates()
{
    myWebCam->resetAbsurdsDetectionStates();
}


