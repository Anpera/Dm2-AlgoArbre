#include "graphics.h"
#include "types.h"
#include <MLV/MLV_all.h>
#include <sys/queue.h>

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

    if (draw_particules && tree->len_plist > 0) {
        GFX_draw_liste_particules(&tree->plist);
    }

    Square sq = tree->pos;
    MLV_draw_rectangle(sq.x, sq.y, sq.size, sq.size, MLV_COLOR_ORANGE);
    GFX_draw_quadtree_grid(tree->hg, draw_particules);
    GFX_draw_quadtree_grid(tree->hd, draw_particules);
    GFX_draw_quadtree_grid(tree->bd, draw_particules);
    GFX_draw_quadtree_grid(tree->bg, draw_particules);
}

void GFX_animate_quadtree(const ListeParticules* particules, const QuadTree* qt) {
    MLV_clear_window(MLV_COLOR_GREY20);
    GFX_draw_quadtree_grid(qt->root, true);
    MLV_wait_milliseconds(20);
    MLV_actualise_window();
}
