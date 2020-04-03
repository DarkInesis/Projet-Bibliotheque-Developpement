#ifndef THREADWEBCAM_H
#define THREADWEBCAM_H
#include <QThread>
#include <QObject>

#include <opencv2/imgproc.hpp>
class ThreadWebcam : public QThread
{
    Q_OBJECT
public:
    ThreadWebcam();

signals:
    // Signal demande de capture
    void signalWebcamToCapture();
    /*
    // Signal emit lorsqu'une frame a été capturée
    void threadSignalFrameCaptured(cv::Mat);
    */
protected:
    void run();
private:

};

#endif // THREADWEBCAM_H
