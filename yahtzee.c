#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mt19937.h"
#include "common.h"

#define NDICE 5

void roll_hand(char dice[NDICE]) {
     for(int ii = 0; ii < NDICE; ii++) {
          dice[ii] = roll_dice();
     }
}

void improve_chance_hand(char dice[NDICE]) {
     for(int ii = 0; ii < NDICE; ii++) {
          if(dice[ii] < 4) {
               dice[ii] = roll_dice();
          }
     }
}

void show_hand(char dice[NDICE]) {
     for(int ii = 0; ii < NDICE; ii++) {
          printf("%d", dice[ii]);

          if (ii < NDICE - 1) {
               printf(",");
          }
     }
     printf("\n");
}

int hand_chance_score(char dice[NDICE]) {
     int score = 0;

     for(int ii = 0; ii < NDICE; ii++) {
          score += dice[ii];
     }

     return score;
}

int main(int argc, char *argv[]) {
     init_mt19937(0);

     int scores[31];
     int improved_scores[31];

     memset(scores, 0, sizeof(scores));
     memset(improved_scores, 0, sizeof(improved_scores));

     char dice[NDICE];

     for(int ii = 0; ii < 100000; ii++) {
          roll_hand(dice);
          DEBUG_MSG(show_hand(dice));

          int score = hand_chance_score(dice);

          DEBUG_MSG(printf("%d\n", score));

          scores[score]++;

          improve_chance_hand(dice);
          improve_chance_hand(dice);

          score = hand_chance_score(dice);
          
          improved_scores[score]++;
     }

     for(int ii = 0; ii < 31; ii++) {
          printf("%d, %d, %d\n", ii, scores[ii], improved_scores[ii]);
     }
     
     fprintf(stderr, "end run.\n");
}