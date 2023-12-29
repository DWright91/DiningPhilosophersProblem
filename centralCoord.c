#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <time.h>
#include "soc.h"
#include "philosopher.h"

void enqueue(int *array, int input);
void dequeue(int *array, int location);
void printq(int array[]);
void printt(int array[]);

int main (int argc, char *argv[]) {
	time_t t = time(0);

	/* initiate variables */
	int err, pid;							// for child process calling
	int i;										// counter
	int cSocket[N];						// client socket
	int sSocket;							// server socket
	struct sockaddr_in sAddr;	// server socket addr
	struct sockaddr_in cAddr[N];// client socket addr
	int cSocLen;							// client socket length
	char msg[BUFFLEN];				// buffer
	int status;								// child process exit status
	int nfd;
	fd_set RFD;
	struct timeval waitTime;	// wait time
	int ADDPORT = atoi(argv[1]);
	char TIME0[30];
	sprintf(TIME0, "%d", t);
	int t0 = atoi(TIME0);

	/* set up coordinator socket */
	sSocket = socket(AF_INET, SOCK_STREAM, 0); // AF_INET
	if (sSocket == -1) {
		perror("socServer: socket creation failed");
		exit(1);
	}
	setAddr(&sAddr, 0, ADDPORT);
	err = bind(sSocket, (struct sockaddr*)&sAddr,
				sizeof(struct sockaddr_in));
	if (err == -1) {
		perror("socServer: bind address to socket failed");
		exit(2);
	}
	err = listen(sSocket, 5);
	if (err == -1) {
		perror("socServer: listen failed");
		exit(3);
	}
	printf("Coordinator setup successfully...\n");
	printf("About to run %d processes...\n\n", N);

	// Execl 5 philosophers
	for (i = 0; i < N; i++) {
		char id[1];
		id[0] = i + '1';

		pid = fork();
		if (pid < 0) {	/* fork failed */
			perror("Starter: Fork failed");
			exit(5);
		}
		else if (pid == 0) {	/* in child process */
			execl("./philosopher", "philosopher", id, argv[1], TIME0);
		}
		cSocket[i] = accept(sSocket, (struct sockaddr*)&cAddr[i], &cSocLen);
		printf("P[%c] connected.\n", id[0]);
	}

	// CENTRALIZED ALGORITHM
	// START SIMULATING //
	printf("\nCENTRALIZED ALGORITHM\n");
	int chopsticks[6] = {-1, 1, 2, 3, 4, 5};
	int queue[5] = {0, 0, 0, 0, 0};

	while(1) {
		int isReceived = 0;
		for (i = 0; i < N; i++) {
			FD_ZERO (&RFD);
			FD_SET (cSocket[i], &RFD);
			nfd = cSocket[i] + 1;
			waitTime.tv_sec = 0;// select return immediately (no wait).
			waitTime.tv_usec = 10;
			err = select (nfd, &RFD, NULL, NULL, &waitTime);
			if (err > 0) {
				err = recv(cSocket[i], msg, 8, 0);
				isReceived = 1;
				break;
			}
		}

		double ts;
		if (isReceived == 1) {
			int thisID = msg[1] - '0';
			int left = msg[2]-'0';
			int right = msg[3]-'0';
			if (msg[0] == 'q') {
				if (chopsticks[left] != 0 &&
					chopsticks[right] != 0) {
					printt(chopsticks);
					chopsticks[left] = 0;
					chopsticks[right] = 0;
					printf("TS%.0f. Coord: P[%d] can eat\n",
							difftime(time(NULL), t0), thisID);
					sprintf(msg, "%c", 'o');
					send(cSocket[thisID - 1], msg, 8, 0);
				}
				else {
					printf("TS%.0f. Coord: P[%d] must wait\n",
							difftime(time(NULL), t0), thisID);
					enqueue(queue, thisID);
					printq(queue);
				}
			}
			if (msg[0] == 'r') {
				chopsticks[left] = left;
				chopsticks[right] = right;
				for (i = 0; i < N; i++) {
					if (queue[i] == 0) break;
					int id = queue[i];
					int leftToken = id;
					int rightToken = (id>1)?(id-1):5;
					if (chopsticks[leftToken] != 0 &&
						chopsticks[rightToken] != 0) {
						printt(chopsticks);
						chopsticks[leftToken] = 0;
						chopsticks[rightToken] = 0;
						printf("TS%.0f. Coord: P[%d] can eat\n",
								difftime(time(NULL), t0), id);
						dequeue(queue, i);
						printq(queue);
						sprintf(msg, "%d", 1);
						send(cSocket[id-1], msg, 8, 0);
					}
				}
			}
		}
	}
}

