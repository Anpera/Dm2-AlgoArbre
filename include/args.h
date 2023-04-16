#ifndef H_ARGS
#define H_ARGS

enum Shape {CARRE, CERCLE};
typedef struct {
    struct {
        int enabled;          /**< true : Génère les points, false : clic souris seulement */
        enum Shape shape;     /**< Génération carée ou cercle */
        int nb_points;        /**< Nombre de points à générer */
        int rayon;            /**< Rayon de la génération */
        double concentration; /**< Effet de concentration pour le mode cercle */
        int tri;              /**< true : Trie les points générés par rapport à leur distance au centre */
        int pas_a_pas;        /**< true : Redessine à chaque ajout, false : affiche directement */
    } gen;
    struct {
        int width;
        int height;
    } window;
    
    struct {
        int max_particules; /**< Nombre maximum de particules dans une feuille (Kp) */
        int taille_min;     /**< Taille minimale d'un carré (puissance de 4) */
        int velocite;       /**< Vitesse de déplacement */
    } feuille;

    int nb_clicks; /**< Nombre de particules supplémentaire pouvant être ajouté par l'utilisateur */

} Parameters;

#endif
