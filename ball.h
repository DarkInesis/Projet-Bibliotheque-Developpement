#ifndef BALL_H
#define BALL_H

#include <qopengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QColor>

// Classe pour afficher la sphère lumineuse
class Ball {
     private:
      GLUquadric *quadric;
      GLuint texture;

     public:
      float ball_x_, ball_y_, rotation;

      Ball();
      Ball(int ball_x, int ball_y);
      // Destructeur
      virtual ~Ball();

      // Methode d'affichage
      void Display();

      // Getters
      std::tuple<float, float> getBallPosition() { return std::make_tuple(ball_x_, ball_y_); }
};

#endif  // BALL_H
