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
    .nb_clicks = 50,
};

int count_bits1(int test){
    return __builtin_popcountll(test);
}

void flags(int argc, char* argv[]){
    int opt, options_index = 0;
    static struct option long_options[] = {
        {"fenetre",       required_argument, 0,  'f' },
        {"gen",           no_argument, 0,  'g' },
        {"shape",         required_argument, 0, 's'},
        {"rayon",         required_argument, 0, 'r'},
        {"nbpoints",      required_argument, 0, 'n'},
        {"concentration", required_argument, 0, 'c'},
        {"progressif",    no_argument, 0, 'p'},
        {"animation",     no_argument, 0, 'a'},
        {"velocite",      required_argument, 0, 'v'},
        {"maxpar",        required_argument, 0,  'm' },
        {"taillemin",     required_argument, 0,  't' },
        {0, 0, 0, 0},
    };

    while ((opt = getopt_long_only(argc, argv, "agps:f:r:n:c:v:m:t:", long_options, &options_index)) != -1){
        switch (opt){
            case 'f':
                if ((params.window.width = ((params.window.height) = atoi(optarg))) <= 0 ||
                     count_bits1(params.window.width) != 1) {
                    fprintf(stderr, "La taille de la fenêtre doit être strictement positive, "
                                    "et être une puissance de 2.\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'g':
                params.gen.enabled = true;
                break;

            case 's':
                if (strcmp(optarg, "carre") == 0)
                    params.gen.shape = CARRE;

                else if (strcmp(optarg, "cercle") == 0)
                    params.gen.shape = CERCLE;

                else {
                    fprintf(stderr, "Erreur, seul les arguments \"cercle\" et \"carre\" "
                                    "sont acceptés avec le flag -s\n");
                    exit(EXIT_FAILURE);
                }
                params.gen.enabled = true;
                break;

            case 'r':
                if ((params.gen.rayon = atoi(optarg)) <= 0) {
                    fprintf(stderr, "Veuillez entrer un rayon strictement positif.\n");
                    exit(EXIT_FAILURE);
                }
                params.gen.enabled = true;
                break;

            case 'n':
                if ((params.gen.nb_points = atoi(optarg)) <= 0) {
                    fprintf(stderr, "Veuillez demander au minimum 1 points à générer.\n");
                    exit(EXIT_FAILURE);
                }
                params.gen.enabled = true;
                break;

            case 'c':
                params.gen.concentration = atof(optarg);
                params.gen.enabled = true;
                break;

            case 'p':
                params.gen.progressif = true;
                params.gen.enabled = true;
                break;

            case 'a':
                params.gen.animation = true;
                params.gen.enabled = true;
                break;

            case 'v':
                params.gen.velocite = atoi(optarg);
                break;

            case 'm':
                if ((params.feuille.max_particules = atoi(optarg)) <= 0) {
                    fprintf(stderr, "Le nombre maximum de particules dans un noeud, "
                                    "doit être strictement positif.\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 't':
                if ((params.feuille.taille_min = atoi(optarg)) <= 0 ||
                     count_bits1(params.feuille.taille_min) != 1) {
                    fprintf(stderr, "La taille minimum d'un noeud, doit être positive, "
                                    "et être une puissance de 2.\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case '?':
            default:
                fprintf(stderr, "?? getopt returned character code %d ??\n", opt);
                exit(EXIT_FAILURE);
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
