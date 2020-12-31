#include "rbt.h"

/**
 * A red black tree template implementation file which contains the logic for a templated red black tree. This 
 * red black tree implementation is bi-directional by utilizing the single 'sentinel' node object to represent
 * all leaves of the tree.
 * 
 * @author Vincent Nigro
 * @version 0.0.1
 */

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                                             PUBLIC INTERFACE                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Implicitly instantiates the sentinel rbNode object and populates the fields with invalid indicators and assigns
 * the root to be equal to the sentinel.
 *
 * @return this
 */
template <class T>
RBT<T>::RBT()
{
    sentinel = new rbNode<T>;
    root = sentinel;
    
    sentinel->key = -1;
    sentinel->data = nullptr;
    sentinel->rbColor = BLACK;
    
    sentinel->left = root;
    sentinel->right = root;
    sentinel->parent = root;
}

/**
 * Calls the private interface to free the entire tree from memory and removes the memory containing the sentinel
 * structure.
 * 
 * @return void
 */
template <class T>
RBT<T>::~RBT()
{
    deleteTree(root);
    
    delete sentinel;
}

/**
 * Returns true if there are no element remaining within the tree.
 * 
 * @return bool
 */ 
template <class T>
bool RBT<T>::isEmpty()
{
    if (root == sentinel)
        return true;
    return false;
}

/**
 * Public wrapper that injects the root into the private interface that prints out the inorder tree traversal 
 * with color.
 * 
 * @return void
 */
template <class T>
void RBT<T>::inorder()
{
    inorder(this->root);
    std::cout << std::endl;
}

/**
 * Public wrapper that injects the root into the private interface that prints out the preorder tree traversal
 * with color.
 * 
 * @return void
 */
template<class T>
void RBT<T>::preorder()
{
    preorder(this->root);
    std::cout << std::endl;
}

/**
 * Public wrapper that injects the root into the private interface that prints out the postorder tree traversal
 * with color.
 * 
 * @return void 
 */
template <class T>
void RBT<T>::postorder()
{
    postorder(this->root);
    std::cout << std::endl;
}

/**
 * Public wrapper that injects the root into the private interface to search for the minimum key value.
 *  
 * @return rbNode<T>*
 */
template <class T>
rbNode<T>* RBT<T>::min()
{
    return pmin(root);
}

/**
 * Public wrapper that injects the root into the private interface to search for the maximum key value.
 *
 * @return rbNode<T>*
 */
template <class T>
rbNode<T>* RBT<T>::max()
{
    return pmax(root);
}

/**
 * Public wrapper that injects the root and the appropriate starter fields for the pprint private interface.
 * 
 * @return void
 */
template <class T>
void RBT<T>::prettyPrint() 
{
	if (this->root)
    	pprint(this->root, "", true);
}

/**
 * Public wrapper for retrieving the root rbNode<T> structure for possible custom use cases with traversing the
 * tree or updating the tree. If this is desired, it must be warned that the user must express an abundance of 
 * caution and should follow the mechanisms used through the public and private interface.
 * 
 * @return rbNode<T>*
 */
template <class T>
rbNode<T>* RBT<T>::getRoot()
{
    return this->root;
}

/**
 * Public wrapper for retrieving the sentinel rbNode<T> structure for possible custom uses with traversing root
 * node and is necessary to have access to the sentinel in this scenario.
 * 
 * @return rbNode<T>*
 */
template<class T>
rbNode<T>* RBT<T>::getSentinel()
{
    return this->sentinel;
}

/**
 * Public wrapper that searches for the specified key and if found will initiate the deletion sequence and return
 * true when finished. If the key isn't in the tree then the method returns false.
 * 
 * @param unsigned long long
 * @return bool
 */
template <class T>
bool RBT<T>::del(unsigned long long key)
{
    rbNode<T>* z = search(key);

    if (z == sentinel)
        return false;
    
    pdel(z);

    sentinel->left = root;
    sentinel->right = root;
    sentinel->parent = root;

    return true;
}

/**
 * Public wrapper for searching through the tree structure for some key and will return the appropriate rbNode<T>*
 * structure. If the structure does not exist, the function will return the sentinel structure.
 * 
 * @param unsigned long long
 * @return rbNode<T>*
 */
