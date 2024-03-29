#ifndef GRAPHICS_INCLUDED
#define GRAPHICS_INCLUDED

#include "types.h"
#include <MLV/MLV_all.h>
#include <sys/queue.h>

#include "quadtree.h"

/**
 * @brief Dessine les points et le quadrillage du Quadtree
 * et affiche dans la fenêtre graphique
 * @param qt 
 */
void GFX_animate_quadtree(const QuadTree* qt);

/**
 * @brief Dessine le Quadtree, et optionnellement ses particules
 * 
 * @param qt 
 * @param draw_particules true : dessine les particules
 */
void GFX_draw_quadtree_grid(const QuadTreeRoot tree, bool draw_particules);

/**
 * @brief Dessines les points d'une liste de particules
 * 
 * @param particules 
 */
void GFX_draw_liste_particules(const ListeParticules* particules);

#endif
