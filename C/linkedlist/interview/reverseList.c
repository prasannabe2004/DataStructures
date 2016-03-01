#include <stdio.h>
#include <stdlib.h>
struct node{
	int data;
	struct node* next;
};

void display(struct node* head);

void push (struct node** head_ref,int new_data)
{
	struct node *new_node=(struct node*)malloc(sizeof(struct node));
	new_node->data=new_data;
	new_node->next=(*head_ref);
	(*head_ref)=new_node;
}

void displayReverse(struct node* head)
{
	if (head==NULL)
		printf("NULL");
	else
	{
		display(head->next);
		printf("->%d",head->data);	
	}
}
void display(struct node* head)
{
	struct node* tmp = head;
	if (tmp==NULL)
		printf("NULL");
	for(;tmp;)
	{
		printf("->%d",tmp->data);
		tmp = tmp->next;	
	}
	printf("\n");
}
void freenodes(struct node* head)
{
	struct node* tmp = head;
	for(;head;)
	{
		tmp=head;
		printf("->%d",tmp->data);
		head = tmp->next;
		free(tmp);

	}
	printf("\n");
}

void reverseList(struct node** head)
{
	struct node* curr = *head;
	struct node* prev = NULL;
	struct node* next;
	
	while(curr)
	{
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	*head = prev;
}

int main()
{
	char ch;
	int i,choice,position;
	struct node *head=NULL;

	push(&head,1);
	push(&head,2);
	push(&head,3);
	push(&head,4);
	push(&head,5);
	printf("Printing the list\n");
	display(head);
	printf("\n");
	printf("Reversing teh list\n");
	reverseList(&head);
	display(head);
	printf("\n");
	freenodes(head);
}
