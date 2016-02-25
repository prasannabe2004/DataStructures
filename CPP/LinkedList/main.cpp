#include <iostream>
#include <sstream>
#include "list.h"

using namespace std;

list *head = NULL;

void addNode(list *node)
{
    node->next = head;
    head = node;
}

void remNode(string s)
{
    list* pre;
    list* cur=head;

    cout << "delete " << s << endl;

    //Empty list check
    if(!cur)
    {
        cout << "head is null"<< endl;
        return;
    }
    //Only one node list
    if(!cur->next && (cur->name).compare(s) == 0)
    {
        cout << "only one node" << endl;
        delete cur;
        head = NULL;
        return;
    }
    //Remove head node
    if ((cur->name).compare(s) == 0)
    {
        cout << "removing head" << endl;
        head = cur->next;
        delete cur;
    }
    //remove other nodes
    for(;cur;)
    {
        if ((cur->name).compare(s) == 0)
        {
            cout << "got it " << cur->name << endl;
            pre->next = cur->next;
            delete cur;
            break;
        }
        pre = cur;
        cur = cur->next;
    }
    return;
}
void printList()
{
    list* ptr = head;

    for(;ptr!=NULL;ptr=ptr->next)
        cout << ptr->name << "->";
    cout<<"NULL\n";
}

int main()
{
    for(int i=0;i<10;i++)
    {
        list* node1 = new list;
        node1->next = NULL;
        ostringstream ss;
        ss << i;
        node1->name = "node" + ss.str();
        addNode(node1);
    }
    printList();

//Removing node in middle
    remNode("node4");
    printList();

// Removing the head node
    remNode("node9");
    printList();

//Removing last node
    remNode("node0");
    printList();
/*
    for(int i=0;i<10;i++)
    {
        ostringstream ss;
        ss << i;
        string name = "node" + ss.str();
        remNode(name);
    }
*/
    return 0;
}
