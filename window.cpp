#include "window.h"
#include "ui_window.h"
#include <QTimer>

using namespace std;
using namespace cv;

Window::Window(QWidget *parent) : QWidget(parent), ui(new Ui::Window) {
      ui->setupUi(this);
      userControler=new UserControl();
      // Init usercontroler for the gamewidget

      isWebcamNeedsInitialization=true;
      QTimer *timer = new QTimer();
      timer->start();
      connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}
void Window::update() {
    // Set the frame of the webcam into the widget
    ui->display_image->setPixmap(
        userControler->getWebcamFrameQPixMap());  // display the image in label that is created earlier
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
