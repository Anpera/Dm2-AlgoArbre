#ifndef __VELOCITE_H_
#define __VELOCITE_H_

#include <MLV/MLV_all.h>
#include <math.h>

#define PI 3.14159265358979323846

typedef struct vitesse_{
    double anglex;
    double angley;
} Vitesse;

Vitesse gen_vitesse(int acceleration);
void invertVect(Vitesse * vect, int choix);
#endif