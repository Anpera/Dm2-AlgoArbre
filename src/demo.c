#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <MLV/MLV_all.h>
#include <getopt.h>
#include <string.h>
#include "args.h"
#include "scenes.h"
#include "utils.h"

static Parameters params = {
    .window = {
        .width = 512,
        .height = 512,
    },
    .gen = {
        .enabled = false,
        .shape = CERCLE,
        .rayon = 0,
        .nb_points = 0,
        .concentration = 1.8,
        .tri = false,
        .pas_a_pas = false,
    },
    .feuille = {
        .max_particules = 5,
        .taille_min = 8,
        .velocite = 0,
    },
    .nb_clicks = 50,
};

void flags(int argc, char* argv[]){
    int opt, options_index = 0;
    static struct option long_options[] = {
        {"fenetre",       required_argument, 0, 'f'},
        {"maxpar",        required_argument, 0, 'k'},
        {"taillemin",     required_argument, 0, 'm'},
        {"nbclicks",      required_argument, 0, 's'},
        {"nbpoints",      required_argument, 0, 'n'},
        {"forme",         required_argument, 0, 'g'},
        {"rayon",         required_argument, 0, 'r'},
        {"concentration", required_argument, 0, 'c'},
        {"velocite",      required_argument, 0, 'v'},
        {"tri",           no_argument,       0, 't'},
        {"pas-a-pas",     no_argument,       0, 'p'},
        {0, 0, 0, 0},
    };

    while ((opt = getopt_long_only(argc, argv, "tpf:k:m:s:n:g:r:c:v:", long_options, &options_index)) != -1){
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
                if (strcmp(optarg, "carre") == 0)
                    params.gen.shape = CARRE;

                else if (strcmp(optarg, "cercle") == 0)
                    params.gen.shape = CERCLE;

                else {
                    fprintf(stderr, "Erreur, seul les arguments \"cercle\" et \"carre\" "
                                    "sont acceptés avec le flag -%c\n", opt);
                    exit(EXIT_FAILURE);
                }
                break;

            case 'r':
                if ((params.gen.rayon = atoi(optarg)) <= 0) {
                    fprintf(stderr, "Veuillez entrer un rayon strictement positif.\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'n':
                if ((params.gen.nb_points = atoi(optarg)) <= 0) {
                    fprintf(stderr, "Veuillez demander au minimum 1 points à générer.\n");
                    exit(EXIT_FAILURE);
                }
                params.gen.enabled = true;
                break;
            
            case 's':
                if ((params.nb_clicks = atoi(optarg)) <= 0) {
                    fprintf(stderr, "Veuillez demander au minimum 1 clic.\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'c':
                params.gen.concentration = atof(optarg);
                break;

            case 't':
                params.gen.tri = true;
                break;

            case 'p':
                params.gen.pas_a_pas = true;
                break;

            case 'v':
                params.feuille.velocite = atoi(optarg);
                break;

            case 'k':
                if ((params.feuille.max_particules = atoi(optarg)) <= 0) {
                    fprintf(stderr, "Le nombre maximum de particules dans un noeud, "
                                    "doit être strictement positif.\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'm':
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
        if (params.gen.rayon == 0)
            params.gen.rayon = (params.window.width - 5) / 2;
    }
}

int main(int argc, char* argv[]) {
    flags(argc, argv);
    MLV_create_window("QuadTree", "", params.window.width, params.window.height);
    SCN_Quadtree(params);
    MLV_free_window();
    exit(EXIT_SUCCESS);
}
