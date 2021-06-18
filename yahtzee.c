#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common.h"

#define NDICE 5

#define NHANDS 2000000000

void roll_hand(char dice[NDICE]) {
     for(int ii = 0; ii < NDICE; ii++) {
          dice[ii] = roll_1d6();
     }
}

void improve_chance_hand(char dice[NDICE], int threshold) {
     for(int ii = 0; ii < NDICE; ii++) {
          if(dice[ii] < threshold) {
               dice[ii] = roll_1d6();
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

int sim_main() {
     int scores[31][7];

     memset(scores, 0, sizeof(scores));

     char original_hand[NDICE];
     char current_hand[NDICE];

     for(int ii = 0; ii < NHANDS; ii++) {

          if (ii % 2000000 == 0) {
               fprintf(stderr, "[%d/%d]\n", ii, NHANDS);
          }
          
          roll_hand(original_hand);
          memcpy(current_hand, original_hand, sizeof(original_hand));
          
          DEBUG_MSG(show_hand(current_hand));


          int score = hand_chance_score(current_hand);

          DEBUG_MSG(printf("%d\n", score));

          scores[score][0]++;

          for(int jj = 0; jj < 6; jj++) {
               memcpy(current_hand, original_hand, sizeof(original_hand));

               improve_chance_hand(current_hand, jj + 1);
               improve_chance_hand(current_hand, jj + 1);

               score = hand_chance_score(current_hand);
          
               scores[score][jj + 1]++;
          }
     }

     for(int ii = 0; ii < 31; ii++) {
          printf("%d, ", ii);

          for(int jj = 0; jj < 7; jj++) {
               printf("%d, ", scores[ii][jj]);
          }

          printf("\n");
     }

     return 0;
}

