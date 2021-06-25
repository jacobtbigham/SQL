#include "../includes/stack/stack.h"

using namespace std;

int main()
{
    Stack<int> q{};
    for (int i = 0; i < 10; ++i) q.push(i);
    Stack<int> q2(q);
    q2.pop();
    cout << "1: " << q << endl;
    cout << "2: " << q2 << endl;

    while (! q.empty())
    {
        cout << " " << q.pop() << endl;
    }

    return 0;
}