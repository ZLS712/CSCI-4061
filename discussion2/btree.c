#include <time.h>
#include <stdlib.h>
#include <stdio.h>

struct node {

	int val;
	struct node* left;
	struct node* right;
};

struct btree {

	struct node* root;
};

void insert(struct btree* tree, int val) {
	
	// TODO: Implement insertion.
    if (tree == NULL) {
        struct tempNode = (struct node *)malloc(sizeof(struct node));
        tempNode->left = NULL;
        tempNode->right = NULL;
        tempNode->val = val;
        tree->root = tempNode;
    } else if (tree->val < val) {
        tree->right = insert(tree->right, val);
    } else {
        tree->left = insert(tree->left, val);
    }
}

struct btree* populate(int size) {

	// DO NOT CHANGE OR REUSE THIS LINE!
	srand(time(NULL));
    
    //Not sure about the *size, seems like it should be needed but who knows, it's C!
    struct tree = (struct btree *)malloc(sizeof(btree)*size);
    tree->root = NULL;
    
	// TODO: Insert 'size' number of random nodes.
	// Obtain random number using rand()
    int i; //Counter Variable
    for(i = 0; i < size; i ++) {
        insert(tree, rand());
    }

	return tree;
}

int main(int argc, char** argv) {

	if (argc < 2) {

		printf("Invalid number of args");
		exit(1);
	}

	int size = atoi(argv[1]);

	if (size < 1) {

		printf("Size must be > 0");
		exit(1);
	}

	// Randomly populate a linked list.
	struct btree* tree = populate(size);

	return 0;
}
