#include "window.h"
#include "ui_window.h"
#include <QTimer>

using namespace std;
using namespace cv;

Window::Window(QWidget *parent) : QWidget(parent), ui(new Ui::Window) {
      ui->setupUi(this);
      this->myWebCam=new Webcam();
      userControler=new UserControl(this,myWebCam);
      // Init usercontroler for the gamewidget
        ui->frameJeu->userControler=this->userControler;
      isWebcamNeedsInitialization=true;
      QTimer *timer = new QTimer();
      timer->start();
      //connect(timer, SIGNAL(timeout()), this, SLOT(update()));
      connect(&threadWebcam,SIGNAL(signalWebcamToCapture()),myWebCam,SLOT(capture()));
      connect(myWebCam,SIGNAL(webcamFrameCaptured(cv::Mat*)),this,SLOT(update(cv::Mat*)));
      myWebCam->moveToThread(&threadWebcam);
      threadWebcam.start();
}
void Window::update(cv::Mat* frame) {
    cv::resize((*frame), (*frame), Size(340, 255), 0, 0, INTER_LINEAR);
    cv::cvtColor((*frame), (*frame), COLOR_BGR2RGB);  // Qt reads in RGB whereas CV in BGR
    QImage imdisplay((uchar *)(*frame).data, (*frame).cols, (*frame).rows, (*frame).step,
                     QImage::Format_RGB888);  // Converts the CV image into Qt standard format
    // Set the frame of the webcam into the widget
    ui->display_image->setPixmap(
        QPixmap::fromImage(imdisplay));  // display the image in label that is created earlier
    updateStateInitialisationButton();
}

Window::~Window() { delete ui; }

void Window::updateStateInitialisationButton()
{
    if (userControler->getNeedWebcamInitialization()) {
          ui->buttonInitWebCam->setEnabled(true);
    } else {
          ui->buttonInitWebCam->setEnabled(false);
    }
}

void Window::on_buttonInitWebCam_clicked() { userControler->resetAbsurdsDetectionStates(); }
