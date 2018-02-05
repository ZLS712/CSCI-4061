#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char** argv) {

	if (argc < 2) {

		printf("Incorrect number of args: given %d, expected 1.\n", argc - 1);
		exit(1);
	}

	// TODO: Fill in your code here.
    int input = atoi(argv[1]);
    int *child_status;
    pid_t pid = fork();
    if (pid <=  0) {
        execl("rtime.o", "useless", input, (char*) NULL);
    } else {
        wait(&pid);
        pid_t pid = fork();
        if (pid <= 0) {
            int* temp = (int*)malloc(sizeof(int));
            *temp = input;
            execv("rtime.o", &argv[1]);
        }
    }


}
