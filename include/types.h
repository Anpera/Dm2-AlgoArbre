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
    Particule* p;
    STAILQ_ENTRY(ListeParticulesEntry) entries; /**< Pointeurs vers les cellules suivantes et précédentes */
} ListeParticulesEntry;

// Définit un type ListeParticules qui est une entête vers les débuts et fin de la liste
typedef STAILQ_HEAD(ListeParticules, ListeParticulesEntry) ListeParticules;

typedef struct TabPoints {
    Particule* tab;
    int len;
    int max_len;
} TabPoints;

#endif
