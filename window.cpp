#include "window.h"
#include "ui_window.h"
#include <QTimer>

using namespace std;
using namespace cv;

Window::Window(QWidget *parent) : QWidget(parent), ui(new Ui::Window) {
    ui->setupUi(this);
    this->myWebCam = new Webcam();

    qRegisterMetaType<Webcam::Move>("Webcam::Move");

    // Connection permettant de mettre a jour la direction dans l'openGLWidget
    connect(myWebCam, SIGNAL(directionChanged(Webcam::Move)), ui->frameJeu,
            SLOT(updateDirection(Webcam::Move)));

    // Connection entre le thread et myWebCam
    connect(myWebCam, SIGNAL(webcamFrameCaptured(cv::Mat *)), this, SLOT(update(cv::Mat *)));
    myWebCam->start();

    // Connections liéés à la fin du jeu
    connect(ui->restart_button_2,SIGNAL(clicked()),ui->frameJeu,SLOT(restartGame()));
    connect(ui->frameJeu,SIGNAL(gameFinished(int)), this,SLOT(enableMenu(int)));
    ui->menu->setHidden(true);
}
void Window::update(cv::Mat *frame) {
    cv::resize((*frame), (*frame), Size(340, 255), 0, 0, INTER_LINEAR);
    cv::cvtColor((*frame), (*frame), COLOR_BGR2RGB);  // Qt reads in RGB whereas CV in BGR
    QImage imdisplay((uchar *)(*frame).data, (*frame).cols, (*frame).rows, (*frame).step,
                     QImage::Format_RGB888);  // Converts the CV image into Qt standard format
    // Set the frame of the webcam into the widget
    ui->display_image->setPixmap(
        QPixmap::fromImage(imdisplay));  // display the image in label that is created earlier
}

Window::~Window() { delete ui; }

void Window::updateStateInitialisationButton(bool needWebcamInitialization) {
    if (needWebcamInitialization) {
        ui->buttonInitWebCam->setEnabled(true);
    } else {
        ui->buttonInitWebCam->setEnabled(false);
    }
}

void Window::on_buttonInitWebCam_clicked() {
    myWebCam->needUpdate();
    ;
}

void Window::on_restart_button_2_clicked()
{
    ui->menu->setHidden(true);
}

void Window::enableMenu(int time)
{
    cout<<to_string(time)<<endl;
    ui->timeLabel_2->setText(QString(time));
    ui->menu->setHidden(false);
}
