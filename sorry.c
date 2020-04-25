#include <stdio.h>

#include "mt19937.h"

/*
 * 5x 1 - forward 1 or pawn out
 * 4x 2 - forward 2 or pawn out (play again)
 * 4x 3 - forward 3
 * 4x 4 - backward 4
 * 4x 5 - forward 5
 * 4x 7 - forward 7 (split between two pawns)
 * 4x 8 - forward 8
 * 4x 10 - forward 10 or backward 1
 * 4x 11 - forward 11 or swap pawns
 * 4x 12 - forward 12
 * 4x sorry - pawn out to opponent's pawn
 */

#define NUM_PLAYERS 4
#define NUM_PAWNS 4

/* Board side:
 *
 * 0 : blank
 * 1 : begin slide
 * 2 : diamond/slide
 * 3 : slide
 * 4 : slide end/start
 * 5 : blank
 * 6 : blank
 * 7 : blank
 * 8 : blank
 * 9 : begin slide
 * 10: slide
 * 11: slide
 * 12: slide
 * 13: end slide
 * 14: blank.
 */
struct game_state {
     int pawn_locs[NUM_PLAYERS * NUM_PAWNS];
};


int main(int argc, char *argv[]) {
     printf("end run.\n");

     return 0;
}
