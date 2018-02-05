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
//    int input = atoi(argv[1]);
    pid_t pid = fork();
    if (pid ==  0) {
        execl("./rtime.o","dummy",argv[1],(char*)NULL);
				printf("execl failed \n");
    } else {
        wait(&pid);
	pid = fork();
fflush(stdout);
	if(pid == 0) {
		execv("rtime.o", argv);
		printf("execv failed \n");
		}else{
wait(NULL);}
    }
}
