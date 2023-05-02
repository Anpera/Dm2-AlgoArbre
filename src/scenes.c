#include "scenes.h"
#include "utils.h"

static void attraction_curseur(Particule* p, MLV_Ev ev) {
    const int masse_curs = 5;
    const int g = 200;

    double dist = distance_euclidienne(p->x, p->y, ev.x, ev.y);
    clamp(g * 0.1, &dist, 1 * g);
    double accel = (masse_curs * p->masse) / CARRE(dist) * g;
    Vitesse dir = {.anglex = (ev.x - p->x) * accel, .angley = (ev.y - p->y) * accel};
    p->x += dir.anglex;
    p->y += dir.angley;
}

/**
 * @brief   Applique la vitesse entré en paramètre
 *          à chaque point de la liste des particules
 *          et gère les collisions avec le bord de l'écran
 * 
 * @param params        Paramètres d'exécution comprenant
 *                      la vitesse
 * @param particules    Liste de points
 */
static void applique_vit(Parameters params, TabPoints *particules, MLV_Ev ev) {
    Particule* p;
    for (int i = 0; i < particules->len; i++) {
        p = &particules->tab[i];

        if (p->x + p->vect.anglex >= params.window.width ||
            p->x + p->vect.anglex < 0){
            invertVect(&p->vect, 0);
        }
        
        if (p->y + p->vect.angley >= params.window.height ||
            p->y + p->vect.angley < 0){
            invertVect(&p->vect, 1);
        }

        if (params.suivi_curseur) {
            attraction_curseur(p, ev);
        }

        p->x += p->vect.anglex;
        p->y += p->vect.angley;
    }
}

/**
 * @brief Déplace avec un glisser-déposer, un point cliqué dans la fenêtre
 * 
 * @param qt Quadtree contenant les points segmentés
 * @param ev Evènement
 * @return true Une action glisser-déposer est en cours
 * @return false L'utilisateur ne déplace actuellement aucun point
 */
static bool deplace_point_manuel(const QuadTree* qt, MLV_Ev ev) {
    static Particule* glisser = NULL;
    
    if (IS_RIGHT_CLICK(ev))
        glisser = Quadtree_search_particule(qt, ev.x, ev.y);
    
    if (glisser) {
        glisser->x = ev.x;
        glisser->y = ev.y;

        if (ev.state == MLV_RELEASED)
            glisser = NULL;
    }

    return !!glisser;
}

void SCN_Quadtree(Parameters params) {
    TabPoints particules;
    MLV_Ev ev = {0};
    Particule* point;
    bool glissement = false;
    QuadTree qt = QuadTree_init(params);
    TABPoints_init_tabpoints(&particules, params.gen.nb_points + params.nb_clicks);

    if (params.gen.enabled) {
        GEN_choose_generation(params, &particules);
        QuadTree_load_particules_list(
            &particules, &qt,
            params.gen.pas_a_pas ? GFX_animate_quadtree : NULL
        );
    }

    while (1) {
        if (params.feuille.velocite || glissement) {
            Quadtree_reset(&qt);
            QuadTree_load_particules_list(&particules, &qt, NULL);
            applique_vit(params, &particules, ev);
        }
        GFX_animate_quadtree(&qt);
        ev = SCN_wait_ev();

        if (ev.type == MLV_KEY) {
            if (ev.key_btn == MLV_KEYBOARD_ESCAPE)
                break;
        }

        else if (IS_LEFT_CLICK(ev)) {
            if (!(point = TABPoints_ajoute(
                &particules,
                (Particule) {.x = ev.x, .y = ev.y, .vect = gen_vitesse(params.feuille.velocite), .masse = 1}
            ))) {
                fprintf(stderr, "Nombre maximum de points atteint.\n");
                continue;
            }

            QuadTree_add(&qt, point);
        }
        glissement = deplace_point_manuel(&qt, ev);
    }
    TABPoints_free(&particules);
}

MLV_Ev SCN_wait_ev(void) {
    MLV_Ev ev = {0};
    MLV_wait_milliseconds(16);

    ev.type = MLV_get_event(&ev.key_btn, NULL, NULL, NULL, NULL, NULL, NULL, &ev.button, &ev.state);
    // if (ev.state == MLV_PRESSED || ev.state == MLV_RELEASED)
        MLV_get_mouse_position(&ev.x, &ev.y);

    return ev;
}
