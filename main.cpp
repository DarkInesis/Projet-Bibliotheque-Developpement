#include <QApplication>
#include <ctime>
#include "window.h"
// #include "gamewidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
     Window w;
     w.show();

//    GameWidget g(nullptr, 10, 6);
//    g.show();

    return a.exec();
}
