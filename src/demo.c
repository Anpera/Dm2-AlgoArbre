#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <MLV/MLV_all.h>
#include <getopt.h>
#include "args.h"
#include "scenes.h"

static Parameters params = {
	.window = {
		.width = 512,
		.height = 512,
	},
	.gen = {
		.enabled = false,
		.shape = CERCLE,
		.rayon = 250,
		.nb_points = 1250,
		.concentration = 2,
		.progressif = true,
		.animation = false,
		.velocite = 2,
	},
	.feuille = {
		.max_particules = 5,
		.taille_min = 8,
	},
};

void options_gen(){
	int choix;
	printf("Quel forme prendra la génération ?\n0 : carré\n1 : cercle\n");
	scanf("%d", &choix);
	params.gen.shape = choix == 1 ?  CERCLE : CARRE;
	
	printf("Rayon de génération\n");
	scanf("%d", &params.gen.rayon);

	printf("Nombre de points\n");
	scanf("%d", &params.gen.nb_points);

	printf("Animation ?\n0: Non\n1: Oui\n");
	scanf("%d", &choix);
	params.gen.animation = choix==1 ? true : false;

	return;
}

void flags(int argc, char* argv[]){
	int opt, options_index = 0;
	static struct option long_options[] = {
            // {"width",  required_argument, 0,  'w' },
            // {"height",  required_argument, 0,  'h' },
			{"fenetre",  required_argument, 0,  'f' },
            {"gen",    no_argument, 0,  'g' },
			{"maxpar",    required_argument, 0,  'm' },
			{"taillemin",    required_argument, 0,  't' },
            {0,         0,                 0,  0 }
        };

	while ((opt = getopt_long_only(argc, argv, "gw:h:m:t:", long_options, &options_index)) != -1){
		switch (opt){
			case 'g':
				params.gen.enabled = true;
				options_gen();
				break;

			case 'w':
				params.window.width = atoi(optarg);
				break;
			
			case 'h':
				params.window.height = atoi(optarg);
				break;

			case 'f':
				params.window.width = atoi(optarg);
				params.window.height = params.window.width;
				break;

			case 'm':
				params.feuille.max_particules = atoi(optarg);
				break;

			case 't':
				params.feuille.taille_min = atoi(optarg);
				break;

			case '?':
            break;

        	default:
            printf("?? getopt returned character code 0%o ??\n", opt);
		}
	}
}

int main(int argc, char* argv[]) {
	flags(argc, argv);
    MLV_create_window("QuadTree", "", params.window.width, params.window.height);
    SCN_Quadtree(params);
    MLV_free_window();
    exit(EXIT_SUCCESS);
}
