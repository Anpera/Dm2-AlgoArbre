#ifndef GENERATION_INCLUDED
#define GENERATION_INCLUDED

#include <stdbool.h>
#include <MLV/MLV_color.h>
#include "args.h"
#include "types.h"
#include "velocite.h"


typedef struct {
    Particule p;
    double dist;
} PointDistance;

typedef Particule (*GENFormule) (int, int, int, int, int, double);

/**
 * @brief Renvoie un nombre aléatoire entre 0 et n.
 * 
 * @param n 
 * @return double 
 */
double rand_double(double n);

/**
 * @brief Renvoie un nombre aléatoire entre a et b.
 * 
 * @param a 
 * @param b 
 * @return double 
 */
double uniform(double a, double b);

/**
 * @brief Renvoie un booléen aléatoire.
 * 
 * @return int 
 */
int random_bool();

/**
 * @brief Renvoie aléatoirement un entier négatif
 * ou positif.
 * 
 * @return int 
 */
int random_direction();

/**
 * @brief Génère un ensemble de points selon un mode de
 * génération fournie par une fonction en paramètre.
 * 
 * @param points Adresse du tableau de points où les points seront stockés.
 * @param largeur Largeur de la fenêtre.
 * @param hauteur Hauteur de la fenêtre.
 * @param nb_points Nombre de points à générer.
 * @param r_max Rayon de l'ensemble.
 * @param concentration 
 * @param tri Spécifie si le tableau doit être trié en fonction
 * de la distance des points par rapport à l'origine du cercle.
 * @param formule Fonction à appeler pour générer un point d'un ensemble.
 * @return 0 si une erreur est survenue pendant l'allocation des points,
 * 1 sinon.
 */
int GEN_points_formule(
    TabPoints* points,
    int largeur, int hauteur,
    int nb_points, int r_max, double concentration, bool tri,
    int velocite,
    Particule (*formule) (int, int, int, int, int, double)
);

/**
 * @brief Calcule une distance relative entre deux points
 * (cf TP ProgC)
 * 
 * @param ax 
 * @param ay 
 * @param bx 
 * @param by 
 * @return double 
 */
double GEN_distance(double ax, double ay, double bx, double by);

/**
 * @brief Fonction de comparaison de distance entre deux points.
 * 
 * @param a 
 * @param b 
 * @return int 
 */
int GEN_compare_point_distance(const void* a, const void* b);

/**
 * @brief Trie un tableau de PointDistance, et le copie dans une ListePoint.
 * 
 * @param tab_points Tableau de PointDistance
 * @param size Taille du tableau
 * @param points Adresse du tableau de particules où seront copiés les points
 */
void GEN_sort_tab_PointDistance_to_TabPoints(
    PointDistance* tab_points, int size, TabPoints* points);


/**
 * @brief Génère des particules dans le tableau ``points`` selon
 * les paramètres demandés par l'utilisateur.
 * Le tableau doit être initialisé à l'avance
 * @param params Paramètres
 * @param points Tableau de points de destination
 */
void GEN_choose_generation(Parameters params, TabPoints* points);

/**
 * @brief Libére la mémoire allouée pour une liste de particules.
 * 
 * @param lst Adresse de la liste
 * @param free_points Spécifie si on libère également
 * les particules allouées.
 */
void GEN_free_ListParticules(ListeParticules* lst, bool free_points);

/**
 * @brief Génère un point dans un carré uniforme.
 * 
 * @return Particule 
 */
Particule GEN_formule_carre_uniforme(
    int largeur, int hauteur,
    int i, int nb_points, int r_max, double concentration
);

/**
 * @brief Génère un point d'un carré croissant, en utilisant
 * l'itérateur de la boucle de la fonction appelante. 
 * 
 * @return Particule 
 */
Particule GEN_formule_carre_croissant(
    int largeur, int hauteur,
    int i, int nb_points, int r_max, double concentration
);

/**
 * @brief Génère un point dans un cercle.
 * 
 * @param largeur Largeur de la fenêtre.
 * @param hauteur Hauteur de la fenêtre.
 * @param i Itérateur de la boucle sur nb_points.
 * @param nb_points Nombre de points à générer.
 * @param r_max Rayon du cercle.
 * @param concentration
 * Cas :
 * - Si < 0 : Les points vont être générés à l'extérieur du cercle,
 * en tendant vers ce dernier
 * - Si = 0 : Les points seront sur le cercle
 * - Si > 0 et < 1 : Les points vont être générés à l'intérieur du cercle,
 * en tendant vers ce dernier
 * - Si = 1 : Les points seront distribués uniformément
 * - Si > 1 : Les points tendront vers le centre du cercle
 * @return Particule Particule généré
 */
Particule GEN_formule_cercle(
    int largeur, int hauteur,
    int i, int nb_points, int r_max, double concentration
);

/**
 * @brief Initialise un tableau de points.
 * 
 * @param points 
 * @param max_len 
 * @return int 
 */
int TABPoints_init_tabpoints(TabPoints* points, int max_len);

/**
 * @brief Ajoute une particule au tableau de particules,
 * et renvoie son adresse dans le tableau.
 * @param points 
 * @param a 
 * @return int 
 */
Particule* TABPoints_ajoute(TabPoints* points, Particule a);

/**
 * @brief Libère un tableau de points.
 * 
 * @param points 
 */
void TABPoints_free(TabPoints* points);

#endif
