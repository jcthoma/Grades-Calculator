/* Jeremy Thomas*/
/*UID: 118511054*/
/*jthoma38*/

#include <stdio.h>
#include <limits.h>
#include <math.h>
#define MAX_ASSIGNS 50
static double compute_score(int array[], int num, int late, int drop);
static void drop_scores(int array[], int num, int drop);
static double find_mean(int array[], int num_assigns, int late);
static double find_std_dev(int array[], int num_assigns, int late);
/*static void print_output(int arr[], int num, int late, int drop, char stats);*/

#include <limits.h>

void drop_scores(int array[], int num, int drop) {
   int x, min_index, temp, count;

   /* sort array in ascending order based on scores */
   for (count = 0; count < num - 1; count++) {
      min_index = count;
      for (x = count + 1; x < num; x++) {
         /* compare scores and weight at index x and min_index */
         if (array[x * 4 + 1] * array[x * 4 + 2] <
	     array[min_index * 4 + 1] * array[min_index * 4 + 2]) {
            min_index = x;
         }
      }
      /* swap the elements to move smallest score to the front */
      for (x = 0; x < 4; x++) {
         temp = array[count * 4 + x];
         array[count * 4 + x] = array[min_index * 4 + x];
         array[min_index * 4 + x] = temp;
      }
   }

   /* set the weight of the lowest assignment scores to 0 */
   for (count = 0; count < drop; count++) {
      array[count * 4 + 2] = 0;
   }
}




static double find_mean(int array[], int num_assigns, int late) {
   int x;
   double mean, sum_scores = 0;

   /* compute the mean of all assign scores */
   for (x = 0; x < (num_assigns * 4); x += 4) {
      sum_scores += (array[x+1] - (array[x+3])*late);
   }
   mean = (sum_scores / num_assigns);
   return mean;
}

static double find_std_dev(int array[], int num_assigns, int late) {
   int x, score;
   double mean, sum = 0, variance, standard_dev;
   mean = find_mean(array, num_assigns, late);

   /* compute variance and standard deviation */
   for (x = 0; x < (num_assigns * 4); x += 4) {
      score = array[x+1] - (array[x+3] * late);
      sum += pow((score - mean), 2);
   }
   variance = sum / (float) num_assigns;
   standard_dev = sqrt(variance);
   
   return standard_dev;
}


static double compute_score(int array[], int num, int late, int drop) {
   int x, assign_late;
   double weight_sum = 0, score = 0;

   /* drop "x number" of lower scores, if drop > 0 */
   if (drop > 0) {
      drop_scores(array, num, drop);
   }

   /* get the weight sum, in case assignments were dropped */
   for (x = 0; x < (num*4); x+=4) {
	 weight_sum += array[x+2];
   }

   /* check that the weight sum isn't over 100 */
   if (weight_sum > 100.0) {
      printf("ERROR: Invalid values provided\n");
      return -1;
   }
   else {
      /* calculate each numeric score */
      for (x = 0; x < (num*4); x+=4) {

	 assign_late = array[x+1] - array[x+3] * late;
	 if (assign_late > 0) {
	    score += assign_late * (array[x+2] / weight_sum);
	 }
      }
   }
   return score;
}

int main(void) {
  int x = 0, penalty, dropped, num_assigns, count = 1;
   int arr[MAX_ASSIGNS * 4] = {0};
   int arr2[MAX_ASSIGNS * 4] = {0};
   double score, mean = 0, sd = 0;
   char stats;

   /* 1. scan and store the assignment info from stdin */
   scanf("%d %d %c\n", &penalty, &dropped, &stats);
   scanf("%d\n", &num_assigns);

   while (x < (num_assigns * 4)) {
      scanf("%d,%d,%d,%d\n", &arr[x], &arr[x+1], &arr[x+2], &arr[x+3]);
      x = x + 4;
   }

   


   /* compute numeric score using a copy of the original array */
   for (x = 0; x < (num_assigns * 4); x++) {
      arr2[x] = arr[x];
   }
   if (dropped >= 0 && dropped < num_assigns)
      score = compute_score(arr2, num_assigns, penalty, dropped);
   else score = 0;

   /* print Numeric Score, Late Penalty Points, and Assigns Dropped */
   printf("Numeric Score: %5.4f\n", score);
   printf("Points Penalty Per Day Late: %d\n", penalty);
   printf("Number of Assignments Dropped: %d\n", dropped);

   /* print log of assignments submitted */
   printf("Values Provided:\nAssignment, Score, Weight, Days Late\n");

   /* print in groups of four from the next largest num */
   while (count <= num_assigns) {

      for (x = 0; x < (num_assigns * 4); x += 4) {
         if (arr[x] == count) {
            printf("%d, %d, %d, %d\n", arr[x], arr[x+1], arr[x+2], arr[x+3]);
         }
      }
      count = count + 1;
   }

   /* print extra stats if the input character was yes (Y or y */
   if (stats == 'Y' || stats == 'y') {
      mean = find_mean(arr, num_assigns, penalty);
      sd = find_std_dev(arr, num_assigns, penalty);
      printf("Mean: %5.4f, Standard Deviation: %5.4f\n", mean, sd);
   }

   return 0;
}

