/** Author : L. Dedier **/

#include <QApplication>
#include <ctime>
#include "window.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Window w;
    w.show();
    return a.exec();
}
