#include "threadwebcam.h"

ThreadWebcam::ThreadWebcam()
{

}
void ThreadWebcam::run()
{
    while (true){
        emit signalWebcamToCapture();
        wait(50);
    }
}
