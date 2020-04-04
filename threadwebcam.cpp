#include "threadwebcam.h"

ThreadWebcam::ThreadWebcam()
{

}
void ThreadWebcam::run()
{
    while (true){
        emit signalWebcamToCapture();
        msleep(1000/30);
    }
}
