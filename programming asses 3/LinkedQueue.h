#ifndef HEADER_FILE
#define HEADER_FILE
#endif

struct Node {
	int row;
	int col;
	float data;
	struct Node* next;
};

void Enqueue(struct Node* front, struct Node* rear, int row, int col, float data);
void Dequeue(struct Node* front, struct Node* rear);
int Front(struct Node* front);
void Print(struct Node* front);