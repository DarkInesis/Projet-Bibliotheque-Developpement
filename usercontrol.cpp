#include "usercontrol.h"

using namespace cv;

UserControl::UserControl() { myWebCam = new Webcam(); }

void UserControl::updateVisage(cv::Mat img) {

      cv::resize(img, img, Size(340, 255), 0, 0, INTER_LINEAR);
      cv::cvtColor(img, img, COLOR_BGR2RGB);  // Qt reads in RGB whereas CV in BGR
      QImage imdisplay((uchar *)img.data, img.cols, img.rows, img.step,
                       QImage::Format_RGB888);  // Converts the CV image into Qt standard format
      ui->display_image->setPixmap(
          QPixmap::fromImage(imdisplay));  // display the image in label that is created earlier
}
