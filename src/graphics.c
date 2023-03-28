#include "graphics.h"
#include "types.h"
#include <MLV/MLV_all.h>
#include <sys/queue.h>

void GFX_draw_liste_particules(const ListeParticules* particules) {
    ListeParticulesEntry* entry;
    TAILQ_FOREACH(entry, particules, entries) {
        MLV_draw_filled_circle(
            entry->p->x,
            entry->p->y,
            4,
            MLV_COLOR_RED
        );
    }
}
