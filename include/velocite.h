#ifndef __VELOCITE_H_
#define __VELOCITE_H_

#include <MLV/MLV_all.h>
#include <math.h>

#define PI 3.14159265358979323846

typedef struct vitesse_{
    double anglex;      // Vitesse de déplacement horizontale
    double angley;      // Vitesse de déplacement verticale
} Vitesse;

/**
 * @brief   Génère aléatoirement un angle de déplacement
 *          multiplié par une vitesse d'accélération
 * 
 * @param acceleration  Vitesse à laquelle on veut les points se déplacer
 * @return Vitesse      Variable de vitesse reprenant la vitesse de 
 *                      déplacement horizontale et verticale                      
 */
Vitesse gen_vitesse(int acceleration);

/**
 * @brief   Inverse les vecteurs de déplacements selon
 *          quelle vecteur faut inverser.
 * 
 * @param vect      Objet Vitesse à modifier.
 * @param choix     0 pour changer anglex, 1 pour angley
 */
void invertVect(Vitesse * vect, int choix);
#endif