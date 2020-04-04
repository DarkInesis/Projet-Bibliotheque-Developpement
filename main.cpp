#include <QApplication>
#include <ctime>
#include "window.h"
// #include "gamewidget.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Window w;
    w.show();
    return a.exec();
}
