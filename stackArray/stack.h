#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED
 
void push(int *s,int* top, int element);
int pop(int *s,int *top);
int full(int *top,const int size);
int empty(int *top);
void init(int *top);
void display(int *s,int *top);
 
#endif // STACK_H_INCLUDED
