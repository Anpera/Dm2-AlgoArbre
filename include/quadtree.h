#ifndef QUADTREE_INCLUDED
#define QUADTREE_INCLUDED

#include "types.h"
#include <stdbool.h>

typedef enum {
    HAUT_GAUCHE, HAUT_DROIT,
    BAS_DROIT, BAS_GAUCHE, EXTERNE
} Secteur;

typedef struct Square {
    int x, y;
    int size;
} Square;

typedef struct QuadTreeNode {
    struct QuadTreeNode* hg;
    struct QuadTreeNode* hd;
    struct QuadTreeNode* bd;
    struct QuadTreeNode* bg;
    Square pos;
    ListeParticules plist; /*< Liste des particules (max 10) */
    int len_plist;
    int nbp; /*< Nombre de particules sous le secteur, pas uniquement dans le noeud */
} QuadTreeNode, *QuadTreeRoot;

typedef struct QuadTree {
    struct QuadTreeNode* tab;
    QuadTreeRoot root;
    int len;
    int max_len;
    int max_particules;
    int taille_min;
} QuadTree;

#define DIVIDED_NODE(node) ((node)->hg != NULL && (node)->hd != NULL && (node)->bd != NULL && (node)->bg != NULL)

/**
 * @brief Initialise un quadtree.
 * Préalloue un tableau qui contiendra tous les noeuds de l'arbre.
 * 
 * @param carre_taille_min Taille minimale d'un carré du Quadtree (puissance de 4)
 * @param max_particules Nombre maximum de particules dans une feuille du quadtree
 * @param taille_fenetre Taille de la fenêtre (puissance de 4)
 * @return QuadTree 
 */
QuadTree QuadTree_init(int carre_taille_min, int max_particules, int taille_fenetre);

/**
 * @brief Alloue un noeud depuis le tableau préalloué
 * 
 * @param qt 
 * @param pos Position du carre
 * @return QuadTreeNode* 
 */
QuadTreeNode* Quadtree_alloc_node(QuadTree* qt, Square pos);

/**
 * @brief Divise un carré selon un secteur souhaité
 * 
 * @param sq 
 * @param seq 
 * @return Square 
 */
Square Square_divide(Square sq, Secteur seq);

/**
 * @brief Détermine si la particule est dans le carre
 * 
 * @param s 
 * @param p 
 * @return true 
 * @return false 
 */
bool Square_contains_point(Square s, Particule p);

/**
 * @brief Ajoute une particule au Quadtree
 * 
 * @param qt 
 * @param p 
 * @return int 
 */
int QuadTree_add(QuadTree* qt, Particule* p);

/**
 * @brief Divise le carré en quatre sous carrés
 * 
 * @param qt 
 * @param node 
 * @return int 
 */
int QuadTree_purge(QuadTree* qt, QuadTreeRoot* node);

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
