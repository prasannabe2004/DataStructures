#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <string>
using namespace std;

class list
{
    public:
        list();
        virtual ~list();
        string name;
        list* prev;
        list* next;
    protected:
    private:

};

#endif // LIST_H
