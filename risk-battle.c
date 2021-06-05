#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mt19937.h"
#include "common.h"

#define DEBUG 0
#define TRACE 0

#define SAMPLE_RUNS 10000000
#define TROOP_RANGE 40

#if DEBUG
#  define DEBUG_MSG(x) x
# else
#  define DEBUG_MSG(x)
#endif

#if TRACE
#  define TRACE_MSG(x) x
# else
#  define TRACE_MSG(x)
#endif


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

     TRACE_MSG(show_dice("attacker", a_die));
     TRACE_MSG(show_dice("defender", d_die));

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

struct simulation_result {
     int attacker_wins;
     int defender_wins;
     int draws;
     int attacker_troop_losses;
     int defender_troop_losses;

     double attacker_win_prob;
     double draw_prob;
     double attacker_avg_losses;
     double defender_avg_losses;
};

void compute_stats(struct simulation_result *result) {
     result->attacker_win_prob = (double)result->attacker_wins / (double)SAMPLE_RUNS;
     result->draw_prob = (double)result->draws / (double)SAMPLE_RUNS;

     result->attacker_avg_losses = (double)result->attacker_troop_losses / (double)SAMPLE_RUNS;
     result->defender_avg_losses = (double)result->defender_troop_losses / (double)SAMPLE_RUNS;
}

void update_result(char *label,
                   struct simulation_result *result,
                   int initial_attackers, int initial_defenders,
                   int attackers, int defenders) {
     result->attacker_troop_losses += (initial_attackers - attackers);
     result->defender_troop_losses += (initial_defenders - defenders);

     if (!attackers) {
          DEBUG_MSG(printf("  %s defender wins\n", label));
          result->defender_wins++;
     } else if (!defenders) {
          DEBUG_MSG(printf("  %s attacker wins\n", label));
          result->attacker_wins++;
     } else {
          DEBUG_MSG(printf("  %s draw\n", label));
          result->draws++;
     }
}

void simulate_games(int initial_attackers, int initial_defenders,
                    struct simulation_result *result,
                    struct simulation_result *c_result) {

     memset(result, 0, sizeof(struct simulation_result));
     memset(c_result, 0, sizeof(struct simulation_result));

     for(int ii = 0; ii < SAMPLE_RUNS; ii++) {
          DEBUG_MSG(printf("\n"));

          int attackers = initial_attackers;
          int defenders = initial_defenders;

          int c_attackers = initial_attackers;
          int c_defenders = initial_defenders;

          int stopped = 0;

          while(attackers && defenders) {
               DEBUG_MSG(printf("a: %d, d: %d\n", attackers, defenders));

               if (attackers < defenders) {
                    stopped = 1;
               }

               evaluate(&attackers, &defenders);

               if (!stopped) {
                    c_attackers = attackers;
                    c_defenders = defenders;
               }
          }

          update_result("total", result, initial_attackers, initial_defenders, attackers, defenders);
          update_result("cautious", c_result, initial_attackers, initial_defenders, c_attackers, c_defenders);
     }

     DEBUG_MSG(printf("att/def wins: %d/%d (%d/%d)\n",
                      result->attacker_wins, result->defender_wins,
                      c_result->attacker_wins, c_result->defender_wins));


     compute_stats(result);
     compute_stats(c_result);
}

void sample(int attackers, int defenders) {
     struct simulation_result result;
     struct simulation_result c_result;

     simulate_games(attackers, defenders, &result, &c_result);

     printf("%d, %d, %0.4f, %0.4f, %0.4f, %0.4f, %0.4f, %0.4f, %0.4f, %0.4f\n",
            attackers, defenders,
            result.attacker_win_prob,
            result.draw_prob,
            result.attacker_avg_losses,
            result.defender_avg_losses,
            c_result.attacker_win_prob,
            c_result.draw_prob,
            c_result.attacker_avg_losses,
            c_result.defender_avg_losses);
}

int main(int argc, char *argv[]) {
     init_mt19937(0);

     for(int attackers = 1; attackers < TROOP_RANGE; attackers++) {
          for(int defenders = 1; defenders < TROOP_RANGE; defenders++) {
               sample(attackers, defenders);
          }
     }

     fprintf(stderr, "end run.\n");
}
