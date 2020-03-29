#include "ball.h"
#include "labyrinthe.h"
#include <iostream>
#include <QGLWidget>

Ball::Ball(int ball_i, int ball_j) {
      // Initialisation des paramètres
      quadric = gluNewQuadric();

      // Ball position
      ball_x_ = ball_i * coeff_move + SZ / 2;
      ball_y_ = -ball_j * coeff_move - SZ / 2;

      rotation = 0;

      // Ajout texture
      QImage image = QGLWidget::convertToGLFormat(
          QImage("../Projet-Bibliotheque-Developpement/resources/logo_tse.png"));

      glGenTextures(1, &this->texture);       // Crée un objet texture et lui associe un
                                              // numéro qu’on stocke
      glBindTexture(GL_TEXTURE_2D, texture);  // Définit la texture courante
      // Transmet à OpenGL toutes les caractéristiques de la texture courante :
      // largeur, hauteur, format, etc... et bien sûr l'image
      glTexImage2D(GL_TEXTURE_2D, 0, 4, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                   image.bits());
      // Utilise la méthode GL_LINEAR si besoin pour l’échantillonnage des couleurs
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Ball::~Ball() {
      // Destruction de la quadrique
      gluDeleteQuadric(quadric);
}

void Ball::Display() {
      glPushMatrix();

      // On se déplace au centre du labyrinthe où se trouve la sphère
      glTranslatef(ball_x_, ball_y_, 5);

      rotation += 0.5;
      glRotatef(rotation, 0, 0, 1);

      // Réglage de la lumière
      GLfloat light_position[] = {0.0, 0.0, 0.0, 1.0};
      GLfloat ball_amb[] = {1.0, 1.0, 1.0, 0.0};
      GLfloat ball_dif[] = {1.0, 1.0, 1.0, 1.0};
      GLfloat ball_spec[] = {1.0, 1.0, 1.0, 1.0};

      glLightfv(GL_LIGHT0, GL_POSITION, light_position);
      glLightfv(GL_LIGHT0, GL_AMBIENT, ball_amb);
      glLightfv(GL_LIGHT0, GL_DIFFUSE, ball_dif);
      glLightfv(GL_LIGHT0, GL_SPECULAR, ball_spec);
      glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0);
      glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0);
      glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.04);

      // Réglage de la sphère
      GLfloat ball_emission[] = {0.2, 0.2, 0.2, 0.2};

      glMaterialfv(GL_FRONT, GL_EMISSION, ball_emission);

      // Activation des textures
      glEnable(GL_TEXTURE_2D);
      gluQuadricTexture(this->quadric, GL_TRUE);
      glBindTexture(GL_TEXTURE_2D, this->texture);

      // Dessin de la sphère
      gluSphere(this->quadric, 1.5, 20, 20);

      // glDisable(GL_TEXTURE_2D);

      glPopMatrix();
}
