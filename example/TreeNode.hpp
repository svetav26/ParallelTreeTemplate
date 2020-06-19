#ifndef TREENODE_HPP
#define TREENODE_HPP

#include <ParallelTree.hpp>

// Узел дерева вариантов. В данном случае каждый узел содержит одно число.
class TreeNode : public Node
{
public:
    TreeNode(int number) :
        number(number) {}
    
    int number;
};

#endif // TREENODE_HPP
