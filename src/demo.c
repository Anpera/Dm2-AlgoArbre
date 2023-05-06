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
        .nb_points = 250,
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

void aled(void) {
    printf("Bienvenue sur le message d'aide. Vous trouverez ici\n"
            "une liste exhaustive de tous les arguments utilisables\n"
            "ainsi que leurs instructions d'utilisation.\n"
            "Ils seront saisissables selon la syntaxe suivante :\n\n"
            "-option_format_long=[parametre] / -o [parametre]\n\n"
            "Par exemple, le message \n"
            "\"-fenetre / -f [int]\" signifie que l'on peut soit\n"
            "utiliser le flag \"-fenetre\", soit son raccourci \"-f\"\n"
            "suivi de l'entier.\n\n");

    printf("-help / -aled / -h\n"
            "\tPermet d'afficher les différentes options mais également\n"
            "\tcomment les utiliser.\n"
            "\tA noter que cette option met fin à l'exécution du programme.\n\n");

    printf("-forme / -g [carre/cercle]\n"
            "\tActive la génération de particules aléatoires et\n"
            "\tdéfinit la forme de la génération.\n"
            "\tSeul \"carre\" et \"cercle\" sont acceptés.\n\n");

    printf("-rayon / -r [int]\n"
            "\tDéfinit le rayon de génération aléatoire de particules.\n"
            "\tL'entier int doit être strictement positif.\n\n");

    printf("-nbpoints / -n [int]\n"
            "\tDéfinit le nombre de points que générera la génération\n"
            "\taléatoire.\n"
            "\tL'entier int doit être strictement positif.\n"
            "\tPar défaut, 250.\n\n");

    printf("-nbclicks / -s [int]\n"
            "\tDéfinit le nombre de points que l'utilisateur peut générer\n"
            "\tmanuellement.\n"
            "\tL'entier int doit être strictement positif.\n"
            "\tPar défaut, 50 avec génération aléatoire,\n"
            "\tsinon 50 + nbpoints.\n\n");

    printf("-concentration / -c [float]\n"
            "\tDéfinit la concentration des points par rapport au centre\n"
            "\tde la génération aléatoire. Une valeur plus élevée donnera\n"
            "\tplus de points au centre.\n"
            "\tLe nombre à virgule float peut être autant négatif\n"
            "\tque compris entre 0 et 1, ou plus.\n"
            "\tPar défaut, 1.8.\n\n");

    printf("-tri / -t\n"
            "\ttrie les points de la génération aléatoire selon leur\n"
            "\tproximité avec le centre de la fenêtre.\n\n");

    printf("-pas-a-pas / -p\n"
            "\tMontre l'ajout des points de la génération aléatoire en\n"
            "\trafraîchissant l'affichage à chaque ajout.\n\n");

    printf("-velocite / -v [int]\n"
            "\tDéfinit une vitesse de déplacement pour tous les points\n"
            "\tajoutés dans le programme, générer aléatoirement ou définie\n"
            "\tpar l'utilisateur.\n"
            "\tL'entier int doit simplement être un entier.\n\n");

    printf("-fenetre / -f [int]\n"
            "\tDéfinit la taille de la fenêtre.\n"
            "\tL'entier [int] doit être strictement positif et\n"
            "\tPar défaut, 512.\n\n");

    printf("-maxpar / -k [int]\n"
            "\tDéfinit le nombre maximum de particules dans un noeud.\n"
            "\tL'entier [int] doit être strictement positif.\n\n"
            "\tPar défaut, 5.\n\n");
        
    printf("-taillemin / -m [int]\n"
            "\tDéfinit la taille minimale d'un noeud en pixels.\n"
            "\tL'entier [int] doit être strictement positif.\n"
            "\tPar défaut, 8.\n\n");

    printf("Pour créer un point, faites un clic gauche.\n"
        "Pour déplacer un point, maintenez le clic droit sur un\n"
        "point et relâchez la souris là où vous voulez déposer le point.\n"
        "Pour quitter le programme, appuyer sur la touche échap.\n");
}

void flags(int argc, char* argv[]){
    int opt, options_index = 0;
    static struct option long_options[] = {
        {"fenetre",       required_argument, 0, 'f'},
        {"forme",         required_argument, 0, 'g'},
        {"rayon",         required_argument, 0, 'r'},
        {"nbpoints",      required_argument, 0, 'n'},
        {"nbclicks",      required_argument, 0, 's'},
        {"concentration", required_argument, 0, 'c'},
        {"maxpar",        required_argument, 0, 'k'},
        {"taillemin",     required_argument, 0, 'm'},
        {"velocite",      required_argument, 0, 'v'},
        {"tri",           no_argument,       0, 't'},
        {"pas-a-pas",     no_argument,       0, 'p'},
        {"help",          no_argument,       0, 'h'},
        {"aled",          no_argument,       0, 'h'},
        {0, 0, 0, 0},
    };

    while ((opt = getopt_long_only(argc, argv, "tphf:k:m:s:n:g:r:c:v:", long_options, &options_index)) != -1){
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
                params.gen.enabled = true;
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

            case 'h':
                aled();
                exit(EXIT_SUCCESS);

            case '?':
            default:
                fprintf(stderr, "?? getopt returned character code %d ??\n", opt);
                aled();
                exit(EXIT_FAILURE);
        }
    }
    if (params.gen.rayon == 0)
        params.gen.rayon = (params.window.width) / 2;
}

int main(int argc, char* argv[]) {
    flags(argc, argv);
    MLV_create_window("QuadTree", "", params.window.width, params.window.height);
    SCN_Quadtree(params);
    MLV_free_window();
    exit(EXIT_SUCCESS);
}
