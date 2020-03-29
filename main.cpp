
/*
 *

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window w;
    w.show();
    return a.exec();
}

*/

#include <QApplication>
#include <ctime>
#include "gamewidget.h"
#include "window.h"

int main(int argc, char *argv[]) {
      string choix = "Maze";

      if (choix == "Maze") {
            // Initialisation du generateur de nombres aleatoires
            srand(time(nullptr));
            // Creation de l'application QT
            QApplication app(argc, argv);
            // Creation du widget opengl
            GameWidget gameWidget;
            gameWidget.show();
            // Execution de l'application QT
            return app.exec();

      } else {
            QApplication a(argc, argv);
            Window w;
            w.show();
            return a.exec();
      }
}
