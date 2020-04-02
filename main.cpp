#include <QApplication>
#include <ctime>
#include "window.h"
// #include "gamewidget.h"


int main(int argc, char *argv[]) {
      string choix = "";
      /*
      if (choix == "Maze") {
            // Initialisation du generateur de nombres aleatoires
            srand(time(nullptr));
            // Creation de l'application QT
            QApplication app(argc, argv);
            // Creation du widget opengl
            GameWidget gameWidget(nullptr, 5, 10);
            gameWidget.show();
            // Execution de l'application QT
            return app.exec();

      } else {*/
            QApplication a(argc, argv);
            Window w;
            w.show();
            return a.exec();
      //}
}
