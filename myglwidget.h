#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include "wall.h"
#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QTimer>


// Classe dediee a l'affichage d'une scene OpenGL
class MyGLWidget : public QGLWidget
{
    Q_OBJECT

public:

    // Constructeur
    MyGLWidget(QWidget * parent = nullptr);

protected:

    // Fonction d'initialisation
    void initializeGL();

    // Fonction de redimensionnement
    void resizeGL(int width, int height);

    // Fonction d'affichage
    void paintGL();

    // Fonction de gestion d'interactions clavier
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);

    void userMove();

private:
    // Timer d'animation
    float m_TimeElapsed { 0.0f };
    QTimer m_AnimationTimer;
    Wall wall = Wall(5, 5);
};

#endif // MYGLWIDGET_H
