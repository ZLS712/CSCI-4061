/*
 * VCforStudents.c
 *
 *  Created on: Feb 2, 2018
 *      Author: ayushi
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>
#include "makeargv.h"

#define MAX_NODES 100

char** candidateNames;
int numCandidates;

//Function signatures

/**Function : parseInputLine
 * Arguments: 's' - Line to be parsed
 * 			  'n' - Pointer to Nodes to be allocated by parsing
 * Output: Number of Region Nodes allocated
 * About parseInputLine: parseInputLine is supposed to
 * 1) Split the Input file [Hint: Use makeargv(..)]
 * 2) Recognize the line containing information of
 * candidates(You can assume this will always be the first line containing data).
 * You may want to store the candidate's information
 * 3) Recognize the line containing "All Nodes"
 * (You can assume this will always be the second line containing data)
 * 4) All the other lines containing data, will show how to connect the nodes together
 * You can choose to do this by having a pointer to other nodes, or in a list etc-
 */

/* function to create new Nodes and assign name and id
 */
struct node *newNode(char *name, int id) {
	struct node *temp = (struct node *)malloc(sizeof(struct node));
	strcpy(temp->name, name);
	temp->id = id;
	return temp;
}


/* 
 *
 *
 *
 *
 */
int parseInputLine(char *s, node_t *n) {
	char **strings = (char **)malloc(1024 * sizeof(char *));
	struct node **queue = (struct node **)malloc(sizeof(strings) * sizeof(struct node));
	if(makeargv(s, ":", &strings) == sizeof(s)) {
		int length = makeargv(s, " ", &strings);
		if(strcmp(strings[0],"Who_Won")) {
			struct node *root = (struct node *) malloc(sizeof(struct node));
			strcpy(root->name,"Who_Won");
			*queue[0] = *root;
			for(int i = 1; i < sizeof(strings); i++) {
				struct node *child = newNode(strings[i], i);
				*queue[i] = *child;
			}
			return 0;
		}
		else {
			numCandidates = atoi(strings[0]);
			char** candidates = (char **)malloc(numCandidates * sizeof(char *));
			candidateNames = (char **)malloc((numCandidates+1) * sizeof(char *));
			int i;
			for (i = 0; i < length; i++) {
				candidates[i] = strings[i+1];
				candidateNames[i] = strings[i+1];
			}
			candidateNames[i] = NULL;
			return 0;
		} 
	} else {
		int len = makeargv(s, ":", &strings);
		int elements_len = makeargv(*strings, " ", &strings);
		struct node *parent = (struct node *)malloc(sizeof(struct node));
		strcpy(parent->name,strings[0]);
		int child_num = 0;
		for(int q = 1; q < sizeof(strings); q++) {
			for(int g = 0; g < sizeof(*queue); g++) {
				if(strings[q] == queue[g]->name) {
					parent->children[child_num] = queue[g]->id;
					parent->num_children = child_num;
					child_num++;
				}
			}
		}
		return child_num;
	}
	return 0;
}

/**Function : parseInput
 * Arguments: 'filename' - name of the input file
 * 			  'n' - Pointer to Nodes to be allocated by parsing
 * Output: Number of Total Allocated Nodes
 * About parseInput: parseInput is supposed to
 * 1) Open the Input File [There is a utility function provided in utility handbook]
 * 2) Read it line by line : Ignore the empty lines [There is a utility function provided in utility handbook]
 * 3) Call parseInputLine(..) on each one of these lines
 ..After all lines are parsed(and the DAG created)
 4) Assign node->"prog" ie, the commands that each of the nodes has to execute
 For Leaf Nodes: ./leafcounter <arguments> is the command to be executed.
 Please refer to the utility handbook for more details.
 For Non-Leaf Nodes, that are not the root node(ie, the node which declares the winner):
 ./aggregate_votes <arguments> is the application to be executed. [Refer utility handbook]
 For the Node which declares the winner:
 This gets run only once, after all other nodes are done executing
 It uses: ./find_winner <arguments> [Refer utility handbook]
 */
int parseInput(char *filename, node_t *n) {
	// Open Input file
	FILE* f = file_open(filename);

	// Read Input File Line By line calling parseInputLine on each line and ignore empty lines
	char* buf = (char*)malloc(sizeof(char)*1024);

	//Int to keep track of total nodes allocated
	int total = 0;

	while((buf = read_line(buf, f)) != NULL) {
		if(buf[0] != '#') {
			total = total + parseInputLine(buf, n);
		}
	}

  // Set progs for all nodes once they've been processed "./leafcounter" "./aggregate_votes" "./find_winner"
	while(n != NULL) {
		// Check if it's the root node
		if(strncmp(n->name, "Who_Won", strlen(n->name)) == 0) {
			strncpy(n->prog, "./find_winner", 1024);
		} else if(n->num_children == 0) {
			strncpy(n->prog, "./leafcounter", 1024);
		} else {
			strncpy(n->prog, "./aggregate_votes", 1024);
		}
		n ++;
	}
	return total;
}

