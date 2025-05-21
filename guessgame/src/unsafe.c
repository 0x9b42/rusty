#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
  int secret;
  int guess;
  char buffer[100];

  srand(time(NULL));
  secret = rand() % 100 + 1;

  printf("pls guess a number: ");
  fflush(stdout);

  scanf("%d", &guess);

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
