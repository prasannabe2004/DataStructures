#include <iostream>
#include <sstream>
#include "list.h"

using namespace std;

list *head = NULL;

void addNode(list *node)
{
    if(!head)
    {
        head = node;
        return;
    }
    head->prev = node;
    node->next = head;
    head = node;
}

void remNode(string s)
{
    list* cur=head;

    cout << "\ndeleting " << s << endl;

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
        head->prev = NULL;
        delete cur;
    }
    //remove other nodes
    for(;cur;)
    {
        if ((cur->name).compare(s) == 0)
        {
            cout << "got it " << cur->name << endl;

            if(cur->prev->next)
                cur->prev->next = cur->next;
            if(cur->next != NULL)
                cur->next->prev = cur->prev;

            delete cur;
            break;
        }
        cur = cur->next;
    }
    return;
}
void printList()
{
    if(!head)
    {
        cout << "List is empty" <<endl;
        return;
    }
    cout << "List contains following" << endl;
    list* ptr = head;

    for(;ptr!=NULL;ptr=ptr->next)
        cout << ptr->name << "->";
    cout<<"NULL\n";
}

void destroy()
{
    for(int i=0;i<10;i++)
    {
        ostringstream ss;
        ss << i;
        string name = "node" + ss.str();
        remNode(name);
    }
}

int main()
{

    for(int i=0;i<10;i++)
    {
        list* node1 = new list;
        node1->next = NULL;
        node1->prev = NULL;

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

    destroy();

    printList();
    return 0;
}