void enqueue(int *array, int input) {
	int i;
	for (i = 0; i < N; i++) {
		if (*(array + i) == 0) {
			*(array + i) = input;
			break;
		}
	}
}

void dequeue(int *array, int location) {
	int i;
	for (i = location; i < N - 1; i++) {
		*(array+i) = *(array+i+1);
	}
	*(array+N-1) = 0;
}

void printq(int array[]) {
	int i;
	printf("Queue: ");
	for (i = 0; i < N; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

void printt(int array[]) {
	int i;
	printf("Available chopsticks: ");
	for (i = 1; i < N + 1; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

// Program Output:
//[rc2282qr@csci4 ~]$ make all
//rm -f startC
//rm -f startTR
//rm -f philosopher
//rm -f tokenRingAlgoirthm
//cc -o startC philosopher.h soc.h centralCoord.c
//cc -o philosopher philosopher.h soc.h philosopher.c
//cc     tokenRingAlgoirthm.c   -o tokenRingAlgoirthm
//cc -o startTR philosopher.h soc.h tokenRingStarter.c
//[rc2282qr@csci4 ~]$ ./startC 7
//Coordinator setup successfully...
//About to run 5 processes...
//
//P[1] connected.
//P[2] connected.
//P[3] connected.
//P[4] connected.
//P[5] connected.
//
//CENTRALIZED ALGORITHM
//TS1. P[1] wants to eat
//Available chopsticks: 1 2 3 4 5
//TS1. Coord: P[1] can eat
//TS1. P[1] picks up 1 and 5
//TS1. P[1] is eating in 2s
//TS1. P[2] wants to eat
//TS1. Coord: P[2] must wait
//Queue: 2 0 0 0 0
//TS1. P[3] wants to eat
//Available chopsticks: 0 2 3 4 0
//TS1. Coord: P[3] can eat
//TS1. P[3] picks up 3 and 2
//TS1. P[3] is eating in 3s
//TS1. P[4] wants to eat
//TS1. Coord: P[4] must wait
//Queue: 2 4 0 0 0
//TS1. P[5] wants to eat
//TS1. Coord: P[5] must wait
//Queue: 2 4 5 0 0
//TS3. P[1] releases 1 and 5
//TS3. P[1] is thinking in 3s
//Available chopsticks: 1 0 0 4 5
//TS3. Coord: P[5] can eat
//Queue: 2 4 0 0 0
//TS3. P[5] picks up 5 and 4
//TS3. P[5] is eating in 2s
//^XTS4. P[3] releases 3 and 2
//TS4. P[3] is thinking in 2s
//Available chopsticks: 1 2 3 0 0
//TS4. Coord: P[2] can eat
//Queue: 4 0 0 0 0
//TS4. P[2] picks up 2 and 1
//TS4. P[2] is eating in 3s
///TS5. P[5] releases 5 and 4
//TS5. P[5] is thinking in 3s
//Available chopsticks: 0 0 3 4 5
//TS5. Coord: P[4] can eat
//Queue: 0 0 0 0 0
//TS5. P[4] picks up 4 and 3
//TS5. P[4] is eating in 4s
//^C
