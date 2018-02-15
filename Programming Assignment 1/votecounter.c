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

/** This isn't done quite yet, but here's what I have so far **/
int parseInputLine(char *s, node_t *n) {
	char **strings = (char **)malloc(1024 * sizeof(char *));
	if(makeargv(s, ":", &strings) == sizeof(s)) {
		int length = makeargv(s, " ", &strings);
		if(isdigit(strings[0])) {
			char *candidates = (char *)malloc(strings[0] * sizeof(char));
			for(int i = 0; i < length; i++) {
				candidates[i] = strings[i+1];
				}
			return 0;
	}
		else if(strings[0] == "Who_Won") {
			struct node *root = (struct node *) malloc(sizeof(struct node));
			root->name = "Who_Won";
			root->num_children = sizeof(strings) - 1;
			for(int i = 0; i < root->num_children; i++) {
				root->children = strings[i+1];
			}
			return 0;
		}
	}
	else {
		int len = makeargv(s, ":", &strings);
	      	int elements_len = makeargv(&strings, " ", &strings);
		return 0;
	}

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

	while((buf = read_line(buf, f)) != NULL) {
		if(buf[0] != '#') {
			parseInputLine(buf, n);
		}
	}

        // Set progs for all nodes once they've been processed "./leafcounter" "./aggregate_votes" "./find_winner"
	while(n != NULL) {
		// Check if it's the root node
		if(n->name == "Who_Won") {
			strncpy(n->prog, "./find_winner", 1024);
		} else if(n->num_children == 0) {
			strncpy(n->prog, "./leafcounter", 1024);
		} else {
			strncpy(n->prog, "./aggregate_votes", 1024);
		}
		n ++;
	}
}

/**Function : execNodes
 * Arguments: 'n' - Pointer to Nodes to be allocated by parsing
 * About execNodes: parseInputLine is supposed to
 * If the node passed has children, fork and execute them first
 * Please note that processes which are independent of each other
 * can and should be running in a parallel fashion
 * */
void execNodes(node_t *n) {

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

	while(mainnodes != NULL && mainnodes->name != "Who_Won") {
		mainnodes ++;
	}

	if (mainnodes != NULL) {
		execNodes(mainnodes);
	}

	return 0;
}
