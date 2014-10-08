#include"red_black_tree.h"
#include<stdio.h>
#include<ctype.h>
#include <string.h>

/*  this file has functions to test a red-black tree of integers */

void IntDest(void* a) 
{
    free((int*)a);
}

int IntComp(const void* a,const void* b) 
{
    if( *(int*)a > *(int*)b) return(1);
    if( *(int*)a < *(int*)b) return(-1);
    return(0);
}

int StrComp(const void* a,const void* b) 
{
    /* Compare the Domain name and return the value to RB Tree. */
    if(strcasecmp((char *)a,(char *)b) > 0 ) 
    {
        return(1); /* String "a" is greater than string "b" */
    }
    else if(strcasecmp((char *)a,(char *)b) < 0)
    {
        return (-1); /* String "b" is greater than string "a" */
    }
    /* Both the strings are equal. */
    return(0);
}

void IntPrint(const void* a) 
{
    printf("%s",(char*)a);
}

void InfoPrint(void* a) 
{
    rb_red_blk_node* newNode = (rb_red_blk_node*)a;
    if(newNode)
        printf("%s->", (char *)newNode->key);
}

void InfoDest(void *a)
{
    ;
}

int main() 
{
    stk_stack* enumResult;
    int option=0;
    char newKey[512];
    char newKey2[512];
    char* newInt;
    rb_red_blk_node* newNode;
    rb_red_blk_tree* tree;

    tree=RBTreeCreate(StrComp,IntDest,InfoDest,IntPrint,InfoPrint);
    while(option!=8) 
    {
        printf("choose one of the following:\n");
        
        printf("\n (a) add to tree\n\n (d) delete from tree\n\n (q) query\n\n (p) find predecessor\n\n (s) find sucessor\n\n (6) enumerate\n\n (x) print tree\n\n (e) quit\n\n");
        
        do 
            option=fgetc(stdin); 
        while(-1 != option && isspace(option));
        /* option-='0'; */
        switch(option)
        {
            case 'a':
            {
                printf("type key for new node\n");
                scanf("%s",newKey);
                newInt=(char*) malloc(strlen(newKey)+1);
                strcpy(newInt,newKey);
                RBTreeInsert(tree,newInt,0);
            }
            break;

            case 'd':
            {
                printf("type key of node to remove\n");
                scanf("%s",newKey);
                if ( ( newNode=RBExactQuery(tree,newKey ) ) ) 
                    RBDelete(tree,newNode);/*assignment*/
                else
                    printf("key not found in tree, no action taken\n");
            }
            break;

            case 'q':
            {
                printf("type key of node to query for\n");
                scanf("%s",newKey);
                if ( ( newNode = RBExactQuery(tree,newKey) ) ) 
                {/*assignment*/
                    printf("data found in tree at location %i\n",(int)newNode);
                    printf("data found in tree is %s\n",(char*)newNode->key);
                } 
                else 
                {
                    printf("data not in tree\n");
                }
            }
            break;
            case 'p':
            {
                printf("type key of node to find predecessor of\n");
                scanf("%s",newKey);
                if ( ( newNode = RBExactQuery(tree,newKey) ) ) 
                {/*assignment*/
                    newNode=TreePredecessor(tree,newNode);
                    if(tree->nil == newNode) 
                    {
                        printf("there is no predecessor for that node (it is a minimum)\n");
                    } else 
                    {
                        printf("predecessor has key %s\n",(char*)newNode->key);
                    }
                }
                else
                {
                    printf("data not in tree\n");
                }
            }
            break;
            case 's':
            {
                printf("type key of node to find successor of\n");
                scanf("%s",newKey);
                if ( (newNode = RBExactQuery(tree,newKey) ) ) 
                {
                    newNode=TreeSuccessor(tree,newNode);
                    if(tree->nil == newNode) 
                    {
                        printf("there is no successor for that node (it is a maximum)\n");
                    }
                    else
                    {
                        printf("successor has key %s\n",(char*)newNode->key);
                    }
                }
                else
                {
                    printf("data not in tree\n");
                }
            }
            break;
            case 6:
            {
                printf("type low and high keys to see all keys between them\n");
                scanf("%s %s",newKey,newKey2);
                enumResult=RBEnumerate(tree,&newKey,&newKey2);	  
                while ( (newNode = StackPop(enumResult)) ) 
                {
                    tree->PrintKey(newNode->key);
                    printf("\n");
                }
                free(enumResult);
            }
            break;
            case 'x':
            {
                RBTreePrint(tree);
            }
            break;
            case 'e':
            {
                RBTreeDestroy(tree);
                return 0;
            }
            break;
            default:
                printf("Invalid input; Please try again.\n");
        }
    }
    return 0;
}




