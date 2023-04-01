#include "quadtree.h"
#include "generation.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

int Quadtree_add_aux(QuadTree* qt, QuadTreeRoot* tree, ListeParticulesEntry* p);

QuadTree QuadTree_init(int carre_taille_min, int max_particules, int taille_fenetre) {
    QuadTree qt = {
        .len = 0,
    };

    int tab_size = pow(taille_fenetre / carre_taille_min, 2);

    qt.tab = (QuadTreeNode*) malloc(tab_size * sizeof(QuadTreeNode));
    qt.max_len = tab_size;
    qt.max_particules = max_particules;
    qt.taille_min = carre_taille_min;

    qt.root = Quadtree_alloc_node(&qt, (Square) {0, 0, taille_fenetre});

    return qt;
}

QuadTreeNode* Quadtree_alloc_node(QuadTree* qt, Square pos) {
    if (qt->len + 1 == qt->max_len)
        return NULL;
    
    QuadTreeNode* new = &qt->tab[qt->len++];
    *new = (QuadTreeNode) {0};
    TAILQ_INIT(&new->plist);
    new->pos = pos;

    return new;
}

/**
 * @brief Retourne la position de la particule dans le carre.
 * 
 * @param s 
 * @param p 
 * @return Secteur 
 */
Secteur Square_pos_particule(Square s, Particule p) {
    if (p.x > (s.x + s.size / 2)) {
        if (p.y > (s.y + s.size / 2))
            return BAS_DROIT;
        else
            return HAUT_DROIT;
    } else {
        if (p.y > (s.y + s.size / 2))
            return BAS_GAUCHE;
        else
            return HAUT_GAUCHE;
    }
}

bool Square_contains_point(Square s, Particule p) {
    return (p.x > s.x && p.x < s.x + s.size) && (p.y > s.y && p.y < s.y + s.size);
}

Square Square_divide(Square sq, Secteur seq) {
    switch (seq) {
    case HAUT_GAUCHE:
        return (Square) {.x = sq.x, .y = sq.y, .size = sq.size / 2};
    case HAUT_DROIT:
        return (Square) {.x = sq.x + sq.size / 2, .y = sq.y, .size = sq.size / 2};
    case BAS_DROIT:
        return (Square) {.x = sq.x + sq.size / 2, .y = sq.y + sq.size / 2, .size = sq.size / 2};
    case BAS_GAUCHE:
        return (Square) {.x = sq.x, .y = sq.y + sq.size / 2, .size = sq.size / 2};
    case EXTERNE:
        break;
    }
    return (Square) {-1, -1, -1};
}

int QuadTree_purge(QuadTree* qt, QuadTreeRoot* tree) {
    (*tree)->hg = Quadtree_alloc_node(qt, Square_divide((*tree)->pos, HAUT_GAUCHE));
    (*tree)->hd = Quadtree_alloc_node(qt, Square_divide((*tree)->pos, HAUT_DROIT));
    (*tree)->bd = Quadtree_alloc_node(qt, Square_divide((*tree)->pos, BAS_DROIT));
    (*tree)->bg = Quadtree_alloc_node(qt, Square_divide((*tree)->pos, BAS_GAUCHE));

    ListeParticulesEntry* entry, *tmp;
    TAILQ_FOREACH(entry, &(*tree)->plist, entries) {
        tmp = TAILQ_NEXT(entry, entries);
        TAILQ_REMOVE(&(*tree)->plist, entry, entries);
        Quadtree_add_aux(qt, tree, entry);
        entry = tmp;
        (*tree)->len_plist--;
        if (!entry)
            break;
    }

    return 1;
}

int Quadtree_add_aux(QuadTree* qt, QuadTreeRoot* tree, ListeParticulesEntry* p) {
    if (!Square_contains_point((*tree)->pos, *p->p))
        return 0;

    if (!DIVIDED_NODE(*tree)) {
        if (((*tree)->pos.size <= qt->taille_min || (*tree)->len_plist < qt->max_particules)) {
            TAILQ_INSERT_HEAD(&((*tree)->plist), p, entries);
            (*tree)->len_plist++;
        }
        else {
            QuadTree_purge(qt, tree);
        }
    }
    else {
        Quadtree_add_aux(qt, &(*tree)->hg, p);
        Quadtree_add_aux(qt, &(*tree)->hd, p);
        Quadtree_add_aux(qt, &(*tree)->bd, p);
        Quadtree_add_aux(qt, &(*tree)->bg, p);
    }

    return 1;
}

int QuadTree_add(QuadTree* qt, Particule* p) {
    ListeParticulesEntry* entry = GEN_new_particule_pointer(p);
    Quadtree_add_aux(qt, &qt->root, entry);

    return 1;
}
