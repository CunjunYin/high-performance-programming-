#ifndef HEADER_FILE
#define HEADER_FILE
#endif

typedef struct Node {
	int row;
	int col;
	float data;
	struct Node* next;
} node;

void Enqueue(node* , node*, int , int , float );
void Dequeue(node*, node* );
int Front( node* );
void Print(node*);