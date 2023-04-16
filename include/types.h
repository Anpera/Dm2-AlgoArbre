#ifndef TYPES_INCLUDED
#define TYPES_INCLUDED

#include <sys/queue.h>

#include "velocite.h"

typedef struct Particule {
    double x;
    double y;
    Vitesse vect;
} Particule;

typedef struct ListeParticulesEntry {
    Particule* p;                               /**< Pointeur sur une Particule*/
    STAILQ_ENTRY(ListeParticulesEntry) entries; /**< Pointeurs vers les cellules suivantes et précédentes */
} ListeParticulesEntry;

// Définit un type ListeParticules qui est une entête vers les débuts et fin de la liste
typedef STAILQ_HEAD(ListeParticules, ListeParticulesEntry) ListeParticules;

/**
 * @brief Gère un tableau de Particules
 * 
 */
typedef struct TabPoints {
    Particule* tab; /**< Tableau de particules */
    int len;        /**< Nombre de points stockés*/
    int max_len;    /**< Nombre de points stockables au maximum*/
} TabPoints;

#endif
