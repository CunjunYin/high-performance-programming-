#ifndef HEADER_FILE
#define HEADER_FILE
#endif

struct Node {
	int row;
	int col;
	float data;
	struct Node* next;
};

void Enqueue(int row, int col, float data);
void Dequeue();
int Front();
void Print();