#include<stdio.h>
#include<stdlib.h>
#include "LinkedQueue.h"
// Two glboal variables to store address of front and rear nodes. 
struct Node* front = NULL;
struct Node* rear = NULL;

// To Enqueue an integer
void Enqueue(int row, int col, float data) {
	struct Node* temp = 
		(struct Node*)malloc(sizeof(struct Node));
	temp->row = row;
	temp->col =col; 
	temp-> data = data;
	temp->next = NULL;
	if(front == NULL && rear == NULL){
		front = rear = temp;
		return;
	}
	rear->next = temp;
	rear = temp;
}

// To Dequeue an integer.
void Dequeue() {
	struct Node* temp = front;
	if(front == NULL) {
		printf("Queue is Empty\n");
		return;
	}
	if(front == rear) {
		front = rear = NULL;
	}
	else {
		front = front->next;
	}
	free(temp);
}

int Front() {
	if(front == NULL) {
		printf("Queue is empty\n");
	}
	return front->data;
}

void Print() {
	struct Node* temp = front;
	while(temp != NULL) {
		printf("%d, %d, %f ",temp->row, temp->col, temp->data);
		temp = temp->next;
	}
	printf("\n");
}

/*int main(){
	// Printing elements in Queue after each Enqueue or Dequeue 
	Enqueue(2,3,777); Print(); 
	Enqueue(4,5,666); Print();
	Enqueue(6,7,888); Print();
	Dequeue();  Print();
	Enqueue(8,9,1000); Print();
}*/
