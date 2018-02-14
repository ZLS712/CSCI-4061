#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {

	if (argc < 2) {

		printf("Wrong number of args, expected 1, given %d", argc - 1);
		exit(1);
	}

	// TODO: Your code goes here.
	int fd[2];
	pipe(fd);
	int argument = atoi(argv[1]);
	pid_t pid = fork();

	//Rerouting output to file


	if (pid == 0) {
		int fdOutput = open("output.txt", O_WRONLY|O_CREAT);
		//close(fd[0]);
		dup2(fdOutput, STDOUT_FILENO);
		write(fd[1], "output.txt", 100);
		chmod("sol.o", 700);
		chmod("rec3.o", 700);
		execl("./rec3.o", "useless", atoi(argv[1]), (char*)NULL);
		close(fd[1]);
	} else {
		wait(&pid);
		char buffer[100];
		fflush(stdout);
		//open(fd[0]);
		read(fd[0], buffer, 100);
		printf("%s\n",buffer);
		int outputFd = open(buffer, O_RDONLY);
		char buffer1[11];
		read(outputFd, buffer1, 11);

		printf("%s\n",buffer1);
		fflush(stdout);
	}
}
