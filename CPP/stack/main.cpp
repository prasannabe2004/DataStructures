#include <iostream>
#include <stack>
#include <string.h>
using namespace std;

void ReverseString(char C[], int length)
{
    stack<char> S;
    int i;

    for(i=0;i<length;i++)
    {
        S.push(C[i]);
    }

    for(i=0;i<length;i++)
    {
        C[i] = S.top();
        S.pop();
    }

}
int main()
{
    char buf[]="Prasanna";
    ReverseString(buf, strlen(buf));
    cout << buf<< endl;
    return 0;
}
