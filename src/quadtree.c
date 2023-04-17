#include "quadtree.h"

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

    // 4^0 + 4^1 + 4^2 + 4^3 + 4^n (n = log2(largeur_fenetre / taille_min_noeud))
    int tab_size = (
        (pow4ll(log2ll(params.window.width / params.feuille.taille_min) + 1) - 1) / \
        (4 - 1) \
        + 1
    );

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
    const TabPoints* particules, QuadTree* qt,
    void (*callback)(const QuadTree*)
) {
    for (int i = 0; i < particules->len; ++i) {
        QuadTree_add(qt, &particules->tab[i]);
        if (callback) {
            callback(qt);
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

void Quadtree_reset(QuadTree* qt) {
    qt->len = 0;
    qt->root = Quadtree_alloc_node(
        qt, qt->root->pos);
    qt->tab_plist.len = 0;
}

static Particule* Quadtree_search_particule_aux(const QuadTreeRoot tree, int x, int y) {
    
    if (!Square_contains_point(tree->pos, (Particule) {.x = x, .y = y}))
        return NULL;
    
    if (!DIVIDED_NODE(tree)) {
        ListeParticulesEntry* entry;
        STAILQ_FOREACH(entry, &tree->plist, entries) {
            if (distance_euclidienne(entry->p->x, entry->p->y, x, y) < 10) {
                return entry->p;
            }
        }
        return NULL;
    }

    Particule* p = NULL;

    if ((p = Quadtree_search_particule_aux(tree->hg, x, y))) return p;
    if ((p = Quadtree_search_particule_aux(tree->hd, x, y))) return p;
    if ((p = Quadtree_search_particule_aux(tree->bd, x, y))) return p;
    if ((p = Quadtree_search_particule_aux(tree->bg, x, y))) return p;

    return NULL;
}

Particule* Quadtree_search_particule(const QuadTree* qt, int x, int y) {
    return Quadtree_search_particule_aux(qt->root, x, y);
}
