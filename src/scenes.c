#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <MLV/MLV_all.h>
#include "scenes.h"
#include "UI.h"
#include "graphics.h"
#include "generation.h"
#include "quadtree.h"

void applique_vit(Parameters params, TabPoints *particules){
    Particule* p;
    for (int i = 0; i < particules->len; i++) {
        p = &particules->tab[i];

        if (p->x + p->vect.anglex >= params.window.width ||
            p->x + p->vect.anglex < 0){
            
            invertVect(&p->vect, 0);
            p->x += p->vect.anglex;

            // printf("x : %d ", p->x);
            // printf("y : %d\n", p->y);
        }
        
        if (p->y + p->vect.angley >= params.window.height ||
            p->y + p->vect.angley < 0){
            
            invertVect(&p->vect, 1);
            p->y += p->vect.angley;
            // printf("x : %d ", p->x);
            // printf("y : %d\n", p->y);
        }

        p->x += p->vect.anglex;
        p->y += p->vect.angley;
    }
}

void SCN_Quadtree(Parameters params) {
    TabPoints particules;
    MLV_Ev ev;
    Particule* point;
    QuadTree qt = QuadTree_init(params);
    TABPoints_init_tabpoints(&particules, params.gen.nb_points + params.nb_clicks);

    if (params.gen.enabled) {
        GEN_choose_generation(params, &particules);
        QuadTree_load_particules_list(
            &particules, &qt,
            params.gen.animation ? GFX_animate_quadtree : NULL
        );
    }

    while (1) {
        GFX_animate_quadtree(&qt);
        applique_vit(params, &particules);
        ev = SCN_wait_ev();
        if (ev.type == MLV_KEY) {
            if (ev.key_btn == MLV_KEYBOARD_ESCAPE)
                break;
        }
        else if (IS_CLICK(ev)) {
            if (!(point = TABPoints_ajoute(
                &particules,
                (Particule) {.x = ev.x, .y = ev.y, .vect = gen_vitesse(params.gen.velocite)}
            ))) {
                fprintf(stderr, "Nombre maximum de points atteint.\n");
                continue;
            }

            QuadTree_add(&qt, point);
        }
    }

    TABPoints_free(&particules);
}

MLV_Ev SCN_wait_ev() {
    MLV_Ev ev = {0};
    MLV_wait_milliseconds(16);

    ev.type = MLV_get_event(&ev.key_btn, NULL, NULL, NULL, NULL, NULL, NULL, &ev.button, &ev.state);
    if (ev.state == MLV_PRESSED && ev.button == MLV_BUTTON_LEFT)
        MLV_get_mouse_position(&ev.x, &ev.y);

    return ev;
}
