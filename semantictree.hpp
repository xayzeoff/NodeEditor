#ifndef SEMANTICTREE_HPP
#define SEMANTICTREE_HPP
#include "node.h"

struct TreeNode{
    Node::Type nodeType;
    Node* nodePointer;
    TreeNode* parent = nullptr;
    TreeNode* next1 = nullptr;
    TreeNode* next2 = nullptr;
};
class SemanticTree{
public:

    TreeNode* addNode(TreeNode* parent, Node* child){
        TreeNode* tn = new TreeNode;
        if(parent == nullptr){

            tn->nodePointer = child;
            tn->nodeType = child->getNodetype();
            first = tn;
        }else{
            tn->parent = parent;
            tn->nodePointer = child;
            tn->nodeType = child->getNodetype();
            if(tn->parent->next1 == nullptr)
                tn->parent->next1 = tn;
            else
                tn->parent->next2 = tn;
        }
        return tn;
    }
    void printBranch(TreeNode* tn){
        if(tn == nullptr) return;
        qDebug()  << tn->nodeType;
        printBranch(tn->next1);
        printBranch(tn->next2);
    }
    void printTree(){
        TreeNode* tn = first;
        printBranch(tn);
    }
    TreeNode* getNodeNext_1(TreeNode* tn){
        return tn->next1;
    }
    TreeNode* getNodeNext_2(TreeNode* tn){
        return tn->next2;
    }
    TreeNode* getFirst(){
        return this->first;
    }


private:
    TreeNode* first = nullptr;
};

#endif // SEMANTICTREE_HPP



