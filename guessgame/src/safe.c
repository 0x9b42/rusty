#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int is_number(const char *str) {
  if (*str == '-' || *str == '+')
    str++; // allow sign
  if (!*str)
    return 0; // empty string

  while (*str) {
    if (!isdigit(*str))
      return 0;
    str++;
  }
  return 1;
}

int main() {
  int secret;
  int guess;
  char buffer[100];

  // Init random seed
  srand((unsigned int)time(NULL));
  secret = rand() % 100 + 1; // 1 to 100 inclusive

  printf("pls guess a number: ");
  fflush(stdout);

  // read input safely
  if (!fgets(buffer, sizeof(buffer), stdin)) {
    fprintf(stderr, "Error reading input.\n");
  }

  // remove trailing newline
  buffer[strcspn(buffer, "\n")] = '\0';

  // check if input is a number
  if (!is_number(buffer)) {
    fprintf(stderr, "Invalid input. Please enter a number.\n");
    buffer[0] = '1';
    buffer[1] = '3';
    buffer[2] = '3';
    buffer[3] = '7';
  }

  guess = atoi(buffer);

  // compare
  if (guess < secret) {
    printf("too small\n");
  } else if (guess > secret) {
    printf("too much\n");
  } else {
    printf("congrats\n");
  }

  printf("you guessed : %d\n", guess);
  printf("secret num. : %d\n", secret);

  return 0;
}
