#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <MLV/MLV_all.h>
#include "scenes.h"
#include "UI.h"
#include "graphics.h"
#include "generation.h"
#include "quadtree.h"


void SCN_Quadtree(Parameters params) {
    ListeParticules particules;
    STAILQ_INIT(&particules);
    MLV_Ev ev;
    Particule* point;
    QuadTree qt = QuadTree_init(
        params.feuille.taille_min, params.feuille.max_particules,
        params.window.width);

    if (params.gen.enabled) {
        GEN_choose_generation(params, &particules);
        struct ListeParticulesEntry *items;
        STAILQ_FOREACH(items, &particules, entries){
            QuadTree_add(&qt, items->p);
            if (params.gen.animation){
                MLV_draw_filled_circle(
                    items->p->x,
                    items->p->y,
                    4,
                    MLV_COLOR_RED
                );
                GFX_animate_quadtree(&qt);
                MLV_wait_milliseconds(20);
                MLV_actualise_window();
            }
        }
        /*QuadTree_load_particules_list(
            &particules, &qt,
            params.gen.animation? GFX_animate_quadtree : NULL
        );*/
    }
    while (1) {
        GFX_animate_quadtree(&qt);
        GFX_draw_liste_particules(&particules);
        MLV_actualise_window();
        ev = SCN_wait_ev();
        if (ev.type == MLV_KEY) {
            if (ev.key_btn == MLV_KEYBOARD_ESCAPE)
                break;
        }
        else if (IS_CLICK(ev)) {
            point = GEN_add_user_particule(&particules, (Particule) {.x = ev.x, .y = ev.y});
            QuadTree_add(&qt, point);
        }
    }

    GEN_free_ListParticules(&particules, true);
}

MLV_Ev SCN_wait_ev() {
    MLV_Ev ev;
    ev.type = MLV_wait_keyboard_or_mouse(
        &ev.key_btn, NULL, NULL,
        &ev.x, &ev.y
    );
    ev.state = MLV_PRESSED;
    ev.button = MLV_BUTTON_LEFT;

    return ev;
}
