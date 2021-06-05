#include "common.h"

#include "mt19937.h"

char roll_dice() {
     long x = mt19937_int31();

     return (char)(x % 6) + 1;
}
