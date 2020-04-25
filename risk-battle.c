#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mt19937.h"

typedef char ball;

#define NBALLS (75)
#define BDIM (5)

int col_ball_p(ball b, int col) {
     ball low = (NBALLS / BDIM) * col;
     ball high = low + (NBALLS / BDIM);

     return (b >= low) && (b < high);
}
              
struct ball_info_t {
     ball b;
     int t;
};

void shuffle_balls(ball balls[]) {
     int ii;

     for(ii = 0; ii < NBALLS; ii++) {
          int index = mt19937_int31() % NBALLS;

          ball tmp = balls[NBALLS - ii - 1];
          balls[NBALLS - ii - 1] = balls[index];
          balls[index] = tmp;
     }

     /*
     printf("after: ");     
     for(ii = 0; ii < NBALLS; ii++) {
          printf("%d, ", (int)balls[ii]);
     }
     printf("\n");
     */
}


struct balls_t {
     int ndrawn;
     ball balls[NBALLS];
};

void init_balls(struct balls_t *b) {
     int ii;
     struct ball_info_t bis[NBALLS];

     for(ii = 0; ii < NBALLS; ii++) {
          b->balls[ii] = ii;
     }

     shuffle_balls(b->balls);
     
     b->ndrawn = 0;
}

ball draw_ball(struct balls_t *bs) {
     if (bs->ndrawn >= NBALLS) {
          return -1;
     }
     
     return bs->balls[bs->ndrawn++];
}

struct board_t {
     ball squares[BDIM * BDIM];
     unsigned int marks;
};

int board_ofs(int x, int y) {
     return x + (y * BDIM);
}

int board_mask(int x, int y) {
     return 0x1 << board_ofs(x, y);
}

void print_board(struct board_t *b) {
     printf(" B   I   N   G   O\n");

     for(int y = 0; y < BDIM; y++) {
          for(int x = 0; x < BDIM; x++) {
               printf("%02d%c ",
                      b->squares[board_ofs(x, y)],
                      ((b->marks & board_mask(x, y)) != 0) ? '*' : ' ');
          }
          printf("\n");
     }
}

void generate_board(struct board_t *board) {
     struct balls_t bs;

     init_balls(&bs);
     
     for(int x = 0; x < BDIM; x++) {
          bs.ndrawn = 0;
          
          for(int y = 0; y < BDIM; y++) {
               ball b = -1;

               while(!col_ball_p(b, x)) {
                    b = draw_ball(&bs);
               }
               
               board->squares[board_ofs(x, y)] = b;
          }
     }
}

void reset_board(struct board_t *board) {
     board->marks = 0;
}

ball board_step(struct balls_t *balls, struct board_t *board) {
     ball b = draw_ball(balls);
     
     for(int ii = 0; ii < BDIM * BDIM; ii++) {
          if (board->squares[ii] == b) {
               board->marks |= (0x1 << ii);
               break;
          }
     }

     /*
     printf(">>> step %d, ball %d\n", balls->ndrawn, (int)b);
     print_board(board);
     */
     
     return b;
}

int board_solved_p(struct board_t *board) {
     return board->marks == 0x01FFFFFF;
}
     
int play_game(struct board_t *board) {
     struct balls_t balls;
     init_balls(&balls);

     int step = 0;
     
     while(!board_solved_p(board)) {
          if (board_step(&balls, board) < 0) {
               break;
          }

          step++;
     }

     return step;
}

int main(int argc, char *argv[]) {
     int ii;
     init_mt19937(0);

     int counts[NBALLS + 1];
     memset(counts, 0, sizeof(counts));

     struct board_t b;

     for(ii = 0; ii < 20 * 100000000; ii++) {
          if (ii % 1000000 == 0) {
               printf("%d\n", ii);
          }
          
          generate_board(&b);
          reset_board(&b);
          int steps = play_game(&b);

          counts[steps]++;
     }

     for(ii = BDIM * BDIM; ii <= NBALLS; ii++) {
          printf("%d, %d\n", ii, counts[ii]);
     }
     
     fprintf(stderr, "end run.\n");
}
