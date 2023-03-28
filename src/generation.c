#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <sys/queue.h>
#include "generation.h"
#include "types.h"

#define PI 3.14159265358979323846


inline double rand_double(double n) {
    return (double)rand() / (double)(RAND_MAX / n);
}

inline double uniform(double a, double b) {
    return a + (b - a) * rand_double(1);
}

inline int random_bool() {
    return rand() > (RAND_MAX / 2);
}


inline int random_direction() {
    return random_bool() == 0 ? -1 : 1;
}

double inline GEN_distance(double ax, double ay, double bx, double by) {
    double comp_x = (bx - ax);
    double comp_y = (by - ay);
    return comp_x * comp_x + comp_y * comp_y;
}

int inline GEN_compare_point_distance(const void* a, const void* b) {
    return ((PointDistance*) a)->dist - ((PointDistance*) b)->dist;
}

Particule GEN_formule_cercle(
    int largeur, int hauteur,
    int i, int nb_points, int r_max, double concentration
) {
    int offset_x = largeur / 2, offset_y = hauteur / 2;
    double rnd, rayon, angle;
    Particule p;
    rnd = rand_double(1);
    rayon = sqrt(pow(rnd, concentration));
    angle = 2 * PI * rand_double(1);
    rayon *= r_max;
    p.x = rayon * cos(angle) + offset_x;
    p.y = rayon * sin(angle) + offset_y;

    return p;
}

Particule GEN_formule_carre_uniforme(
    int largeur, int hauteur,
    int i, int nb_points, int r_max, double concentration
) {
    int offset_x = largeur / 2, offset_y = hauteur / 2;
    Particule p;
    p.x = uniform(-r_max, r_max) + offset_x;
    p.y = uniform(-r_max, r_max) + offset_y;

    return p;
}

Particule GEN_formule_carre_croissant(
    int largeur, int hauteur,
    int i, int nb_points, int r_max, double concentration
) {
    double dist_inc = ((double) r_max / (double) nb_points) * i;
    double a_x = -dist_inc, a_y = -dist_inc, b_x = dist_inc, b_y = dist_inc;
    int offset_x = largeur / 2;
    int offset_y = hauteur / 2;

    Particule p;
    p.x = uniform(a_x, b_x) + offset_x;
    p.y = uniform(a_y, b_y) + offset_y;

    return p;
}

int GEN_points_formule(
    ListeParticules* points,
    int largeur, int hauteur,
    int nb_points, int r_max, double concentration, bool tri,
    Particule (*formule) (int, int, int, int, int, double)
) {
    PointDistance* tab_points = NULL;
    int offset_x = largeur / 2, offset_y = hauteur / 2;
    TAILQ_INIT(points);
    if (tri) {
        tab_points = malloc(nb_points * sizeof(PointDistance));
        if (!tab_points)
            return 0;
    }

    for (int i = 0; i < nb_points; ++i) {
        Particule p = formule(
            largeur, hauteur,
            i, nb_points,
            r_max, concentration
        );

        // Si on s'attend à trier, on les ajoute à un tableau
        if (tri) {
            double dist = GEN_distance(p.x, p.y, offset_x, offset_y);
            PointDistance p_dist = {p, dist};
            tab_points[i] = p_dist;
        }
        else { // Sinon on ajoute directement dans la liste
            ListeParticulesEntry* new_entry = GEN_new_particule(p);
            if (!new_entry)
                return 0;
            TAILQ_INSERT_TAIL(points, new_entry, entries);
        }
    }
    if (tri) {
        GEN_sort_tab_PointDistance_to_ListeParticules(tab_points, nb_points, points);
    }
    return 1;
}

void GEN_sort_tab_PointDistance_to_ListeParticules(
    PointDistance* tab_points, int size, ListeParticules* points
) {
    qsort(tab_points, size, sizeof(PointDistance), GEN_compare_point_distance);
    for (int i = 0; i < size; ++i) {
        ListeParticulesEntry* new_entry = GEN_new_particule(tab_points[i].p);
        TAILQ_INSERT_TAIL(points, new_entry, entries);
    }
    free(tab_points);
}

void GEN_choose_generation(Parameters params, ListeParticules* points) {
    TAILQ_INIT(points);

    Particule (*formule) (int, int, int, int, int, double);
    if (params.gen.shape == CERCLE) {
        formule = GEN_formule_cercle;
    }
    /* La génération croissante, ne nous permet pas de
    générer des points uniformément, de la même manière que la fonction cercle */
    else if (params.gen.concentration != 1) {
        formule = GEN_formule_carre_croissant;
    } else {
        formule = GEN_formule_carre_uniforme;
    }
    srand(time(NULL));
    GEN_points_formule(points,
        params.window.width, params.window.height,
        params.gen.nb_points, params.gen.rayon,
        params.gen.concentration, params.gen.progressif,
        formule
    );
    /*printf(
        "Animation : %d, Tri : %d\n",
        params.gen.animation, params.gen.progressif
    );*/
}

ListeParticulesEntry* GEN_new_particule(Particule p) {
    Particule* new_p = malloc(sizeof(Particule));
    ListeParticulesEntry* new_vtx = malloc(sizeof(ListeParticulesEntry));

    if (!new_p || !new_vtx)
        return NULL;

    *new_p = p;
    new_vtx->p = new_p;

    return new_vtx;
}

ListeParticulesEntry* GEN_new_particule_pointer(Particule* p) {
    ListeParticulesEntry* new_vtx = malloc(sizeof(Particule));

    if (!new_vtx)
        return NULL;

    new_vtx->p = p;

    return new_vtx;
}


Particule* GEN_add_user_particule(ListeParticules* particules, Particule p) {
    ListeParticulesEntry* new;
    new = GEN_new_particule(p);

    if (!new) return NULL;

    TAILQ_INSERT_TAIL(particules, new, entries);

    return new->p;
}

void GEN_free_ListParticules(ListeParticules* lst, bool free_points) {
    ListeParticulesEntry *vtx = TAILQ_FIRST(lst), *vtx2;

    while (vtx != (void*) lst) {
        vtx2 = TAILQ_NEXT(vtx, entries);
        if (free_points)
            free(vtx->p);
        free(vtx);
        vtx = vtx2;
    }
    TAILQ_INIT(lst);
}
