#include <iostream>
#include <stack>
#include <string.h>

using namespace std;

bool ReverseString(char C[], int length)
{
    stack<char> S;
    int i;

    for(i=0;i<length;i++)
    {
        if(C[i] == '{' || C[i] == '(' || C[i] == '[')
        {
            cout << "Pushing " << C[i] << endl;
            S.push(C[i]);
        }
        else if ((C[i] == '}' || C[i] == ')' || C[i] == ']'))
        {
            cout << "Stack is " << S.empty() << endl ;
            if(!S.empty())
            {
                char ch = S.top();
                //cout << "Top is " << ch << endl;
                //cout << "Stack is " << S.empty() << endl ;
                if(((ch == '(' && C[i] == ')') || (ch == '[' && C[i] == ']') || (ch == '{' && C[i] == '}')))
                {
                    cout << "Poping " << C[i] << endl;
                    S.pop();
                }
            }
            else
            {
                cout << "Not a balance parenthesis 1" <<endl;
                return false;
            }
        }
    }
    if(S.empty())
    {
        cout << "Balanced parenthesis" << endl;
        return true;
    }
    else
    {
        cout << "Not a balance parenthesis 2" <<endl;
        return false;
    }

}
int main()
{
    char buf[]="(){}()";
    ReverseString(buf, strlen(buf));
    return 0;
}
