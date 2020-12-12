#ifndef RBT_H
#define  RBT_H

#include <string>
#include <iostream>

#define SPACE_COUNT 10

/**
 * A red black tree header file containing all appropriate declarations.
 *
 * @author Vincent Nigro
 * @version 0.0.1
 */

/**
 * enumeration used for simplifying and allowing a more descriptive representation of the node 'colors'
 */ 
enum rbColor
{
    RED,
    BLACK
};

/**
 * Templated node structure that allows a generic type to be stored under the attribute data. The key attribute
 * is implemented as an unsigned long long and is left up to the developer for which the key represents. The nodes will be 
 * inserted into the red black tree using this field.
 */
template <typename T>
struct rbNode
{
    T data;
    int rbColor;
    unsigned long long key;
    rbNode* left, *right, *parent;
};

/**
 * A red black tree class definition that is based off of the rbNode structure defined above.
 */
template <class T>
class RBT
{
    public:
        RBT();
        ~RBT();
        bool isEmpty();
        void inorder();
        void preorder();
        void postorder();
        rbNode<T>* min();
        rbNode<T>* max();
        void prettyPrint();
        rbNode<T>* getRoot();
        rbNode<T>* getSentinel();
        bool del(unsigned long long key);
        rbNode<T>* search(unsigned long long key);
        void insert(unsigned long long key, T data);
        
    private:
        rbNode<T>* root;
        rbNode<T>* sentinel;

        void pdel(rbNode<T>* x);
        void leftRotate(rbNode<T>* x);
        void inorder(rbNode<T>* root);
        void preorder(rbNode<T>* root);
        void rightRotate(rbNode<T>* y);
        void postorder(rbNode<T>* root);
        rbNode<T>* pmin(rbNode<T>* root);
        rbNode<T>* pmax(rbNode<T>* root);
        void deleteTree(rbNode<T>* root);
        void rbTransplant(rbNode<T>* u, rbNode<T>* v);
        void pdelFixup(rbNode<T>* &root, rbNode<T>* &ptr);
        rbNode<T>* pinsert(rbNode<T>* root, rbNode<T>* ptr);
        void pinsertFixup(rbNode<T>* &root, rbNode<T>* &ptr);
        rbNode<T>* instantiateNode(unsigned long long key, T data);
        rbNode<T>* psearch(rbNode<T>* root, unsigned long long key);
        void pprint(rbNode<T>* root, std::string indent, bool last);
};

// Include templated implementation file as it is necessary to present during compile time
#include "rbt.tpp"

#endif