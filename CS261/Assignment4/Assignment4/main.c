#include<stdio.h>
#include<stdlib.h>
#include "bst.h"
#include "structs.h"

/* Example main file to begin exercising your tree */

int main(int argc, char *argv[])
{
	struct BSTree *tree	= newBSTree();

	/*Create value of the type of data that you want to store*/
	struct data myData1;
	struct data myData2;
	struct data myData3;
	struct data myData4;
    struct data myData5;
    struct data myData6;
    struct data myData7;

	myData1.number = 4;
	myData1.name = "rooty";
	myData2.number = 2;
	myData2.name = "lefty";
	myData3.number = 6;
	myData3.name = "righty";
	myData4.number = 1;
	myData4.name = "righty";
    myData5.number = 3;
	myData5.name = "righty";
    myData6.number = 5;
	myData6.name = "righty";
    myData7.number = 7;
	myData7.name = "righty";

	/*add the values to BST*/
	addBSTree(tree, &myData1);
	addBSTree(tree, &myData2);
	addBSTree(tree, &myData3);
	addBSTree(tree, &myData4);
    addBSTree(tree, &myData5);
    addBSTree(tree, &myData6);
    addBSTree(tree, &myData7);

	/*Print the entire tree*/
	printTree(tree); printf("\n");
	/*(( 1 ( 3 ) ) 5 ( 10 ))*/
    //removeBSTree(tree, &myData4);
    printTree(tree);
	return 1;
}

