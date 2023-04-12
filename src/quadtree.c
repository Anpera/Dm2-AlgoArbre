#include "quadtree.h"
#include "generation.h"
#include "args.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

static int Quadtree_add_aux(QuadTree* qt, QuadTreeRoot* tree, ListeParticulesEntry* p) {
    if (!Square_contains_point((*tree)->pos, *p->p))
        return 0;

    if (!DIVIDED_NODE(*tree)) {
        if (((*tree)->pos.size <= qt->taille_min || (*tree)->len_plist < qt->max_particules)) {
            STAILQ_INSERT_HEAD(&((*tree)->plist), p, entries);
            (*tree)->len_plist++;
            return 1;
        }
        else {
            QuadTree_purge(qt, tree);
        }
    }
    Quadtree_add_aux(qt, &(*tree)->hg, p);
    Quadtree_add_aux(qt, &(*tree)->hd, p);
    Quadtree_add_aux(qt, &(*tree)->bd, p);
    Quadtree_add_aux(qt, &(*tree)->bg, p);

    return 1;
}

QuadTree QuadTree_init(Parameters params) {
    QuadTree qt = {
        .len = 0,
    };

    //int tab_size = pow(params.window.width / params.feuille.taille_min, 2);
    // 4^0 + 4^1 + 4^2 + 4^3 + 4^n (n = log2(W))
    int tab_size = (pow(4, log2(params.window.width)) - 1) / 3 + 1;

    qt.tab = (QuadTreeNode*) malloc(tab_size * sizeof(QuadTreeNode));
    qt.max_len = tab_size;
    qt.max_particules = params.feuille.max_particules;
    qt.taille_min = params.feuille.taille_min;

    qt.root = Quadtree_alloc_node(&qt, (Square) {0, 0, params.window.width});

    qt.tab_plist = (TabListeEntryParticules) {
        .max_len = params.gen.nb_points + params.nb_clicks,
        .len = 0,
    };
    qt.tab_plist.tab = (ListeParticulesEntry*) malloc(qt.tab_plist.max_len * sizeof(ListeParticulesEntry));

    return qt;
}

QuadTreeNode* Quadtree_alloc_node(QuadTree* qt, Square pos) {
    if (qt->len >= qt->max_len)
        return NULL;
    
    QuadTreeNode* new = &qt->tab[qt->len++];
    *new = (QuadTreeNode) {0};
    STAILQ_INIT(&new->plist);
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

    ListeParticulesEntry* entry;
    while (!STAILQ_EMPTY(&(*tree)->plist)) {
        entry = STAILQ_FIRST(&(*tree)->plist);
        STAILQ_REMOVE_HEAD(&(*tree)->plist, entries);
        (*tree)->len_plist--;
        Quadtree_add_aux(qt, tree, entry);
    }

    return 1;
}

int QuadTree_add(QuadTree* qt, Particule* p) {
    ListeParticulesEntry* entry = TabListeEntryParticules_alloc_cellule(&qt->tab_plist, p);
    if (!entry)
        return 0;
    
    Quadtree_add_aux(qt, &qt->root, entry);

    return 1;
}

void QuadTree_load_particules_list(
    const ListeParticules* particules, QuadTree* qt,
    void (*callback)(const ListeParticules*, const QuadTree*)
) {
    struct ListeParticulesEntry *items;
    STAILQ_FOREACH(items, particules, entries){
        QuadTree_add(qt, items->p);
        if (callback){
            callback(particules, qt);
        }
    }
}

ListeParticulesEntry* TabListeEntryParticules_alloc_cellule(
    TabListeEntryParticules* tab_plist, Particule* p
) {
    ListeParticulesEntry *new_entry;
    if (tab_plist->len >= tab_plist->max_len)
        return NULL;

    new_entry = &(tab_plist->tab[tab_plist->len++]);
    new_entry->p = p;
    return new_entry;
}
