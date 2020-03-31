#include "window.h"
#include "ui_window.h"
#include <mutex>
#include <QTimer>

using namespace std;
using namespace cv;

Window::Window(QWidget *parent) : QWidget(parent), ui(new Ui::Window) {
      ui->setupUi(this);

      myWebCam = new Webcam();
      QTimer *timer = new QTimer();
      timer->start();
      connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}
void Window::update() {
      cv::Mat frame;
      if (myWebCam->getNeedWebcamInitialization()) {
            ui->buttonInitWebCam->setEnabled(true);
            frame = myWebCam->initModel();
      } else {
            ui->buttonInitWebCam->setEnabled(false);
            frame = myWebCam->captureOrientation();
      }
      updateVisage(frame);
}

void Window::updateVisage(cv::Mat img) {
      cv::resize(img, img, Size(340, 255), 0, 0, INTER_LINEAR);
      cv::cvtColor(img, img, COLOR_BGR2RGB);  // Qt reads in RGB whereas CV in BGR
      QImage imdisplay((uchar *)img.data, img.cols, img.rows, img.step,
                       QImage::Format_RGB888);  // Converts the CV image into Qt standard format
      ui->display_image->setPixmap(
          QPixmap::fromImage(imdisplay));  // display the image in label that is created earlier
}

Window::~Window() { delete ui; }

void Window::on_buttonInitWebCam_clicked() { myWebCam->resetAbsurdsDetectionStates(); }