template <class T>
rbNode<T>* RBT<T>::search(unsigned long long key)
{
    return psearch(root, key);
}

/**
 * Public wrapper for inserting some generic data type, T by creating a rbNode<T> structure containing the data
 * and respective key. Once the structure is created a private interface insert is called to insert the node in
 * the proper location. The tree will be updated to make sure the red black tree properties persist after
 * the insertion. Finally the sentinel structure will have its left & right pointers reset once the tree root
 * has been initialized.
 *
 * @param unsigned long long
 * @param T
 * @return void
 */
template <class T>
void RBT<T>::insert(unsigned long long key, T* data)
{
    bool init = false;
    
    // only true in initialization case 
    if (root == sentinel)
        init = true;
    
    // instantiate rbNode<T>* with defaults
    rbNode<T> *ptr = instantiateNode(key, data);

    // add ptr to tree
    root = pinsert(root, ptr);
    
    // Init is the deterministic condition
    if (init)
    {
        sentinel->left = root;
        sentinel->right = root;
        sentinel->parent = root;
    }

    // rotate and recolor tree
    pinsertFixup(root, ptr);

    sentinel->left = root;
    sentinel->right = root;
    sentinel->parent = root;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                                              PRIVATE INTERFACE                                          ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * The red black tree deletion procedure is an extension to the basic binary tree 
 * deletion with the following additions. We maintain node y as the node either removed
 * from the tree or moved within the tree. We need to save y's original color in both cases
 * in order to test it at the end where if it was black, then removing or moving y could cause
 * violations of the red-black properties. We also track node x that moves into node y's original
 * position. We also always set x.p to point to the original position in the tree of y's parent even 
 * if x is, in fact, the sentinel. Finally, if node y was black, we might have introduced violations
 * of red-black properties so we call pdelFixup to restore red-black properties.
 * 
 * @param rbNode<T>*
 * @return void
 */
template <class T>
void RBT<T>::pdel(rbNode<T>* z)
{
    rbNode<T>* x;
    rbNode<T>* y = z;
    rbColor yOrig = (rbColor) y->rbColor;

    if (z->left == sentinel) // no children or only right
    {
        // x contains right subtree which needs to be transplanted to move out z
        x = z->right;
        rbTransplant(z, z->right);
    }
    else if (z->right == sentinel) // only left child
    {
        // x contains left subtree which needs to be transplanted to move out z
        x = z->left;
        rbTransplant(z, z->left);
    }
    else // both children
    {
        // Get z's successor which will move into z's position in the tree
        y = pmin(z->right);
        yOrig = (rbColor) y->rbColor;
        x = y->right;
        
        /* When y's original parent is z, we do not want x's parent to point to the node
         * being removed from the tree. Because node y will move up to take z's position in
         * the tree, setting x's parent to y causes x.p to point to the original position of 
         * y's parent, even if x == sentinel.
         */
        if (y->parent == z)
            x->parent = y;
        else
        {
            /*
             * Otherwise, notice that x's parent is always set via the rbTransplant function.
             */
            rbTransplant(y, y->right);

            // reconfigure y's right pointers based on z
            y->right = z->right;
            y->right->parent = y;
        }
        // transplant z with y
        rbTransplant(z, y);

        // reconfigure y's left pointers and color based on z
        y->left = z->left;
        y->left->parent = y;
        y->rbColor = z->rbColor;
    }

    /* 
     * If the original color of y was black, we might have introduced one or more violations
     * of the red-black properties. If y was red, the red-black properties still hold when
     * y is removed or moved for the following reasons:
     *  1. No black-heights in the tree have changed
     *  2. No red nodes have been made adjacent. Because y takes z's place in the tree, along
     *  with z's color, we cannot have two adjacent red nodes at y's new position in the tree.
     *      - In addition, if y was not z's right child, then y's original right child x replaces
     *      y in the tree. If y is red, then x must be black, and so replacing y by x cannot 
     *      cause two red nodes to become adjacent.
     *  3. Since y could not have been the root if it was red, the root remains black.
     */
    if (yOrig == BLACK)
        pdelFixup(this->root, x);
    
    delete z;
}

/**
 * Performs a left rotation such that if X has a left child named A, and a right child named Y where Y has a 
 * left child named B and a right child named Z; the following will be the result. Y will have a left child of 
 * X and a right child of Z, X will have a left child of A and a right child of B.
 * 
 * @param rbNode<T>*
 * @return void
 */
template <class T>
void RBT<T>::leftRotate(rbNode<T>* x)
{
    rbNode<T>* y = x->right;
    x->right = y->left;
    
    if (y->left != sentinel)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == sentinel)
        this->root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else 
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

/**
 * Private method which prints the inorder traversal of keys regarding the current state of the tree with color.
 * 
 * @param rbNode<T>*
 * @return void 
 */
template <class T>
void RBT<T>::inorder(rbNode<T>* root)
{
    if (root == sentinel)
        return;
    
    inorder(root->left);
    
    if (root->rbColor == BLACK)
            std::cout << "\033[1;30m" << root->key << " \033[0m";
        else // RED
            std::cout << "\033[1;31m" << root->key << " \033[0m";

    inorder(root->right);
}

/**
 * Private method which prints the preorder traversal of keys regarding the current state of the tree with color.
 * 
 * @param rbNode<T>*
 * @return void 
 */
template <class T>
void RBT<T>::preorder(rbNode<T>* root)
{
    if (root == sentinel)
        return;
    
    if (root->rbColor == BLACK)
            std::cout << "\033[1;30m" << root->key << " \033[0m";
        else // RED
            std::cout << "\033[1;31m" << root->key << " \033[0m";
    
    preorder(root->left);
    
    preorder(root->right);
}

/**
 * Performs a right rotation such that if Y has a left child named X, and a right child named Z where X has a 
 * left child named A and a right child named B; the following will be the result. X will have a left child of 
 * A and a right child of Y, Y will have a left child of B and a right child of Z.
 * 
 * @param rbNode<T>*
 * @return void
 */
template <class T>
void RBT<T>::rightRotate(rbNode<T>* y)
{
    rbNode<T>* x = y->left;
    y->left = x->right;

    if (x->right != sentinel)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == sentinel)
        this->root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else 
        y->parent->right = x;

    x->right = y;
    y->parent = x;
}

