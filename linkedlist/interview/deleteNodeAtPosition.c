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
	while(head!=NULL)
	{
		printf("%d-->",head->data);
		head=head->next;

	}
	printf("NULL\n");
}

void deleteNode(struct node **head, int p)
{
	struct node *temp = *head;
	int i =0;

	if(head == NULL)
		return;
	if(p == 0)
	{
		*head = temp->next;
		free(temp);
		return;
	}

	for (i =0;i<p-1;i++)
	{
		if(temp->next)
			temp = temp->next;
		else
			return;
	}
	free(temp->next);
	temp->next = temp->next->next;
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
	display(head);
	deleteNode(&head,0);
	display(head);
	deleteNode(&head,21);
	display(head);
}
