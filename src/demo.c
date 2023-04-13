#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <MLV/MLV_all.h>
#include <getopt.h>
#include <string.h>
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
        .concentration = 1.8,
        .progressif = false,
        .animation = false,
        .velocite = 0,
    },
    .feuille = {
        .max_particules = 5,
        .taille_min = 8,
    },
};

int count_bits1(int test){
    return __builtin_popcountll(test);
}

void flags(int argc, char* argv[]){
    int opt, options_index = 0;
    static struct option long_options[] = {
            {"fenetre",  required_argument, 0,  'f' },
            {"gen",    no_argument, 0,  'g' },
            {"shape", required_argument, 0, 's'},
            {"rayon", required_argument, 0, 'r'},
            {"nbpoints", required_argument, 0, 'n'},
            {"concentration", required_argument, 0, 'c'},
            {"progressif", required_argument, 0, 'p'},
            {"animation", no_argument, 0, 'a'},
            {"velocite", no_argument, 0, 'v'},
            {"maxpar",    required_argument, 0,  'm' },
            {"taillemin",    required_argument, 0,  't' },
            {0,         0,                 0,  0 }
        };

    while ((opt = getopt_long_only(argc, argv, "f:gs:r:n:c:p:av:m:t:", long_options, &options_index)) != -1){
        switch (opt){
            case 'f':
                params.window.width = atoi(optarg);
                if (count_bits1(params.window.width) == 1)
                    params.window.height = params.window.width;
                else
                    params.window.width = 512;
                break;

            case 'g':
                params.gen.enabled = true;
                break;

            case 's':
                if (strcmp(optarg, "carre") == 0)
                    params.gen.shape = CARRE;

                else if (strcmp(optarg, "cercle") == 0)
                    params.gen.shape = CERCLE;

                else
                    printf("Erreur, seul les arguments \"cercle\" et \"carre\" "
                           "sont acceptés avec le flag -s\n");
                params.gen.enabled = true;
                break;

            case 'r':
                params.gen.rayon = atoi(optarg);
                if (!params.gen.rayon)
                    params.gen.rayon = 250;
                params.gen.enabled = true;
                break;

            case 'n':
                params.gen.nb_points = atoi(optarg);
                if (!params.gen.nb_points)
                    params.gen.nb_points = 1250;
                params.gen.enabled = true;
                break;

            case 'c':
                params.gen.concentration = atof(optarg);
                if (!params.gen.concentration)
                    params.gen.concentration = 1.8;
                params.gen.enabled = true;
                break;

            case 'p':
                if (atoi(optarg) == 1)
                    params.gen.progressif = true;
                else
                    params.gen.progressif = false;
                params.gen.enabled = true;
                break;

            case 'a':
                params.gen.animation = true;
                params.gen.enabled = true;
                break;

            case 'v':
                params.gen.velocite = atoi(optarg);
                params.gen.enabled = true;
                break;

            case 'm':
                params.feuille.max_particules = atoi(optarg);
                if (params.feuille.max_particules == 0)
                    params.feuille.max_particules = 5;
                break;

            case 't':
                params.feuille.taille_min = atoi(optarg);
                    if (count_bits1(params.feuille.taille_min) != 1)
                        params.feuille.taille_min = 8;
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
