#include <vector>
#include <iostream>
#include "include/rbt.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                                                DEMO DRIVER                                              ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Main controller for demoing a simple RBT class implementation which holds int types as its data.
 * 
 * @param int
 * @param char**
 * @return int
 */
int main(int argc, char** argv)
{
    RBT<int> tree = RBT<int>();
    
    cout << "Enter 1 to insert a new node into the tree." << endl;
    cout << "Enter 2 to delete a node from the tree." << endl;
    cout << "Enter 3 to print out the inorder representation of the tree." << endl;
    cout << "Enter 4 to print out the preorder representation of the tree." << endl;
    cout << "Enter 5 to print out the postorder representation of the tree." << endl;
    cout << "Enter -1 to quit the program." << endl;

    do
    {
        int key = -1;
        cout << "Entry: ";
        cin >> key;
        
        if (key == -1)
            break;
        
        if (key == 1) // insertion selection
        {
            cout << "Enter key (-1 to quit) to insert: ";
            cin >> key;

            if (key == -1)
                break;
            
            // The tree uses the key for its key and data fields. The trees' generics are for the data portion of the node.
            tree.insert(key, &key); //(key, data)
            tree.prettyPrint();
        }
        else if (key == 2) // deletion selection
        {
            cout << "Enter key (-1 to quit) to remove: ";
            cin >> key;

            if (key == -1)
                break;
            
            tree.del(key);
            tree.prettyPrint();
        }
        else if (key == 3)
            tree.inorder();
        else if (key == 4)
            tree.preorder();
        else if (key == 5)
            tree.postorder();
        else
        {
            cout << "Invalid option please try again with a valid entry." << endl;
        }
    } while ((tree.getRoot() != tree.getSentinel()));

    return 0;
}