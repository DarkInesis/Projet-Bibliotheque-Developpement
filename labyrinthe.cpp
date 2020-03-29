#include "labyrinthe.h"
#include "draw_functions.h"
#include <iostream>
#include <QGLWidget>

Labyrinthe::Labyrinthe(int width, int height) {
      // Initialisation des paramètres
      // Le labyrinthe à dessiner est généré ici
      maze = Maze(width, height);
      maze.generate();

      // Temporaire : on affiche dans la console pour vérifier que
      // le labyrinthe est bien dessiné
      std::cout << "Affichage" << endl;
      maze.display();

      quadric = gluNewQuadric();

      //      // Ball position
      //      ball_x = (maze.width_ * coeff_move + SZ / 4) / 2;
      //      ball_y = -(maze.height_ * coeff_move + SZ / 4) / 2;

      // Random exit
      // Côté aléatoire du labyrinthe pour la porte de sortie
      // N = 0, E = 1, S = 2, W = 3
      int cote = rand() % 4;

      if (cote == 0) {
            i_exit = 0;
            j_exit = rand() % maze.width_;
            d_exit = Cell::N;
            x_exit_light = i_exit * coeff_move + SZ / 2;
            y_exit_light = SZ / 4;
      } else if (cote == 1) {
            i_exit = rand() % maze.height_;
            j_exit = maze.width_ - 1;
            d_exit = Cell::E;
            x_exit_light = maze.width_ * coeff_move + SZ / 2;
            y_exit_light = -j_exit * coeff_move - SZ / 2;
      } else if (cote == 2) {
            i_exit = maze.height_ - 1;
            j_exit = rand() % maze.width_;
            d_exit = Cell::S;
            x_exit_light = i_exit * coeff_move + SZ / 2;
            y_exit_light = -maze.height_ * coeff_move - SZ / 2;
      } else {
            i_exit = rand() % maze.height_;
            j_exit = 0;
            d_exit = Cell::W;
            x_exit_light = -SZ / 4;
            y_exit_light = -j_exit * coeff_move - SZ / 2;
      }
}

Labyrinthe::~Labyrinthe() {
      // Destruction de la quadrique
      gluDeleteQuadric(quadric);
}

void Labyrinthe::Display() const {
      glPushMatrix();  // On stocke le repère d'origine

      int i, j, k;

      // Couleur du sol et plafond
      GLfloat floor_amb[] = {0.2, 0.2, 0.2, 1.0};
      GLfloat no_emission[] = {0.0, 0.0, 0.0, 0.0};
      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, floor_amb);
      glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);

      // Dessin du sol et plafond du labyrinthe
      glBegin(GL_QUADS);

      // Dessin du sol
      glVertex3f(0, 0, 0);
      glVertex3f(coeff_move * maze.width_ + SZ / 4, 0, 0);
      glVertex3f(coeff_move * maze.width_ + SZ / 4, -coeff_move * maze.height_ - SZ / 4, 0);
      glVertex3f(0, -coeff_move * maze.height_ - SZ / 4, 0);

      // Dessin du plafond
      glVertex3f(0, 0, 0.6 * SZ);
      glVertex3f(coeff_move * maze.width_, 0, 0.6 * SZ);
      glVertex3f(coeff_move * maze.width_, -coeff_move * maze.height_, 0.6 * SZ);
      glVertex3f(0, -coeff_move * maze.height_, 0.6 * SZ);

      glEnd();

      // Couleur des murs
      GLfloat wall_amb_dif_spec[] = {0.8, 0.8, 0.8, 1.0};
      glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wall_amb_dif_spec);
      glMaterialfv(GL_FRONT, GL_SPECULAR, wall_amb_dif_spec);
      glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);

      //***                                             ***//
      //*  Début de l'algorithme de dessin du labyrinthe  *//
      //***                                             ***//

      // On se déplace au centre de la première case
      glTranslatef(SZ / 2, -SZ / 2, 0);

      // Dessin du mur du haut du labyrinthe
      // On monte sur la case du haut pour dessiner les murs côté Nord avec la
      // fonction DrawSouthWall()
      glTranslatef(0, coeff_move, 0);

      // On dessine le poteau en bas à gauche de la case
      DrawPostSW(SZ);

      // Pour toutes les cases de la première ligne
      for (k = 0; k < maze.width_; k++) {
            // Si il y a un mur au Nord de la case
            if (maze.grid_[0][k].isFrontier(Cell::N)) {
                  // On dessine le mur en bas vu qu'on est au dessus
                  DrawSouthWall(SZ);
            }
            // On dessine le poteau en bas à droite
            DrawPostSE(SZ);

            // On se déplace sur la case à droite
            glTranslatef(coeff_move, 0, 0);
      }

      // On revient à la ligne
      glTranslatef(-coeff_move * maze.width_, -coeff_move, 0);

      // Dessine les autres lignes
      for (i = 0; i < maze.height_; i++) {
            // On se déplace sur la case de gauche pour dessiner le poteau en bas à
            // droite
            glTranslatef(-coeff_move, 0, 0);
            DrawPostSE(SZ);
            // Si il y a un mur à gauche dans la première case de la ligne
            if (maze.grid_[i][0].isFrontier(Cell::W)) {
                  // On dessine le mur de gauche avec la fonction DrawEastWall
                  DrawEastWall(SZ);
            }
            // On fait le translate inverse
            glTranslatef(coeff_move, 0, 0);

            // Pour toutes les cases
            for (j = 0; j < maze.width_; j++) {
                  // On dessine les murs soit Est, soit Sud
                  if (maze.grid_[i][j].isFrontier(Cell::E)) DrawEastWall(SZ);
                  if (maze.grid_[i][j].isFrontier(Cell::S)) DrawSouthWall(SZ);
                  // On dessine le poteau en bas à droite
                  DrawPostSE(SZ);

                  // On passe à la case d'après
                  glTranslatef(coeff_move, 0, 0);
            }
            // On revient à la ligne
            glTranslatef(-coeff_move * maze.width_, -coeff_move, 0);
      }

      glPopMatrix();
}

void Labyrinthe::ShowBall() const {
      glPushMatrix();

      // On se déplace au centre du labyrinthe où se trouve la sphère
      glTranslatef(ball_x, ball_y, 5);

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
      GLfloat ball_emission[] = {1.0, 1.0, 1.0, 1.0};

      glMaterialfv(GL_FRONT, GL_EMISSION, ball_emission);

      // Dessin de la sphère
      gluSphere(this->quadric, 1.5, 20, 20);

      glPopMatrix();
}

void Labyrinthe::ShowExitLighting() const {
      glPushMatrix();

      // On se déplace au centre du labyrinthe où se trouve la sphère
      glTranslatef(x_exit_light, y_exit_light, 5);

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

      //  // Réglage de la sphère
      //  GLfloat ball_emission[] = {1.0, 1.0, 1.0, 1.0};

      //  glMaterialfv(GL_FRONT, GL_EMISSION, ball_emission);

      //  // Dessin de la sphère
      //  gluSphere(this->quadric, 1.5, 20, 20);

      glPopMatrix();
}
