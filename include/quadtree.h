#ifndef QUADTREE_INCLUDED
#define QUADTREE_INCLUDED

#include "types.h"

typedef enum {
    HAUT_GAUCHE, HAUT_DROIT,
    BAS_DROIT, BAS_GAUCHE,
} Secteur;

typedef struct QuadTreeNode {
    struct QuadTreeNode* hg;
    struct QuadTreeNode* hd;
    struct QuadTreeNode* bd;
    struct QuadTreeNode* bg;
    Secteur pos;
    ListeParticules plist; /*< Liste des particules (max 10) */
    int nbp; /*< Nombre de particules sous le secteur, pas uniquement dans le noeud */
} QuadTreeNode;

typedef struct QuadTree {
    struct QuadTreeNode* tab;
    int len;
} QuadTree;

QuadTree QuadTree_init(int size);

void QuadTree_add(QuadTree* qt, Particule p);

void QuadTree_purge(QuadTreeNode* node);


/**
 * @brief Ajoute sucessivement les particules de la liste
 * au Quadtree
 * 
 * @param particules 
 * @param qt 
 * @param callback Si différent de NULL,
 * fonction à appeler après chaque ajout (fonction de
 * dessin par exemple) 
 */
void QuadTree_load_particules_list(
    const ListeParticules* particules, QuadTree* qt,
    void (*callback)(QuadTree*)
);

#endif
