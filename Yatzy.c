/*Name: -------------->   Alija Cerimagic        */
/*E-mail: ------------>   acer21@student.aau.dk  */
/*Group Number: ------>   Group B224             */
/*Field of Study: ---->   Software               */
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>

int prompt_user(int amount_dice);
void flush_input(void);
void calculator(int amount_dice);
void roll_dice(int amount_dice, int *rand_numbers);
void caller(int amount_dice, int *rand_numbers, int gamemode, int *results);
int upper(int amount_dice, int *rand_numbers, int gamemode);
int s_pair(int amount_dice, int *rand_numbers, int size_comp, int skip_num);
int straights(int amount_dice, int *rand_numbers, int small_or_big);
int f_chance(int amount_dice, int *rand_numbers);
/*The enum  creates a better overview in the program code while functioning as an array of integers*/
enum game_mode {ones, twos, threes, fours, fives, sixes, bonus, one_pair, two_pairs, three_kind,
                four_kind, full_house, small_straight, large_straight, chance, yatzy, e_sum};

int main(void){
  int j = 0, amount_dice = 0;
  char i = 'y';
  /*main-funtion runs the main functions and asked the user to run the program again (generally controls this function)*/
  while(i != 'n'){
    if(j == 1){
      printf("\nDo you want to play again - y/n?: ");
      scanf("%c", &i);
      flush_input();
    }
    if(i != 'n'){
      amount_dice = prompt_user(amount_dice);
      srand(time(NULL));
      calculator(amount_dice);
      j = 1;
    } else{
      printf("\nThank you for playing!\n");
    }
  }
  
  return 0;
}

int prompt_user(int amount_dice){
  amount_dice = 0;
  /*Promt user for the amount of dices to use in the game*/
  while(amount_dice < 5){
    printf("How many dice do you want to use (minimum of 5): ");
    scanf(" %d", &amount_dice);
    flush_input();
  }
  /*Input validation is made to prevent error by entering letters of other not supported characters*/
  return amount_dice;
}

void flush_input(void){
  while((getchar()) != '\n');
}

void calculator(int amount_dice){
  int *rand_numbers, i, c, m = 0, j = 0, sum = 0;
  enum game_mode gamemode;
  /*Here we define two arrays - one for the results of the gamemodes (caller()) and one for the display name of gamemodes*/
  int results[16];
  char game_name[17][19] = {"Ones:", "Twos:", "Threes:", "Fours:", "Fives:", "Sixes:", "BONUS:", 
                            "One Pair:", "Two Pairs:", "Three Kind:", "Four Kind:", "Full House:",
                            "Small Straight:", "Large Straight:", "Chance:", "Yatzy:", "SUM"};

  /*Dynamically allocate memory*/
  rand_numbers = (int*)malloc(amount_dice * sizeof(int));
  /*These are printf functions for interface headers*/
  printf("\nGAME MODE         DICE ROLLS");
  for(m = 0 ; m < ((amount_dice * 2) - 6) ; m++){
    printf(" ");
  }
  printf("|POINTS");
  printf(" |ACCUMULATED SUM");
  /*This is the for-loop that makes the game go through all the gamemodes*/
  printf("\n");
  for(gamemode = ones ; gamemode <= e_sum ; gamemode++){
    /*This call randomizez the numbers before each gamemode is performed*/
    roll_dice(amount_dice, rand_numbers);
    /*More printf's to make the interface consistent with changing variables displayed*/
    printf("%s", game_name[gamemode]);

    for (c = 0 ; game_name[gamemode][c] != '\0' ; c++);
    for(m = 0 ; m < (18 - c) ; m++){
      printf(" ");
    }

    if(i != 6 && i != 16){
      for(i = 0 ; i < amount_dice ; i++){
        printf("%d ", rand_numbers[i]);
      }
    } else {
      for(m = 0 ; m < (amount_dice * 2) ; m++){
        printf(" ");
      }
    }
    /*This calls the function which calls all the calculations for each gamemode*/
    /*Also it outputs the random numbers and makes it so, that the array is accessible by caller*/
    caller(amount_dice, rand_numbers, gamemode, results);
  /*More printf adjusting for the interface*/
  if(j != 16){  
    if(results[j] > 9){
      printf("    |   %d", results[j]);
    } else {
      printf("    |    %d", results[j]);
    }
  } else {
    printf("    |     ");
  }
  /*Here the accumulated sum and end sum is calculated and after displayed*/
    sum = 0;
    for(i = 0 ; i < j + 1 ; i++){
      sum += results[i];
    }
    if(j == 16){
      sum = 0;
      for(i = 0 ; i < 16 ; i++){
        sum += results[i];
      }
      j = 0;
    }
    printf("  |  %d", sum);
    printf("\n");
    /*'j' is a simple counter, så that we can calculate sum (gamemode could be used but if changed like we do with j, it would affect the main for-loop)*/
    j++;
  }
  free(rand_numbers);
}

void roll_dice(int amount_dice, int *rand_numbers){
  int i = 0;

  /*Get the elements of the array*/
  for(i = 0 ; i < amount_dice ; i++){
    rand_numbers[i] = (rand() % 6) + 1;
  }
}

