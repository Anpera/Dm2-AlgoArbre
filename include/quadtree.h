#ifndef QUADTREE_INCLUDED
#define QUADTREE_INCLUDED

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "args.h"
#include "generation.h"
#include "types.h"
#include "utils.h"


typedef enum {
    HAUT_GAUCHE, HAUT_DROIT,
    BAS_DROIT, BAS_GAUCHE, EXTERNE
} Secteur;

typedef struct Square {
    int x, y;
    int size;
} Square;

/**
 * @brief Noeud d'un Quadtree.
 * 
 */
typedef struct QuadTreeNode {
    struct QuadTreeNode *hg, *hd, *bd, *bg; /**< Pointeurs sur les noeuds enfants */
    Square pos;            /**< Position et taille du noeud */
    ListeParticules plist; /**< Liste des particules */
    int len_plist;         /**< Nombre de particules dans le noeud */
    int nbp;               /**< Nombre de particules sous le secteur, pas uniquement dans le noeud */
} QuadTreeNode, *QuadTreeRoot;

/**
 * @brief Tableau de cellules de liste de particules.
 * Permet la préallocation des cellules, afin d'éviter
 * les appels de malloc/free sucessifs.
 */
typedef struct TabListeEntryParticules {
    ListeParticulesEntry* tab; /**< Tableau de cellules préalloueés */
    int len;                   /**< Nombre de cellules utilisées */
    int max_len;               /**< Nombre de cellules préallouées */
} TabListeEntryParticules;

/**
 * @brief Structure contenant les paramètres du Quadtree,
 * et son tableau de cellules de liste préalloué.
 * 
 */
typedef struct QuadTree {
    QuadTreeRoot root;  /**< Racine de l'arbre */
    int max_particules; /**< Nombre maximum de particules dans un noeud */
    int taille_min;     /**< Taille minimale d'un noeud */

    // Préallocation des noeuds de l'arbre
    struct QuadTreeNode* tab;
    int len;            /**< Nombre de noeud actuellement alloué */
    int max_len;        /**< Nombre de noeuds réellement alloué en mémoire*/
    
    // Préallocation des cellules des sous-listes
    TabListeEntryParticules tab_plist; /**< Tableau de cellules de liste prélloué */
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
 * @brief Réinitialise le Quadtree.
 * 
 * @param qt 
 */
void Quadtree_reset(QuadTree* qt);

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
