#include <stdio.h>
#include <stdlib.h>
// stdlib includes malloc() and free()

// define instructions 
#define PREVIOUS -1
#define NEXT 1
#define DELETE 0


// Node declaration ( Do not edit )
typedef struct NODE
{
	int data;
	struct NODE* previousNode;
	struct NODE* nextNode;
} node;

// Function prototypes
void insertNodeNext(int,int,node*);//TODO
void insertNodePrevious(int,int,node*);// TODO
void deleteNode(int,node*); // TODO
void deleteList(node*); // TODO
node* getNodeAt(int, node*);

void printList(node*); // Implemented for you

// Start of main
int main()
{
	int position;
	int instruction;
	int value;
	
	// Declaration of the origin Node ( do not edit )
	node* originNode = (node*)malloc(sizeof(node));
	originNode->previousNode = originNode;
	originNode->nextNode = originNode;
	originNode->data = 0;

	// input and processing has been done for you. 
	
	while(scanf("%i%i",&instruction,&position) == 2)
		//scanf() returns an int that corresponds to the number of values scanned. 
	{
		if(instruction == DELETE)
		{
			deleteNode(position,originNode);
		}
		else if(instruction == NEXT)
		{
			scanf("%i",&value);
			insertNodeNext(position,value,originNode);
		}
		else if(instruction == PREVIOUS)
		{
			scanf("%i",&value);
			insertNodePrevious(position,value,originNode);
		}
	}
	printList(originNode);
	deleteList(originNode);
	

	printf("Circular List after delete\n");
	printList(originNode);
	
	free(originNode);
	
	return 0;
}


//Function Implementations 

node* getNodeAt(int position, node *originNode) {
	node* cur = originNode;
	for(int i=0;i<position;i++) {
		cur = cur -> nextNode;
	}
	return cur;
}

void insertNodePrevious(int position,int value, node* originNode)
{
	node* cur = originNode;
	int i;
	for(i=0;cur->nextNode != originNode;i++) {
		cur = cur -> nextNode;
	}
	i++;
	insertNodeNext((position - 1 + i)%i, value, originNode);
}

void insertNodeNext(int position, int value, node* originNode)
{
	node* newNode = (node*)malloc(sizeof(node));
	newNode->data = value;
	node* cur = originNode;
	int i;
	for(i=0;i<position;i++) {
		cur = cur -> nextNode;
		if(cur == originNode) break;
	} 
	if(i < position) {
		cur = getNodeAt(position % (i+1), originNode);
	}

	newNode->nextNode = cur->nextNode;
	newNode->nextNode->previousNode = newNode;
	newNode->previousNode = cur;
	cur->nextNode = newNode;
}

void deleteNode (int position,node* originNode)
{	
	node* cur = getNodeAt(position, originNode);
	cur->previousNode->nextNode = cur->nextNode;
	cur->nextNode->previousNode = cur->previousNode;
	free(cur);
}

void deleteList(node* originNode)
{
	node* cur = originNode->nextNode;
	originNode->nextNode = originNode;
	originNode->previousNode = originNode;
	while(cur != originNode) {
		node* nxt = cur->nextNode;
		free(cur);
		cur = nxt;
	}
}

// printList has been implemented for you.
// You may study the functions below to help you implement your own functions. 
void printList(node* originNode)
{
	int count = 0;

	printf("Printing clockwise:\n");
	node* iterator = originNode->nextNode;
	printf("[Pos %i:%i]",0,originNode->data);
	printf("\n   |\n   v\n");
	while(iterator != originNode)
	{
		count++;
		printf("[Pos %i:%i]",count,iterator->data);
		printf("\n   |\n   v\n");
		iterator = iterator->nextNode;
	}
	printf("[Pos %i:%i]",0,originNode->data);	
	
	printf("\nPrinting counter-clockwise:\n");
	iterator = originNode->previousNode;
	printf("[Pos %i:%i]",0,originNode->data);
	printf("\n   |\n   v\n");
	while(iterator!= originNode)
	{
		printf("[Pos %i:%i]",count,iterator->data);
		printf("\n   |\n   v\n");
		iterator = iterator->previousNode;
		count--;
	}
	printf("[Pos %i:%i]\n",0,originNode->data);		

	return;
}