void caller(int amount_dice, int *rand_numbers, int gamemode, int *results){
  /*The switch takes in the gamemode as a number and calls the functions for the respective gamemode and takes in the result in the results-array*/
  int i = 0, sum_b = 0, score, score_2, f_house_temp, f_house_temp_2;
  switch(gamemode){
    case ones: case twos: case threes: case fours: case fives: case sixes:
    results[gamemode] = upper(amount_dice, rand_numbers, gamemode + 1);
    case bonus:
      results[6] = 0;
      for(i = 0 ; i < 6 ; i++){
        sum_b += results[i];
      }
      if(sum_b >= 63){
        results[6] = 50;
      } else {
        results[6] = 0;
      }
    break;
    case one_pair:
      results[7] = (s_pair(amount_dice, rand_numbers, 2, 0) * 2);
    break;
    case two_pairs:
      score = s_pair(amount_dice, rand_numbers, 2, 0);
      if(score == 1){  
        score = 0;
        score_2 = 0;
      }else if(score > 1 && score < 7){
        score_2 = s_pair(amount_dice, rand_numbers, 2, score);
      }else {
        score = 0;
        score_2 = 0;
      }
      /*Checking if first pair is 1 cause then second pair can't exist. And if second pair is 0, then points 0*/
      if(score_2 == 0){
        score = 0;
      }
      results[8] = ((score + score_2) * 2);
    break;
    case three_kind:
      results[9] = (s_pair(amount_dice, rand_numbers, 3, 0) * 3);
    break;
    case four_kind:
      results[10] = (s_pair(amount_dice, rand_numbers, 4, 0) * 4);
    break;
    case full_house:
      f_house_temp = s_pair(amount_dice, rand_numbers, 3, 0);
      if(f_house_temp != 0){
        f_house_temp_2 = s_pair(amount_dice, rand_numbers, 2, f_house_temp);
      } else{
        f_house_temp_2 = 0;
      }
      results[11] = ((f_house_temp * 3) + (f_house_temp_2 * 2));
    break;
    case small_straight:
      results[12] = straights(amount_dice, rand_numbers, 6);
    break;
    case large_straight:
      results[13] = straights(amount_dice, rand_numbers, 1);
    break;
    case chance:
      results[14] = f_chance(amount_dice, rand_numbers);
    break;
    case yatzy:
      results[15] = s_pair(amount_dice, rand_numbers, 5, 0);
      if(results[15] > 0){
        results[15] = 50;
      } else {
        results[15] = 0;
      }
    break;
  }
}

int upper(int amount_dice, int *rand_numbers, int gamemode){
  /*Checks the random number array with gamemode, which is 1-6 depending on the switch case call*/
  int i, score = 0;

  for(i = 0 ; i < amount_dice ; i++){
    if(rand_numbers[i] == gamemode && score < 5){/*< 5, da vi kun skal bruge 5 terninger*/
      score++;
    }
  }

  return (score * gamemode);
}

int s_pair(int amount_dice, int *rand_numbers, int size_comp, int skip_num){
  /*Checks if the sum of the 1's, 2's... 6's is equal or greater than a itseæf times an adjustable variable to find the amount of that number*/
  int score = 0, i;

  for(i = 6 ; i > 0 ; i--){
    score = upper(amount_dice, rand_numbers, i);
    if(score >= (i * size_comp)){
      if(i != skip_num){
        score = i;
        i = 0;
      }
    }else {
      score = 0;
    }
  }

  return score;
}

int straights(int amount_dice, int *rand_numbers, int small_or_big){
  int i, score = 0, j;
  /*When encountering our number (1-6) in order, then score+1. Small_or_big is adjusted for gamemode*/
  for(j = 1 ; j < amount_dice ; j++){
    for(i = 0 ; i < amount_dice ; i++){ /*Nested for-loop, to compare each array-number with 1-6*/
      if(rand_numbers[i] == j && j != small_or_big){
        score++;
        i = amount_dice + 1; /*When one number is confirmed, we want to exit nested loop to check next number*/
      }
    }
  }/*Give points depinding if called for small og large straight*/
  if(score == 5){
    if(small_or_big == 6){
      score = 15;
    } else if(small_or_big == 1){
      score = 20;
    }
  } else{
    score = 0;
  }
  
  return score;
}

int f_chance(int amount_dice, int *rand_numbers){
  int i, j, k = 6, score = 0, n = 0, scores[5];
  
  for(j = 0 ; j < amount_dice + 1 ; j++){
    for(i = 0 ; i < amount_dice ; i++){
      if(rand_numbers[i] == k && n != 5){
        scores[n] = rand_numbers[i];
        n++;
      }
    /*Loops whole array for each number (1-6) from largest to smallest.*/} 
    k--; /*Counter to switch to next highest number - continues until we have all 5 numbers needed*/
    if(n == 5){
      j = amount_dice;
    }
  }
    for(i = 0 ; i < 5 ; i++){
        score += scores[i];
    }

  return score;
}