#ifndef H_ARGS
#define H_ARGS

enum Shape {CARRE, CERCLE};
typedef struct {
    struct {
        int enabled;          /*< true : on génère les points, false : clic souris */
        enum Shape shape;     /*< Génération carée ou dans un cercle */
        int nb_points;        /*< Nombre de points à générer */
        int rayon;            /*< Rayon de la génération */
        double concentration; /*< Effet de concentration pour le mode cercle */
        int tri;              /*< true : Trie les points générés par rapport à leur distance au centre */
        int pas_a_pas;        /*< true : Redessine à chaque ajout */
        int velocite;         /*< Vitesse de déplacement */
    } gen;
    struct {
        int width;
        int height;
    } window;
    
    struct {
        int max_particules; /**< Nombre max de particule dans une feuille (Kp) */
        int taille_min;     /**< Taille minimale du carré (puissance de 4) */
    } feuille;

    int nb_clicks; /**< Nombre de particules pouvant être ajouté par l'utilisateur */

} Parameters;

void print_help(void);
Parameters parse_args(int argc, char *argv[]);

#endif
