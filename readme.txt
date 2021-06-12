
UPDATE ADDED VERBOSE MODE

Carlos Hevia Alejano

As stated in project description
only works with inputs of size N = 2^k | k is an integer

The solution implemented was based on the pseudo-code
of reusable barriers found in "The little book of semaphores"

Using 3 sempahores, one mutex to protect variable "count"
and the wait/signal functions of the turnstiles (the two other semaphores).

"Only the nth thread can lock or unlock the turnstiles.
Before a thread can unlock the first turnstile, it has to close the second,
and vice versa; it is impossible for one thread to get ahead of
the others by more than one turnstile."

To compile: $ g++ sort.cpp -lpthread -lrt -o sort

To run: ./sort example.txt -r	(or -o for observation)

example.txt - example list to sort

sem-init.txt - to initialize the semaphores

indexes.h - header file with funcitons to find correspondent indexes to compare given thread id and phase

sort.cpp - main program
