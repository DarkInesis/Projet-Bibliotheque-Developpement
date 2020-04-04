#include "usercontrol.h"
#include <QEvent>
using namespace cv;

UserControl::UserControl(QWidget* parent,Webcam* myWebCam) {
    this->myWebCam=myWebCam;
}



bool UserControl::getNeedWebcamInitialization()
{
    return myWebCam->getNeedWebcamInitialization();
}
bool UserControl::getNeedToPaint2DLabyrinthe(){
    return myWebCam->getCounterConsecutivesNull()>5;
}
