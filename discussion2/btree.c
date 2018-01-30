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
    
    struct node* node = tree->root;
    struct node* node2 = (struct node*)malloc(sizeof(struct node));
    node2->val = val;
    node2->left = NULL;
    node2->right = NULL;
    if(tree == NULL) {
        tree = (struct btree*) malloc(sizeof(struct btree));
        tree->root = node2;
    } else if (node == NULL) {
        node = node2;
    } else {
        while(node != NULL) {
            node = node->left;
        }
        node = node2;
    }
}
    
	// TODO: Implement insertion.
    /*
    if (tree == NULL) {
        struct node* tempNode = (struct node*)malloc(sizeof(struct node));
        tempNode->left = NULL;
        tempNode->right = NULL;
        tempNode->val = val;
        tree->root = tempNode;
    } else {
        struct btree* temp2 = (struct btree*)malloc(sizeof(struct btree));
        temp2->root = tree->root->right;
        insert(temp2, val);
    }
    */
                                                    

struct btree* populate(int size) {

	// DO NOT CHANGE OR REUSE THIS LINE!
	srand(time(NULL));
    
    //Not sure about the *size, seems like it should be needed but who knows, it's C!
    struct btree* tree = (struct btree *)malloc(sizeof(struct btree)*size);
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
