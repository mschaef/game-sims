#include <stdio.h>
#include <stdio.h>

#include "common.h"

#include "mt19937.h"

char roll_1d6() {
     long x = mt19937_int31();

     return (char)(x % 6) + 1;
}


int main(int argc, char *argv[]) {
     init_mt19937(0);

     int rc = sim_main();
     
     fprintf(stderr, "end run.\n");

     return rc;
}
