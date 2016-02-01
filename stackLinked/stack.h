/*
 * File:    stack.h
 * Author:  zentut.com
 * Purpose: linked stack header file
 */
#ifndef LINKEDSTACK_H_INCLUDED
#define LINKEDSTACK_H_INCLUDED
 
int empty(struct node *s);
struct node* push(struct node *s,int data);
struct node* pop(struct node *s,int *data);
void init(struct node* s);
void display(struct node* head);
#endif // LINKEDSTACK_H_INCLUDED