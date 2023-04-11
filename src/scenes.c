#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <MLV/MLV_all.h>
#include "scenes.h"
#include "UI.h"
#include "graphics.h"
#include "generation.h"
#include "quadtree.h"

void applique_vit(Parameters params, ListeParticules *particules){
    ListeParticulesEntry* entry;
    STAILQ_FOREACH(entry, particules, entries) {
        if (entry->p->x + entry->p->vect.anglex >= params.window.width ||
            entry->p->x + entry->p->vect.anglex < 0){
            
            invertVect(&entry->p->vect, 0);
            entry->p->x += entry->p->vect.anglex;

            // printf("x : %d ", entry->p->x);
            // printf("y : %d\n", entry->p->y);
        }
        
        if (entry->p->y + entry->p->vect.angley >= params.window.height ||
            entry->p->y + entry->p->vect.angley < 0){
            
            invertVect(&entry->p->vect, 1);
            entry->p->y += entry->p->vect.angley;
            // printf("x : %d ", entry->p->x);
            // printf("y : %d\n", entry->p->y);
        }

        entry->p->x += entry->p->vect.anglex;
        entry->p->y += entry->p->vect.angley;
    }
}

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
        QuadTree_load_particules_list(
            &particules, &qt,
            params.gen.animation ? GFX_animate_quadtree : NULL
        );
    }
    while (1) {
        applique_vit(params, &particules);
        GFX_animate_quadtree(&particules, &qt);
        ev = SCN_wait_ev();
        if (ev.type == MLV_KEY) {
            if (ev.key_btn == MLV_KEYBOARD_ESCAPE)
                break;
        }
        else if (IS_CLICK(ev)) {
            point = GEN_add_user_particule(&particules, (Particule) {.x = ev.x, .y = ev.y, .vect = gen_vitesse(params.gen.velocite)});
            QuadTree_add(&qt, point);
        }
    }

    GEN_free_ListParticules(&particules, true);
}

MLV_Ev SCN_wait_ev() {
    MLV_Ev ev;
    ev.type = MLV_get_event(&ev.key_btn, NULL, NULL, NULL, NULL, &ev.x, &ev.y, &ev.button, &ev.state);
    MLV_wait_milliseconds(16);
    /* ev.type = MLV_wait_keyboard_or_mouse_or_seconds(
        &ev.key_btn, NULL, NULL,
        &ev.x, &ev.y, 1
    );
    ev.state = MLV_PRESSED;
    ev.button = MLV_BUTTON_LEFT;
    */

    return ev;
}