/**Function : getNodeByID
 * Arguments : 'id' - an integer representing the id of
 *						 the node
 * About getNodeByID: getNodeByID returns the node that
 * corresponds to 'id'
 */
 node_t* getNodeByID(int id) {

 }

 /**Function : itoa
  *  Arguments : 'n' an integer
	*
	*  About itoa : itoa returns a string describing 'n';
  */
 char* itoa(int n) {
	 char* buf = (char *)malloc(sizeof(int) * 4 + 1);
	 sprintf(buf, "%d", n);
	 return buf;
 }

/**Function : execNodes
 * Arguments: 'n' - Pointer to Nodes to be allocated by parsing
 * About execNodes: parseInputLine is supposed to
 * If the node passed has children, fork and execute them first
 * Please note that processes which are independent of each other
 * can and should be running in a parallel fashion
 * */
void execNodes(node_t *n) {
	if(n->num_children == 0) {
		// Exec on leafnodes
		char fileName[] = "Output_";
		strcat(fileName, n->name);
		int fd = open(fileName, O_WRONLY|O_CREAT);
		dup2(fd, STDOUT_FILENO);
		//chmod(fd, 700);
		// Program Name, Input File Name, Output File Name,
		// Number of candidates, Names of candidates, NULL
		int argsLength = 5 + numCandidates;
		char* args[argsLength];
		args[0] = "votecounter";
		char* tempVar = (char*)malloc(strlen(n->name)+1);
		strcpy(tempVar, n->name);
		args[1] = tempVar;
		args[2] = fileName;
		args[3] = itoa(numCandidates);
		int i;
		for(i = 4; candidateNames[i-4] != NULL; i++) {
			args[i] = candidateNames[i-4];
		}
		args[i] = NULL;
		execv("leafcounter", args);
		return;
	} else {
		pid_t pid;
		char* childOutputFileNames[n->num_children];
		int i;
		for(i = 0; i < n->num_children; i ++) {
			node_t* childNode = getNodeByID(n->children[i]);
			pid = fork();
			if (pid == 0) {
				execNodes(childNode);
			} else {
				wait(&pid);
				char outputName[] = "Output_";
				strcat(outputName, childNode->name);
				char* tempVar = (char*)malloc((strlen(outputName)+1)*sizeof(char));
				strcpy(tempVar, outputName);
				childOutputFileNames[i] = tempVar;
				continue;
			}
		}
		if (pid != 0) {
			wait(&pid);
			// Program Name, Number of Input Files, [Input Files' Names,]
			// Output File Name, Number of candidates, [Names of candidates,] NULL
			int argsLength = 5 + n->num_children + numCandidates;
			char* args[argsLength];
			args[0] = "votecounter";
			args[1] = itoa(n->num_children);
			// INSERT INPUT FILES' NAMES
			int i;
			for (i = 2; i < n->num_children; i ++) {
				args[i] = childOutputFileNames[i-2];
			}
			// INSERT OUTPUT FILE NAME
			args[i++] = "Output_";
			strcat(args[i], n->name);
			args[i++] = itoa(numCandidates);
			int valToSubtract = i;
			for(; candidateNames[i-valToSubtract] != NULL; i ++) {
				args[i] = candidateNames[i-valToSubtract];
			}
			args[i] = NULL;
			if (strncmp(n->name, "Who_Won", strlen(n->name)) != 0) {
				execv("aggregate_votes", args);
			} else {
				execv("find_winner", args);
			}
			return;
		}
	}
}

int main(int argc, char **argv){

	//Allocate space for MAX_NODES to node pointer
	struct node* mainnodes=(struct node*)malloc(sizeof(struct node)*MAX_NODES);

	if (argc != 2){
		printf("Usage: %s Program\n", argv[0]);
		return -1;
	}

	//call parseInput
	int num = parseInput(argv[1], mainnodes);


	//Call execNodes on the root node
	//Written by Shri on Wednesday Feb 14 at 1:29 PM

	// Not sure if this is right
	while(mainnodes != NULL && strncmp(mainnodes->name, "Who_Won", strlen(mainnodes->name)) != 0) {
		mainnodes ++;
	}



	if (mainnodes != NULL) {
		execNodes(mainnodes);
	}

	return 0;
}
