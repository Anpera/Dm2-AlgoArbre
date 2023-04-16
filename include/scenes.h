#ifndef H_SCENES
#define H_SCENES

#include <MLV/MLV_all.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "args.h"
#include "generation.h"
#include "graphics.h"
#include "quadtree.h"
#include "UI.h"
#include "velocite.h"

/**
 * @brief       Boucle de fonctionnement principal du programme,
 *              affichant les points et le dessin de l'arbre quadtree
 *              jusqu'à ce que l'utilisateur quitte.
 * 
 * @param params    Liste de paramètres d'éxécution
 */
void SCN_Quadtree(Parameters params);

/**
 * @brief   Attends 16 millisecondes et récolte le
 *          premier évènement de clavier ou de souris.
 *          Peut être vide si l'utilisateur ne fait rien.
 * 
 * @return MLV_Ev   Evènement détecté.
 */
MLV_Ev SCN_wait_ev();

#endif