/**
 * Private method which prints the preorder traversal of keys regarding the current state of the tree with color.
 * 
 * @param rbNode<T>*
 * @return void 
 */
template <class T>
void RBT<T>::postorder(rbNode<T>* root)
{
    if (root == sentinel)
        return;
    
    postorder(root->left);
    
    postorder(root->right);

    if (root->rbColor == BLACK)
            std::cout << "\033[1;30m" << root->key << " \033[0m";
        else // RED
            std::cout << "\033[1;31m" << root->key << " \033[0m";
}

/**
 * Private method which traverses the tree from the root in order to find the minimum currently in the tree struct.
 * The minimum is determined by the key value of the rbNode<T> structure and is left up to the use case for what
 * the key value represents.
 *  
 * @param rbNode<T>*
 * @return rbNode<T>*
 */
template <class T>
rbNode<T>* RBT<T>::pmin(rbNode<T>* root)
{
    while (root->left != sentinel)
        root = root->left;
    return root;
}

/**
 * Private method which traverses the tree from the root in order to find the maximum currently in the tree struct.
 * The maximum is determined by the key value of the rbNode<T> structure and is left up to the use case for what
 * the key value represents.
 */
template <class T>
rbNode<T>* RBT<T>::pmax(rbNode<T>* root)
{
    while (root->right != sentinel)
        root = root->right;
    return root;
}

/**
 * Releases resources saved in the RBT in a depth first manner which propagates all the way up to the root.
 * 
 * @param rbNode<T>*
 * @return void
 */
template <class T>
void RBT<T>::deleteTree(rbNode<T>* root)
{
    if (root->left == sentinel && root->right == sentinel) // leaf node
    {
        if (root != sentinel)
            delete root; 
    }
    else if (root->left == sentinel) // left leaf branch
    {
        deleteTree(root->right);

        if (root != sentinel)
            delete root;
    }
    else if (root->right == sentinel) // right leaf branch
    {
        deleteTree(root->left);

        if (root != sentinel)
            delete root;
    }
    else // internal node with active branches
    {
        deleteTree(root->left);
        deleteTree(root->right);

        if (root != sentinel)
            delete root;
    }
}

