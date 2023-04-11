#include "velocite.h"

Vitesse gen_vitesse(int acceleration){
    Vitesse vect;
    double point = point = MLV_get_random_double(0,1) * 2 * PI;
    vect.anglex = acceleration * cos(point);
    vect.angley = acceleration * sin(point);

    return vect;
}

void invertVect(Vitesse * vect, int choix){
    if (choix == 0) // Si Vitesse X est à inverser
        vect->anglex *= -1.0;

    else if (choix == 1) // Si Vitesse Y est à inverser
        vect->angley *= -1.0;
}

