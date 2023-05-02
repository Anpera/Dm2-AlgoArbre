#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED

#include <math.h>

/**
 * @brief Compte le nombre de bits à 1 d'un nombre
 * binaire.
 * 
 * @param test 
 * @return int 
 */
int count_bits1(int test);

/**
 * @brief Calcule le logarithme en base 2, d'un entier 64 bits.
 * 
 * @param n Entier 64 bits
 * @return unsigned short 
 */
unsigned short log2ll(unsigned long long int n);

/**
 * @brief Calcule 4^n.
 * 
 * @param n 
 * @return unsigned long int 
 */
unsigned long int pow4ll(unsigned short int n);

double distance_euclidienne(int ax, int ay, int bx, int by);

/**
 * @brief Limite le nombre n aux bornes min et max
 * 
 * @param min 
 * @param n 
 * @param max 
 */
void clamp(double min, double* n, double max);

#define ABS(x) (((x) < 0) ? -(x) : (x))

#define CARRE(a) ((a) * (a))

#define CUBE(a) (CARRE(a) * (a))

#endif
