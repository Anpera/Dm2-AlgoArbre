#include "utils.h"
#include "math.h"

int count_bits1(int test) {
    return __builtin_popcountll(test);
}

inline unsigned short log2ll(unsigned long long int n) {
    unsigned short tmp = 0;
    while (n >>= 1) ++tmp;
    return tmp;
}

inline unsigned long int pow4ll(unsigned short int n) {
    return (1 << (2 * n));
}

double distance_euclidienne(int ax, int ay, int bx, int by) {
    return sqrt(CARRE(by - ay) + CARRE(bx - ax));
}
