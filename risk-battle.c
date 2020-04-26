#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mt19937.h"

#define DEBUG_MSG(x)

#define SAMPLE_RUNS 1000000
#define TROOP_RANGE 20

char roll_dice() {
     long x = mt19937_int31();

     return (char)(x % 6) + 1;
}

void sort_step(char *a, char *b) {
     if (*a < *b) {
          char temp = *b;
          *b = *a;
          *a = temp;
     }
}
     
void roll_hand(int die_rolled, int num_die, char dice[]) {
     for(int ii = 0; ii < num_die; ii++) {
          dice[ii] = (ii < die_rolled) ? roll_dice() : -1;
     }

     // num_die bounded by sorting network capacity.
     assert(num_die < 4);
     
     sort_step(&dice[0], &dice[1]);
     sort_step(&dice[1], &dice[2]);
     sort_step(&dice[0], &dice[1]);
     sort_step(&dice[1], &dice[2]);
}

int min(int a, int b) {
     return (a > b) ? b : a;
}

void show_dice(char *label, char die[]) {
     printf(" %s > %d, %d, %d\n", label, (int)die[0], (int)die[1], (int)die[2]);
}

void evaluate(int *attackers, int *defenders) {
     char a_die[3];
     char d_die[3];

     roll_hand(min(3, *attackers), 3, a_die);
     roll_hand(min(2, *defenders), 3, d_die);

     DEBUG_MSG(show_dice("attacker", a_die));
     DEBUG_MSG(show_dice("defender", d_die));

     for(int ii = 0; ii < 3; ii++) {
          if ((a_die[ii] == -1) || (d_die[ii] == -1)) {
               return;
          }
          
          if (a_die[ii] > d_die[ii]) {
               *defenders = *defenders - 1;
          } else if (a_die[ii] <= d_die[ii]) {
               *attackers = *attackers - 1;
          }
     }
}

void simulate_games(int _attackers, int _defenders,
                    double *attacker_win_prob,
                    double *attacker_avg_losses,
                    double *defender_avg_losses) {
     
     int attacker_wins = 0;
     int defender_wins = 0;

     int attacker_troop_losses = 0;
     int defender_troop_losses = 0;
     
     for(int ii = 0; ii < SAMPLE_RUNS; ii++) {
          DEBUG_MSG(printf("\n"));

          int attackers = _attackers;
          int defenders = _defenders;

          int count = 100;

          while(attackers && defenders && count) {
               evaluate(&attackers, &defenders);

               DEBUG_MSG(printf("a: %d, d: %d\n", attackers, defenders));

               count--;
          }

          attacker_troop_losses += (_attackers - attackers);
          defender_troop_losses += (_defenders - defenders);

          if (!attackers) {
               defender_wins++;
          } else if (!defenders) {
               attacker_wins++;
          }
     }

     DEBUG_MSG(printf("att/def wins: %d/%d\n", attacker_wins, defender_wins));
     
     *attacker_win_prob =
          (double)attacker_wins / ((double)attacker_wins + (double)defender_wins);

     *attacker_avg_losses = (double)attacker_troop_losses / (double)SAMPLE_RUNS;
     *defender_avg_losses = (double)defender_troop_losses / (double)SAMPLE_RUNS;
}

int main(int argc, char *argv[]) {
     init_mt19937(0);

     for(int attackers = 1; attackers < TROOP_RANGE; attackers++) {
          for(int defenders = 1; defenders < TROOP_RANGE; defenders++) {
               double p_awin;
               double avg_aloss;
               double avg_dloss;

               simulate_games(attackers, defenders, &p_awin, &avg_aloss, &avg_dloss);

               printf("%d,%d,%0.4f,%0.4f,%0.4f\n", attackers, defenders, p_awin, avg_aloss, avg_dloss);
          }
     }
     
     fprintf(stderr, "end run.\n");
}
