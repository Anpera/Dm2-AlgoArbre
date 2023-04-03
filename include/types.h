#ifndef TYPES_INCLUDED
#define TYPES_INCLUDED

#include <sys/queue.h>

typedef struct Particule {
    int x;
    int y;
} Particule;

typedef struct ListeParticulesEntry {
    Particule* p;
    STAILQ_ENTRY(ListeParticulesEntry) entries; /**< Pointeurs vers les cellules suivantes et précédentes */
} ListeParticulesEntry;

// Définit un type ListeParticules qui est une entête vers les débuts et fin de la liste
typedef STAILQ_HEAD(ListeParticules, ListeParticulesEntry) ListeParticules;

#endif
