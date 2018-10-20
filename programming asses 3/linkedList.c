//
//  linkedList.c
//  
//
//  Created by Toomato on 20/8/18.
//
#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"

void display(node* head){
    node *display = head;
    while (display->next!=NULL) {
        printf("%d %d %f\n",display->row,display->col,display->data);
        display = display->next;
    }
}

void push(node * head, int row, int col,float data){
    // create a new node, to append at the start
    node * new_node;
    new_node = malloc(sizeof(node));
    
    // link rechange
    new_node->row = row;
    new_node->col = col;
    new_node->data = data;
    new_node->next = head;
    *head = *new_node;
}


