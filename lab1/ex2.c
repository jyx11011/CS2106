#include <stdio.h>
#include <stdlib.h>
// stdlib includes malloc() and free()

// define instructions
#define PREVIOUS -1
#define NEXT 1
#define DELETE 0
#define INSERTSUBNODE 2 // New instruction compared to ex1. 

// Node and SubNode declarations ( Do not Edit )
typedef struct SUBNODE
{
	int data;
	struct SUBNODE* nextSubNode;
} subNode;

typedef struct NODE
{
	int data;
	struct NODE* previousNode;
	struct NODE* nextNode;
	subNode* subNodeHead;
} node;

// Function prototypes
void insertNodeNext(int,int,node*); // TODO
void insertNodePrevious(int,int,node*); //TODO
void deleteNode(int,node*); // TODO
void deleteList(node*); // TODO
void insertSubNode(int,int,int,node*); // TODO
void deleteAllSubNodes(node*); // Optional TODO: will be useful in ex3. 

void printList(node*); // Implemented for you
void printSubNodes(node*); // Implemented for you :Helper function for print list

int getLength(node*);
node* getNodeAt(int, node*);

// Start of main
int main()
{
	int position;
	int instruction;
	int subPosition;
	int value;
	
	// Declaration of the origin Node ( do not edit )
	node* originNode = (node*)malloc(sizeof(node));
	originNode->previousNode = originNode;
	originNode->nextNode = originNode;
	originNode->data = 0;
	originNode->subNodeHead = NULL;
	
	// input and processing has been done for you 
	while(scanf("%i%i",&instruction,&position) == 2)
		//scanf() returns an int that corresponds to the number of values scanned. 
	{
		if(instruction == DELETE)
		{
			deleteNode(position,originNode);
		}
		else if(instruction == INSERTSUBNODE)
		{
			scanf("%i%i",&subPosition,&value);
			insertSubNode(position,subPosition,value,originNode);
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

int getLength(node* originNode) {
	node* cur = originNode;
	int len = 0;
	do{
		cur = cur -> nextNode;
		len++;
	}while(cur != originNode);
	return len;
}

node* getNodeAt(int position, node* originNode) {
	int len = getLength(originNode);
	position %= len;
	node* cur = originNode;
	for(int i=0;i<position;i++)
		cur=cur->nextNode;
	return cur;
}

//Function Implementations 
void insertSubNode(int position,int subPosition,int value,node* originNode)
{
	node* targetNode = getNodeAt(position, originNode);
	subNode* cur = targetNode->subNodeHead;
	subNode* newSubNode = (subNode*)malloc(sizeof(subNode));
	newSubNode -> data = value;

	if(subPosition == 0){
		newSubNode->nextSubNode = cur;
		targetNode->subNodeHead = newSubNode;
	} else {
		for(int i=0;i<subPosition - 1 && cur!=NULL;i++){
			cur = cur->nextSubNode;
		}
		if(cur == NULL){
			free(newSubNode);
		} else {
			newSubNode->nextSubNode = cur->nextSubNode;
			cur->nextSubNode = newSubNode;
		}
	}
}

void insertNodePrevious(int position,int value, node* originNode)
{
	// TODO
	// Note: you can use a similar implementation as in ex1. 
	int len = getLength(originNode);
	insertNodeNext((position - 1 + len) % len, value, originNode);
}

void insertNodeNext(int position, int value, node* originNode)
{
	// TODO
	// Note: you can use a similar implementation as in ex1.
	node* newNode = (node*)malloc(sizeof(node));
	newNode->data = value;
	newNode->subNodeHead = NULL;
	node* cur = originNode;
	int len = getLength(originNode); 
	cur = getNodeAt(position % len, originNode);

	newNode->nextNode = cur->nextNode;
	newNode->nextNode->previousNode = newNode;
	newNode->previousNode = cur;
	cur->nextNode = newNode; 
}

void deleteNode (int position,node* originNode)
{
	// TODO
	// Note: you can use a similar implementation as in ex1.
	node* cur = getNodeAt(position, originNode);
	cur->previousNode->nextNode = cur->nextNode;
	cur->nextNode->previousNode = cur->previousNode;
	deleteAllSubNodes(cur);
	free(cur); 
}

void deleteAllSubNodes (node* targetNode)
{
	// Optional TODO
	// it is advised to use a helper function in conjunction with deleteNode().
	subNode* cur = targetNode->subNodeHead;
	while(cur != NULL) {
		subNode* nxt = cur -> nextSubNode;
		free(cur);
		cur = nxt;
	}
	targetNode->subNodeHead = NULL;
}

void deleteList(node* originNode)
{
	// TODO
	// Note: you can use a similar implementation as in ex1.
	node* cur = originNode->nextNode;
	originNode->nextNode = originNode;
	originNode->previousNode = originNode;
	while(cur != originNode) {
		node* nxt = cur->nextNode;
		deleteAllSubNodes(cur);
		free(cur);
		cur = nxt;
	}
	deleteAllSubNodes(originNode);
}

//Print list has been implemented for you. 
// You may study the functions below to help you implement your own functions. 
void printList(node* originNode)
{
	int count = 0;
	printf("Printing clockwise:\n");
	node* iterator = originNode->nextNode;
	printf("[Pos %i:%i]",0,originNode->data);
	// printing subNodes
	printSubNodes(originNode);
	printf("\n   |\n   v\n");
	while(iterator != originNode)
	{
		count++;
		printf("[Pos %i:%i]",count,iterator->data);
		// printing subNodes
		printSubNodes(iterator);
		printf("\n   |\n   v\n");
		iterator = iterator->nextNode;
	}
	printf("[Pos %i:%i]",0,originNode->data);
	// printing subNodes
	printSubNodes(originNode);		
	
	printf("\nPrinting counter-clockwise:\n");
	iterator = originNode->previousNode;
	printf("[Pos %i:%i]",0,originNode->data);
	// printing subNodes
	printSubNodes(originNode);
	printf("\n   |\n   v\n");
	while(iterator!= originNode)
	{
		printf("[Pos %i:%i]",count,iterator->data);
		// printing subNodes
		printSubNodes(iterator);
		printf("\n   |\n   v\n");
		iterator = iterator->previousNode;
		count--;
	}
	printf("[Pos %i:%i]",0,originNode->data);	
	// printing subNodes
	printSubNodes(originNode);
	printf("\n");

	return;
}

void printSubNodes(node* mainNode)
{
	int count = 0;
	subNode* iterator = mainNode->subNodeHead;
	while(iterator != NULL)
	{
		printf("->[subNode pos %i:%i]",count,iterator->data);
		iterator = iterator->nextSubNode;
		count++;
	}
}
