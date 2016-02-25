#ifndef LIST_H
#define LIST_H

#include <string>
#include <iostream>
using namespace std;

class list
{
    public:
        string name;
        list* next;
        list();
        virtual ~list();
    protected:
    private:
};

#endif // LIST_H
