//
//  linkedList.h
//  
//
//  Created by Toomato on 20/8/18.
//

#ifndef HEADER_FILE
#define HEADER_FILE

typedef struct matrix {
    int row;
    int col;
    float data;
    struct matrix * next;
} node;

void display(node*);
void push(node* , int , int , float);
#endif

