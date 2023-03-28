#ifndef GRAPHICS_INCLUDED
#define GRAPHICS_INCLUDED

#include "quadtree.h"

/**
 * @brief Dessine les points et le quadrillage du Quadtree
 * 
 * @param qt 
 */
void GFX_animate_quadtree(const QuadTree* qt);

/**
 * @brief Dessine le quadrillage du Quadtree
 * 
 * @param qt 
 */
void GFX_draw_quadtree_grid(QuadTree qt);

/**
 * @brief Dessine les particules du Quadtree
 * 
 * @param qt 
 */
void GFX_draw_quadtree_particules(QuadTree qt);

/**
 * @brief Dessines les points d'une liste de particules
 * 
 * @param particules 
 */
void GFX_draw_liste_particules(const ListeParticules* particules);

#endif
