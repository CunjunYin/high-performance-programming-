//
//  linkedList.h
//  
//
//  Created by Toomato on 20/8/18.
//

#ifndef HEADER_FILE
#define HEADER_FILE
#endif
typedef struct matrix {
    int row;
    int col;
    float data;
    struct matrix * next;
} queue;

void display(queue*);
void push(queue* , int , int , float);
void i_toString(int* , int, int);
void f_toString(float* , int, int);
