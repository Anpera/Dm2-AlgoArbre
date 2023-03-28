#ifndef H_UI
#define H_UI
#include <MLV/MLV_all.h>
#include <stdbool.h>
#include "Grille.h"

/**
 * @struct MLV_Ev
 * @brief Evènement souris
 */
typedef struct {
    int x; /**< Position x du clic */
    int y; /**< Position y du clic */
    MLV_Event type;
    MLV_Mouse_button button; /**< Type du clic : Gauche, Milleu, Droit */
    MLV_Button_state state; /**< Etat du clic */
    MLV_Keyboard_button key_btn; /**< Touche pressée */
} MLV_Ev;

enum ButtonType {NONE = 0, BAREBONE = 1, TEXT = 2, BOOLEAN = 4};

typedef struct {
    int width;
    int height;
} Size;

typedef struct {
    Rect relative;
    Rect absolute;
    MLV_Color back_color;
    bool invisible;
    Size size;
    int id;
} BareButton;

typedef struct {
    char* text;
    MLV_Font* font;
    MLV_Color text_color;
    bool hoverable;
    MLV_Color back_color_hovered;
} TextButton;

typedef struct {
    char* active_text;
    char* inactive_text;
    MLV_Font* font;
    MLV_Color text_color;
    MLV_Color active_color;
    MLV_Color active_color_hovered;
    MLV_Color inactive_color;
    MLV_Color inactive_color_hovered;
    int* value;
} BooleanButton;

typedef struct _Button {
    int type;
    BareButton bare;
    union {
        TextButton text_button;
        BooleanButton boolean_button;
    };
} Button;

typedef struct {
    Button* buttons;
    Grille* grille;
    int len;
    int capacity;
} Buttons;

/**
 * @brief Initialise une liste de boutons, qui seront
 * positionnés selon une grille.
 * Une grille est automatiquement initialisée avec n lignes
 * et p colonnes, si grille_base est nul. 
 * 
 * @param lignes Nombre de lignes de la grille à générer
 * @param colonnes Nombre de colonnes de la grille à générer
 * @param grille_pos Désigne la position absolue de la grille dans la fenêtre
 * @param grille_base Optionnel, permet d'utiliser une grille existante
 * @param carre Définit si la grille doit être carrée ou étirée par
 * rapport à la fenêtre
 * @return Buttons 
 */
Buttons UI_init_buttons(
    int lignes, int colonnes,
    Rect grille_pos, Grille* grille_base,
    bool carre
);

/**
 * @brief Libère la mémoire allouée pour une liste de boutons.
 * 
 * @param buttons 
 */
void UI_free_buttons(Buttons* buttons);

/**
 * @brief Ajoute un élément de type bouton à la liste.
 * 
 * @param buttons L'instance contenant la liste de boutons
 * @param button Le bouton à ajouter
 */
void UI_add_button(Buttons* buttons, Button button);

/**
 * @brief Teste si le bouton a été survolé.
 * 
 * @param mouse Evénement souris
 * @param b L'adresse du bouton
 * @return bool 
 */
bool UI_test_button(MLV_Ev mouse, Button* b);

/**
 * @brief Affiche un bouton.
 * 
 * @param button L'adresse du bouton
 * @param survole Indique si le bouton à été survolé,
 * et adapte sa couleur.
 */
void UI_draw_button(Button* button, bool survole);

/**
 * @brief Affiche les boutons de l'instance.
 * 
 * @param buttons L'adresse de l'instance de boutons
 * @param ev L'évenement souris
 * @return int 
 */
int UI_draw_buttons(Buttons* buttons, MLV_Ev ev);

/**
 * @brief Teste tous les boutons et renvoie l'identificateur de celui
 * ayant été cliqué. 
 * 
 * @param buttons L'adresse de l'instance de boutons
 * @param ev L'évenement souris
 * @return int L'identificateur du bouton, 0 sinon
 */
int UI_test_buttons(Buttons* buttons, MLV_Ev ev);

/**
 * @brief Attend un évènement et le renvoie
 * 
 * @return MLV_Ev 
 */
MLV_Ev UI_wait_ev();

/**
 * @brief Détermine si l'évènement souris est un clic
 * 
 */
#define IS_CLICK(ev) (ev.button == MLV_BUTTON_LEFT && ev.state == MLV_PRESSED)

/**
 * @brief Détermine si le bouton id_button_clicked correspond à id_button
 * 
 */
#define IS_BUTTON(id_button_clicked, id_button) (id_button_clicked == id_button)

/**
 * @brief Détermine si l'évènement souris ``ev`` est un clic
 * et si ``id_button_clicked`` correspond à ``id_button``
 * 
 */
#define IS_CLICKED(ev, id_button_clicked, id_button) (IS_CLICK(ev) && (IS_BUTTON(id_button_clicked, id_button)))


// Ces macros permettent d'initaliser des structures
// de boutons avec des valeurs par défaut.

#define DEFAULT_BARE(...) ((BareButton) { \
        .back_color = MLV_COLOR_GREY, \
        __VA_ARGS__ \
    })

#define COLOR_TRANSPARENT MLV_rgba(0, 0, 0, 0)

#define DEFAULT_TEXT(...) ((TextButton) { \
        .text_color         = MLV_COLOR_BLACK, \
        .back_color_hovered = MLV_COLOR_GREY50, \
        .hoverable = 1, \
        __VA_ARGS__ \
    })

#define DEFAULT_BOOLEAN(...) ((BooleanButton) { \
        .text_color             = MLV_COLOR_BLACK, \
        .active_color           = MLV_rgba(10, 128, 41, 255), \
        .active_color_hovered   = MLV_rgba(11, 79, 52, 255), \
        .inactive_color         = MLV_rgba(207, 14, 14, 255), \
        .inactive_color_hovered = MLV_rgba(148, 16, 16, 255), \
        __VA_ARGS__ \
    })

#endif
