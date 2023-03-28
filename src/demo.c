#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <MLV/MLV_all.h>
#include "args.h"
#include "scenes.h"

static Parameters params = {
	.window = {
		.width = 512,
		.height = 512,
	},
	.gen = {
		.enabled = true,
		.shape = CERCLE,
		.rayon = 250,
		.nb_points = 1248,
		.concentration = 1.8,
		.progressif = true,
		.animation = false,
	},
	.feuille = {
		.max_particules = 10,
		.taille_min = 6,
	},
};

int main(int argc, char* argv[]) {
    MLV_create_window("QuadTree", "", params.window.width, params.window.height);
    SCN_Quadtree(params);
    MLV_free_window();
    exit(EXIT_SUCCESS);
}
