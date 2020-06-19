#include <iostream>
#include <limits>
#include <stack>

using namespace std;

// Рекурсивный обход дерева
void minTreeRecursive(int value, int& record)
{
    if(value < record)
        record = value;
    if(value > 1)
    {
        // Потомки данного узла
        minTreeRecursive(value-1, record);
        minTreeRecursive(value-1, record);
    }
}

// Замена рекурсии на стек
void minTreeStack(int rootValue, int& record)
{
    // Узлы дерева, которые нужно обработать
    stack<int> nodes;
    nodes.push(rootValue);
    
    while(!nodes.empty())
    {
        int value = nodes.top();
        nodes.pop();
        if(value < record)
            record = value;
        if(value > 1)
        {
            // Потомки данного узла
            nodes.push(value-1);
            nodes.push(value-1);
        }
    }
}


int main()
{
    int recordRecursive = numeric_limits<int>::max();
    minTreeRecursive(15, recordRecursive);
    
    int recordStack = numeric_limits<int>::max();
    minTreeStack(15, recordStack);
    
    cout << "Result (recursive): " << recordRecursive << "\n";
    cout << "Result (stack):     " << recordStack     << "\n";
    
    return 0;
}
