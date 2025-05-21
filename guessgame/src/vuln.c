#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * compile with:
 * gcc vuln.c -fno-stack-protector -z execstack -no-pie
 * gcc vuln.c -fPIE -pie -fno-stack-protector
 * gcc vuln.c -o vuln -fPIE -pie -fno-stack-protector
 **/

void win() {
    printf("YOU HAVE BEEN HACKED! WIN FUNCTION EXECUTED\n");
    system("/bin/sh");
}

int main() {
    int secret;
    int guess;
    char buffer[32];  // small buffer to overflow

    srand(time(NULL));
    secret = rand() % 100 + 1;

    printf("pls guess a number: ");
    fflush(stdout);

    scanf("%s", buffer); // vulnerability here: no bounds check!

    guess = atoi(buffer);

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