/**
 * In order to move subtrees around the red-black tree we define a routine
 * which replaces one subtree as a child of its parent with another subtree.
 * When replacing the subtree rooted at node u with the subtree rooted at node v,
 * node u's parent becomes node v's parent and u's parent ends up having v as its
 * appropriate child.
 * 
 * @param rbNode<T>*
 * @param rbNode<T>*
 */
template <class T>
void RBT<T>::rbTransplant(rbNode<T>* u, rbNode<T>* v)
{
    if (u->parent == sentinel) // if u is root
        root = v; 
    else if (u == u->parent->left) // if u is left child
        u->parent->left = v;
    else
        u->parent->right = v; // if u is right child
    
    // "Exploit" ability to assign v.p when v == T.nil
    v->parent = u->parent;
}

/**
 * When called 3 problems may have arose from the red black tree pdel. First, if y had been
 * the root and a red child of y becomes the new root, we have violated the root must be black
 * property. Secondly, if both x and x.p are red, then we have violated that if a node is red,
 * then both its children are black property. Thirdly, moving y within the tree causes any simple
 * path that previously contained y to have one fewer black nodes, thus the "for each node, all
 * simple paths from the node to decendant leaves contain the same number of black nodes" property
 * is now violated by any ancester of y in the tree.
 * 
 * @param rbNode<T>* &
 * @param rbNode<T>* &
 * @return void
 */
template <class T>
void RBT<T>::pdelFixup(rbNode<T>* &root, rbNode<T>* &ptr)
{
    /* Goal: Continue to move the 'extra black' up the tree until:
     *      1. ptr points to a red-and-black node, which we color ptr (singly) black at end.
     *      2. ptr points to the root, in which case we simply "remove" the extra black, or
     *      3. Having performed suitable rotations and recolorings, we exit.
     */
    while ((ptr != root) && (ptr->rbColor == BLACK))
    {
        // While in the loop, ptr always points to a nonroot, doubly black node.

        /* Case A:
         * If ptr is a left child of parent x.p
         */
        if (ptr == ptr->parent->left)
        {
            // sibling of ptr
            rbNode<T>* w = ptr->parent->right;

            /* Case 1A:
             * Sibling is red
             * 
             * Since w must have black children, we can switch the colors of w and ptr->parent
             * then perform a left-rotation on ptr->parent without violating any of the red-black
             * properties. The new sibling of x, which is one of w's children prior to the rotation,
             * is now black.
             */
            if (w->rbColor == RED)
            {
                w->rbColor = BLACK;
                ptr->parent->rbColor = RED;
                leftRotate(ptr->parent);
                w = ptr->parent->right;
            }

            /* Case 2A:
             * Sibling w is black and both of w's children are black
             * 
             * Since w and its children are black, we take one black off both ptr and w, leaving
             * ptr with only one black and leaving w red. To compensate for removing one black from
             * ptr and w, we would like to add an extra black to ptr->parent, which was originally
             * red or black. We do so by repeating the while loop with ptr->parent as the new ptr.
             */
            if (w->left->rbColor == BLACK && w->right->rbColor == BLACK)
            {
                w->rbColor = RED;
                ptr = ptr->parent;
            }

            /* Case 3A:
             * Ptr's sibling w is black, w's left child is red, and w's right child is black
             * 
             * We can switch out the colors of w and its left child w->left and the perform a 
             * right rotation on w without violating any red-black properties. The new sibling
             * w of x is now a black node with a red right child.
             */
            else if (w->right->rbColor == BLACK)
            {
                w->left->rbColor = BLACK;
                w->rbColor = RED;
                rightRotate(w);
                w = ptr->parent->right;
            }

            /* Case 4A:
             * Ptr's sibling w is black, and w's right child is red
             * 
             * By making some color changes and performing a left rotation on ptr->parent, we
             * can remove the extra black on ptr, making it singly black, without violating
             * any of the red-black properties. Setting ptr to be root causes the while loop
             * to terminate.
             */
            else
            {
                w->rbColor = ptr->parent->rbColor;
                ptr->parent->rbColor = BLACK;
                w->right->rbColor = BLACK;
                leftRotate(ptr->parent);
                ptr = root;
            }
        }
        /* Case B:
         * If ptr is a right child of parent x.p
         */ 
        else
        {
            // sibling of ptr
            rbNode<T>* w = ptr->parent->left;

            /* Case 1B:
             * Sibling is red
             * 
             * Since w must have black children, we can switch the colors of w and ptr->parent
             * then perform a right-rotation on ptr->parent without violating any of the red-black
             * properties. The new sibling of x, which is one of w's children prior to the rotation,
             * is now black.
             */
            if (w->rbColor == RED)
            {
                w->rbColor = BLACK;
                ptr->parent->rbColor = RED;
                rightRotate(ptr->parent);
                w = ptr->parent->left;
            }

            /* Case 2B:
             * Sibling w is black and both of w's children are black
             * 
             * Since w and its children are black, we take one black off both ptr and w, leaving
             * ptr with only one black and leaving w red. To compensate for removing one black from
             * ptr and w, we would like to add an extra black to ptr->parent, which was originally
             * red or black. We do so by repeating the while loop with ptr->parent as the new ptr.
             */
            if (w->right->rbColor == BLACK && w->left->rbColor == BLACK)
            {
                w->rbColor = RED;
                ptr = ptr->parent;
            }

            /* Case 3B:
             * Ptr's sibling w is black, w's right child is red, and w's left child is black
             * 
             * We can switch out the colors of w and its right child w->right and the perform a 
             * left rotation on w without violating any red-black properties. The new sibling
             * w of x is now a black node with a red left child.
             */
            else if (w->left->rbColor == BLACK)
            {
                w->right->rbColor = BLACK;
                w->rbColor = RED;
                leftRotate(w);
                w = ptr->parent->left;
            }

            /* Case 4B:
             * Ptr's sibling w is black, and w's left child is red
             * 
             * By making some color changes and performing a right rotation on ptr->parent, we
             * can remove the extra black on ptr, making it singly black, without violating
             * any of the red-black properties. Setting ptr to be root causes the while loop
             * to terminate.
             */
            else
            {
                w->rbColor = ptr->parent->rbColor;
                ptr->parent->rbColor = BLACK;
                w->left->rbColor = BLACK;
                rightRotate(ptr->parent);
                ptr = root;
            }
        }
    }
    ptr->rbColor = BLACK;
}

