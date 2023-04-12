#ifndef QUADTREE_INCLUDED
#define QUADTREE_INCLUDED

#include "types.h"
#include "args.h"
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

typedef struct TabListeEntryParticules {
    ListeParticulesEntry* tab;
    int len;
    int max_len;
} TabListeEntryParticules;

typedef struct QuadTree {
    QuadTreeRoot root;  /*< Racine de l'arbre */
    int max_particules; /*< Nombre maximum de particules dans un noeud */
    int taille_min;     /*< Taille minimale d'un noeud */
    
    // Préallocation des noeuds de l'arbre
    struct QuadTreeNode* tab;
    int len; /*< Nombre de noeud actuellement alloué */
    int max_len; /** Nombre de noeuds réellement alloué en mémoire*/
    
    // Préallocation des cellules des sous-listes
    TabListeEntryParticules tab_plist;
} QuadTree;

#define DIVIDED_NODE(node) ((node)->hg != NULL && (node)->hd != NULL && (node)->bd != NULL && (node)->bg != NULL)

/**
 * @brief Initialise un quadtree.
 * Préalloue un tableau qui contiendra tous les noeuds de l'arbre.
 * 
 * @return QuadTree 
 */
QuadTree QuadTree_init(Parameters params);

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
 * fonction à appeler après chaque ajout, prenant en paramètre
 * le quadtree (fonction de dessin par exemple) 
 */
void QuadTree_load_particules_list(
    const TabPoints* particules, QuadTree* qt,
    void (*callback)(const QuadTree*)
);

/**
 * @brief Alloue une cellule de liste depuis le tableau préalloué
 * tab_plist, et réalloue son espace si nécessaire.
 * 
 * @param tab_plist 
 * @param p 
 * @return ListeParticulesEntry* 
 */
ListeParticulesEntry* TabListeEntryParticules_alloc_cellule(
    TabListeEntryParticules* tab_plist, Particule* p);

#endif
