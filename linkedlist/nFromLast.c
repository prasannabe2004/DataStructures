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

void secondlastnode(struct node* head, int n)
{
	int j = n;
	if(head==NULL||head->next==NULL)
	{
		printf("NULL");
	}
	else
	{
		struct node* temp = head;
		struct node* curr = head;
		while(j && temp)
		{
			printf("incrementing temp\n");
			temp=temp->next;
			j--;
		}
		while(temp->next)
		{
			printf("incrementing curr and temp\n");
			curr = curr -> next;
			temp = temp->next;
		}
		printf("\nThe %d from last node is %d",n, curr->data);
	}
}

int main()
{
	char ch;
	int i,choice,position;
	struct node *head=NULL;
	int n = 0;
	printf("Enter the nth element from last node\n");
	scanf("%d",&n);
	do {
		printf("\nEnter data:");
		scanf("%d",&i);
		push(&head,i);
		printf("\ndo you wish to continue:");
		ch=getchar();
	}while(ch!='n');
	display(head);
	secondlastnode(head,n);
}
