#include<stdio.h>
#include<stdlib.h>
struct node{
	int data;
	struct node* next;
};

void push (struct node** head_ref,int new_data)
{
	struct node *new_node=(struct node*)malloc(sizeof(struct node));
	new_node->data=new_data;
	new_node->next=(*head_ref);
	(*head_ref)=new_node;
}

void display(struct node* head)
{
	if (head==NULL)
		printf("NULL");
	else
	{
		display(head->next);
		printf("->%d",head->data);	
	}
}

void display2(struct node* head)
{
	while(head!=NULL)
	{
		printf("%d->",head->data);
		head=head->next;
	}
	printf("NULL\n");
}

void count(struct node* head)
{
	int count = 0;
	while(head)
	{
		count++;
		head = head->next;
	}
	printf("Number of nodes=%d\n",count);
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
	display2(head);
	printf("Printing using recursive\n");
	display(head);
	printf("\n");
	count(head);
}
