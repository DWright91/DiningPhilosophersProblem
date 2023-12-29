#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "soc.h"
#include "philosopher.h"

int main (int argc, char *argv[]) {
	int i;
	int pid;

	printf("\nAbout to run %d processes...\n", N);

	for (i = 1; i < N + 1; i++) {
		char id[1];
		id[0] = i + '0';

		pid = fork();
		if (pid < 0) {	/* fork failed */
			perror("Starter: Fork failed");
			exit(5);
		}
		else if (pid == 0) {	/* in child process */
			execl("./tokenRingAlgoirthm", "tokenRingAlgoirthm", id, argv[1]);
		}
	}

	for(;;){
	}
}

// Program Output:
//[rc2282qr@csci4 ~]$ ./startTR 7
//
//About to run 5 processes...
//ID[1]: Port 31208 is ready
//ID[2]: Port 31209 is ready
//ID[3]: Port 31210 is ready
//ID[5]: Port 31212 is ready
//ID[4]: Port 31211 is ready
//ID[2]: Connects port 31210
//ID[1]: Connects port 31209
//ID[3]: Connects port 31211
//ID[5]: Connects port 31208
//ID[4]: Connects port 31212
//P[1]: Available chopsticks: 1 2 3 4 5
//
//TOKEN RING ALGORITHM
//TS2. P[1]: Has token. Ready to eat.
//P[1]: Pick up 1 and 5
//P[1]: Available chopsticks: 0 2 3 4 0
//TS2. P[1] is eating in 3s.
//TS2. P[3]: Has token. Ready to eat.
//P[3]: Pick up 3 and 2
//P[3]: Available chopsticks: 0 0 0 4 0
//TS2. P[3] is eating in 2s.
//TS4. P[3] has done eating.
//P[3]: Release 3 and 2
//P[3]: Available chopsticks: 0 2 3 4 0
//TS4. P[4]: Has token. Ready to eat.
//P[4]: Pick up 4 and 3
//P[4]: Available chopsticks: 0 2 0 0 0
//TS4. P[3] is thinking in 4s.
//TS4. P[4] is eating in 4s.
//TS5. P[1] has done eating.
//P[1]: Release 1 and 5
//P[1]: Available chopsticks: 1 2 0 0 5
//TS5. P[1] is thinking in 4s.
//TS5. P[2]: Has token. Ready to eat.
//P[2]: Pick up 2 and 1
//P[2]: Available chopsticks: 0 0 0 0 5
//TS5. P[2] is eating in 4s.
//^C
