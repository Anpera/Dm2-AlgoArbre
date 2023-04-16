#include "graphics.h"

void GFX_draw_liste_particules(const ListeParticules* particules) {
    ListeParticulesEntry* entry;
    STAILQ_FOREACH(entry, particules, entries) {
        MLV_draw_filled_circle(
            entry->p->x,
            entry->p->y,
            4,
            MLV_COLOR_RED
        );
    }
}

void GFX_draw_quadtree_grid(const QuadTreeRoot tree, bool draw_particules) {
    if (!(tree))
        return;

    Square sq = tree->pos;
    if (draw_particules && tree->len_plist > 0) {
        MLV_draw_filled_rectangle(sq.x, sq.y, sq.size, sq.size, MLV_COLOR_GREY30);
        MLV_draw_rectangle(sq.x, sq.y, sq.size, sq.size, MLV_COLOR_CYAN);
        GFX_draw_liste_particules(&tree->plist);
    }

    //MLV_draw_rectangle(sq.x, sq.y, sq.size, sq.size, MLV_COLOR_ORANGE);
    GFX_draw_quadtree_grid(tree->hg, draw_particules);
    GFX_draw_quadtree_grid(tree->hd, draw_particules);
    GFX_draw_quadtree_grid(tree->bd, draw_particules);
    GFX_draw_quadtree_grid(tree->bg, draw_particules);
}

void GFX_animate_quadtree(const QuadTree* qt) {
    MLV_clear_window(MLV_COLOR_BLACK);
    //MLV_clear_window(MLV_COLOR_GREY20);
    GFX_draw_quadtree_grid(qt->root, true);
    MLV_wait_milliseconds(20);
    MLV_actualise_window();
}
