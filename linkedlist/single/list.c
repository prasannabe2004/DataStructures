#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include <time.h>

struct node
{
    int val;
    struct node *next;
};

static struct node *head = NULL;
static struct node *curr = NULL;

static struct timeval  tv1, tv2;

struct node* create_list(int val)
{
    printf("Creating list with headnode as [%d]\n",val);
    struct node *ptr = (struct node*)malloc(sizeof(struct node));
    if(NULL == ptr)
    {
        printf("Node creation failed \n");
        return NULL;
    }
    ptr->val = val;
    ptr->next = NULL;

    head = curr = ptr;
    return ptr;
}

struct node* add_to_list(int val)
{
    if(NULL == head)
    {
        return (create_list(val));
    }

    struct node *ptr = (struct node*)malloc(sizeof(struct node));
    if(NULL == ptr)
    {
        printf("Node creation failed \n");
        return NULL;
    }
    ptr->val = val;
    ptr->next = NULL;

    curr->next = ptr;
    curr = ptr;

    return ptr;
}

struct node* search_in_list(int val, struct node **prev)
{
    struct node *ptr = head;
    struct node *tmp = NULL;
    bool found = false;

    printf("Searching the list for value [%d] \n",val);
	
	gettimeofday(&tv1, NULL);

    while(ptr != NULL)
    {
        if(ptr->val == val)
        {
            found = true;
			gettimeofday(&tv2, NULL);
			printf ("Linear Search took %f seconds\n",(double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec));
            break;
        }
        else
        {
            tmp = ptr;
            ptr = ptr->next;
        }
    }

    if(true == found)
    {
        if(prev)
            *prev = tmp;
        return ptr;
    }
    else
    {
        return NULL;
    }
}

int delete_from_list(int val)
{
    struct node *prev = NULL;
    struct node *del = NULL;

    printf("Deleting value [%d] from list\n",val);

    del = search_in_list(val,&prev);
    if(del == NULL)
    {
        return -1;
    }
    else
    {
        if(prev != NULL)
            prev->next = del->next;

        if(del == curr)
        {
            curr = prev;
        }
        else if(del == head)
        {
            head = del->next;
        }
    }

    free(del);
    del = NULL;

    return 0;
}

void print_list(void)
{
    struct node *ptr = head;

    printf("-------Printing list Start-------\n");
    while(ptr != NULL)
    {
        printf("[%d]->",ptr->val);
        ptr = ptr->next;
    }
    printf("\n");

    return;
}

void reverseList()
{
    struct node* curr = head;
    struct node* prev = NULL;
    struct node* next = NULL;

    while(curr !=NULL)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    head = prev;
}

int main(void)
{
    int i = 0, ret = 0;
    struct node *ptr = NULL;

    print_list();

    for(i = 0; i<100000000; i++)
        add_to_list(i);
    
	//print_list();

	ptr = search_in_list(99999999, NULL);
	if(NULL == ptr)
	{
    	printf("Search [val = %d] failed, no such element found\n",i);
    }
    else
    {
        printf("Search passed [val = %d]\n",ptr->val);
    }

    printf("Lets reverse the linked list\n");

    reverseList();

    return 0;
}