/**
 * A simple recursive binary tree insertion call that is used within the private interface for just adding a new
 * node into the tree structure by traversing down the appropriate subtrees until the sentinel is reached.
 *  
 * @param rbNode<T>*
 * @param rbNode<T>*
 * @return rbNode<T>*
 */
template <class T>
rbNode<T>* RBT<T>::pinsert(rbNode<T>* root, rbNode<T>* ptr)
{
    if (root == sentinel)
        return ptr;

    if (ptr->key > root->key)
    {
        root->right = pinsert(root->right, ptr);
        root->right->parent = root;
    }
    else
    {
        root->left = pinsert(root->left, ptr);
        root->left->parent = root;
    }
    return root;
}

/**
 * Maintains the red-black tree properties by continually recoloring and rotating the tree from bottom to top
 * until the while loop condition is no longer statisfied.
 * 
 * @param rbNode<T>* &
 * @param rbNode<T> *&
 * @return void
 */
template <class T>
void RBT<T>::pinsertFixup(rbNode<T>* &root, rbNode<T>* &ptr)
{
    rbNode<T>* parent_ptr = sentinel;
    rbNode<T>* grandparent_ptr = sentinel;

    // While current ptr is not the root and ptr is red and its parent is also red (adjacent red nodes)
    while ((ptr != root) && (ptr->rbColor != BLACK) && (ptr->parent->rbColor == RED))
    {
        parent_ptr = ptr->parent;
        grandparent_ptr = ptr->parent->parent;

        /*
         * Case: A
         * Parent of ptr is left child of grandparent of ptr
         */
        if (parent_ptr == grandparent_ptr->left)
        {
            rbNode<T>* uncle_ptr = grandparent_ptr->right;
            
            /*
             * Case: 1
             * The uncle of ptr is also red only recoloring is required
             */
            if (uncle_ptr != sentinel && uncle_ptr->rbColor == RED)
            {
                grandparent_ptr->rbColor = RED;
                parent_ptr->rbColor = BLACK;
                uncle_ptr->rbColor = BLACK;
                ptr = grandparent_ptr;
            }
            else
            {
                /*
                 * Case: 2
                 * ptr is right child of its parent left rotation is required
                 */
                if (ptr == parent_ptr->right)
                {
                    leftRotate(parent_ptr);
                    ptr = parent_ptr;
                    parent_ptr = ptr->parent;
                }

                /*
                 * Case: 3
                 * ptr is left child of its parent right rotation is required
                 */
                rightRotate(grandparent_ptr);
                rbColor tmp = (rbColor) parent_ptr->rbColor;
                parent_ptr->rbColor = grandparent_ptr->rbColor;
                grandparent_ptr->rbColor = tmp;
                ptr = parent_ptr;
            }
        }

        /*
         * Case: B
         * Parent of ptr is right child of grandparent of ptr
         */ 
        else 
        {
            rbNode<T>* uncle_ptr = grandparent_ptr->left;
            
            /*
             * Case: 1
             * The uncle of ptr is also red only recoloring is required
             */
            if ((uncle_ptr != sentinel) && (uncle_ptr->rbColor == RED))
            {
                grandparent_ptr->rbColor = RED;
                parent_ptr->rbColor = BLACK;
                uncle_ptr->rbColor = BLACK;
                ptr = grandparent_ptr;
            }
            else
            {
                /*
                 * Case: 2
                 * ptr is left child of its parent right rotation is required
                 */ 
                if (ptr == parent_ptr->left)
                {
                    rightRotate(parent_ptr);
                    ptr = parent_ptr;
                    parent_ptr = ptr->parent;
                }

                /*
                 * Case: 3
                 * ptr is right child of its parent left rotation is required
                 */
                leftRotate(grandparent_ptr);
                rbColor tmp = (rbColor) parent_ptr->rbColor;
                parent_ptr->rbColor = grandparent_ptr->rbColor;
                grandparent_ptr->rbColor = tmp;
                ptr = parent_ptr;
            }
        }
    }
    root->rbColor = BLACK;
}

