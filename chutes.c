#include <stdio.h>
#include <string.h>

#include <inttypes.h>

#include "mt19937.h"

#define GAME_COUNT (100000) //000)

#define HISTORY_LENGTH (512)

#define BOARD_SIZE (100)

typedef unsigned int board_pos_t;

static board_pos_t warps[BOARD_SIZE];
static int counts[BOARD_SIZE][HISTORY_LENGTH];

void init() {
     memset(counts, 0, sizeof(counts));

     memset(warps, 0, sizeof(warps));

     warps[1]  = 38;
     warps[4]  = 14;
     warps[9]  = 32;
     warps[16] = 6;
     warps[21] = 42;
     warps[28] = 84;
     warps[36] = 44;
     warps[48] = 26;
     warps[49] = 11;
     warps[51] = 67;
     warps[62] = 19;
     warps[64] = 60;
     warps[71] = 91;
     warps[80] = 100;
     warps[87] = 24;
     warps[93] = 73;
     warps[95] = 75;
     warps[98] = 78;
}

int roll_die() {
     return (mt19937_int31() % 6) + 1;
}

int game_sim(int buf_len, int game_buf[]) {
     int move = 0;

     board_pos_t location = 0;

     while (location < BOARD_SIZE) {
          if (move >= buf_len) {
               return -1;
          }

          int die = roll_die();

          game_buf[move] = location;
          move++;

          board_pos_t new_location;

          new_location = location + die;

          if (new_location > BOARD_SIZE) {
               continue;
          }

          if (warps[new_location]) {
               location = warps[new_location];
          } else {
               location = new_location;
          }
     }

     return move;
}

void update_durations(int game_history_len, int game_history[]) {
     for(int ii = game_history_len - 1; ii >= 0; ii--) {
          counts[game_history[ii]][game_history_len - ii]++;
     }
}

void show_durations() {
     /* Print a table that has a row for each position on the board
      * and a column for the number of plays remaining in the
      * game. Each cell shows how many games took that length from
      * that position.
      */
     for(int bpos = 0; bpos < BOARD_SIZE; bpos++) {
          for(int len = 0; len < HISTORY_LENGTH; len++) {
               printf("%d,", counts[bpos][len]);
          }
          printf("\n");
     }
}

int main(int argc, char *argv[]) {
     init();

     int overrun_count = 0;

     for(int ii = 0; ii < GAME_COUNT; ii++) {
          int game_history[HISTORY_LENGTH];

          int game_history_len = game_sim(HISTORY_LENGTH, game_history);

          if (game_history_len < 0) {
               overrun_count++;
          } else {
               update_durations(game_history_len, game_history);
          }
     }

     show_durations();

     if (overrun_count > 0) {
          printf("\n%d game history overrun%s!\n", overrun_count, (overrun_count == 1) ? "" : "s");
          return 1;
     }

     return 0;
}
