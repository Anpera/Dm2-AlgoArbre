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
    //QuadTree qt = QuadTree_init(params.window.height * params.window.width);
    ListeParticules particules;
    TAILQ_INIT(&particules);
    MLV_Ev ev;
    Particule* point;

    if (params.gen.enabled) {
        GEN_choose_generation(params, &particules);
        /*QuadTree_load_particules_list(
            &particules, &qt,
            params.gen.animation? GFX_animate_quadtree : NULL
        );*/
    }
    while (1) {
        // GFX_animate_quadtree(qt);
        GFX_draw_liste_particules(&particules);
        MLV_actualise_window();
        ev = SCN_wait_ev();
        if (ev.type == MLV_KEY) {
            if (ev.key_btn == MLV_KEYBOARD_ESCAPE)
                break;
        }
        else if (IS_CLICK(ev)) {
            point = GEN_add_user_particule(&particules, (Particule) {.x = ev.x, .y = ev.y});
            // Ajouter le point au Quadtree ici
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
