//
//  linkedList.c
//  
//
//  Created by Toomato on 20/8/18.
//
#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"

typedef struct liveCells {
    int row;
    int col;
    struct liveCells * next;
} node;

int main(int argc, char *argv[]){
    
}

void display(node* head){
    NODE *display = node;
    while (display->next!=NULL) {
        printf("rwo = %d, col = %d\n",display.row,display.col);
        display = display->next;
    }
}

void push(node * head, int row, int col){
    // create a new node, to append at the start
    node * new_node;
    new_node = malloc(sizeof(node));
    
    // link rechange
    new_node->val = val;
    new_node->col = col;
    new_node->next = *head;
    *head = new_node;
}


