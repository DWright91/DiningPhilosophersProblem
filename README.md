# DiningPhilosophersProblem

Purpose: The objective of this project is to implement several distributed mutual
exclusion algorithms and use each to solve a specified synchronization problem.

Requirements:
1. This project is based on Section 6.3 (pages 321 – 329) of the textbook:
Distributed Systems by van Steen and Tanenbaum, third edition. You are to write
two distributed programs. Each is to solve the Dining Philosophers Problem
using a different mutual exclusion algorithm. That is, one program must use the
centralized algorithm, and one must use the token ring algorithm.

2. Mutual Exclusion Algorithms: Mutual exclusion is to be used to update shared
variables related to the Dining Philosophers Problem. For a description of this
problem, see a textbook such as: Operating System Concepts with Java by
Silberschatz, Galvin and Gagne, 8th edition, pp. 262 – 264.

3. Dining Philosophers Solution: A process must be used to represent each dining
philosopher; there must be a total of five such processes. The duration of the
eating and thinking phases is to be randomly selected at run-time by each process.
Your code should display messages that correspond to the start and stop of each
of these phases. These phases must not interfere with the communication
responsibilities of the process related to mutual exclusion. For example, the reply
to a message must not be delayed because the philosopher is eating or thinking.
In addition the completion of a phase must not be delayed because I/O is blocked.
Your solution must be deadlock free.

4. Shared Variables: The shared variables are to be accessed in some manner that
could be supported in a distributed environment.

5. Output: Your output must clearly demonstrate that each mutual exclusion
algorithm is effective. Further, it is expected that it will demonstrate that more
than one philosopher is able to eat at one time.

6. Error checking: Provide appropriate error checking for each system call and take
appropriate steps if an error is encountered.

7. Communications: You must use Internet type sockets. Though I only expect you
to test your program on the virtual machine (199.17.28.75 or 199.17.28.80), it
should be possible to have the processes located on different machines and still
have the programs communicate correctly.