/**
 * Used during the insertion procedure in order to generate a generic node containing the key & data passed.
 * The left, right and parent pointers will point to the sentinel until updated at a later point.
 * Lastly the rbColor is always set to rbColor::RED until otherwise updated later during recoloring phases.
 * 
 * @param unsigned long long
 * @param T
 * @return rbNode<T>*
 */
template <class T>
rbNode<T>* RBT<T>::instantiateNode(unsigned long long key, T* data)
{
    rbNode<T>* n = new rbNode<T>;
    n->key = key;
    n->data = data;
    n->left = sentinel;
    n->right = sentinel;
    n->parent = sentinel;
    n->rbColor = RED;
    return n;
}

/**
 * Recursively traverses through the subtrees within the tree until the desired rbNode with the same key
 * is found or sentinel is reached. If the desired node is found it will be returned, otherwise sentinel 
 * is returned.
 * 
 * @param rbNode<T>*
 * @param unsigned long long
 * @return rbNode<T>* 
 */
template <class T>
rbNode<T>* RBT<T>::psearch(rbNode<T>* root, unsigned long long key)
{
    if (root == sentinel)
        return sentinel;
    else if (root->key == key)
        return root;
    
    if (key > root->key)
        return psearch(root->right, key);
    else
        return psearch(root->left, key);
}

/**
 * Recursively prints out the red black tree from top to bottom. If the terminal supports color, the nodes
 * will be colored respective to the color they possess at the time of the printout.
 * 
 * @param rbNode<T>*
 * @param string
 * @param bool
 * @return void
 */
template <class T>
void RBT<T>::pprint(rbNode<T>* root, std::string indent, bool last) 
{
    if (root != sentinel) 
    {
        std::cout << indent;
        if (last)
        {
            std::cout<<"R----";
            indent += "     ";
        } 
        else 
        {
            std::cout<<"L----";
            indent += "|    ";
        }

        if (root->rbColor == BLACK)
            std::cout << "\033[1;30m" << root->key <<"("<<"BLACK"<<")" << "\033[0m" << std::endl;
        else // RED
            std::cout << "\033[1;31m" << root->key <<"("<<"RED"<<")" << "\033[0m" << std::endl;
        pprint(root->left, indent, false);
        pprint(root->right, indent, true);
    }
}