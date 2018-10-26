//
//  linkedList.c
//  
//
//  Created by Toomato on 20/8/18.
//
#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"

void display(queue *head){
    if(head == NULL){
        printf("NULL\n");
    }
    else{
        printf("%f\n", head -> data);
        display(head->next);
    }
}

int isempty(queue *q){
    return (q->next == NULL);
}


void push(queue* node, int row, int col, float data){
    queue* temp = malloc(sizeof(queue));
    temp->row = row;
    temp->col = col;
    temp->data = data;
}

void i_toString(int* a, int start, int end){
    for(int i= start; i <= end; i++){
        printf("%d ",a[i]);
    }
    printf("\n");
}

void f_toString(float* a, int start, int end){
    for(int i= start; i <= end; i++){
        printf("%f ",a[i]);
    }
    printf("\n");
}

