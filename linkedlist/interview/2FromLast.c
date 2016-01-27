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
	printf("NULL");
}

void secondlastnode(struct node* head)
{
	if(head==NULL||head->next==NULL)
	{
		printf("NULL");
	}
	else
	{
		struct node* temp=head;
// Add the n times ->next for the nth element from last
		while(temp->next->next!=NULL)
		{
			temp=temp->next;
		}
		printf("\nSecond last node is %d",temp->data);
	}
}

int main()
{
	char ch;
	int i,choice,position;
	struct node *head=NULL;
	do {
		printf("\nEnter data:");
		scanf("%d",&i);
		push(&head,i);
		printf("\ndo you wish to continue:");
		ch=getchar();
	}while(ch!='n');
	display(head);
	secondlastnode(head);
}
