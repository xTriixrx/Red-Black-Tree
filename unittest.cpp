#include "rbt.h"
#include <time.h>
#include <vector>
#include <chrono>
#include <cassert>
#include <iomanip>
#include <stdlib.h>
#include <iostream>

using namespace std;
using namespace chrono;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                                              UNIT TESTING SUITE                                         ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
bool isBlack(rbNode<T>*);

template <class T>
bool allRedOrBlack(rbNode<T>*, rbNode<T>*);

template <class T>
void performRedBlackPropertyAssertion(RBT<T>*);

template <class T>
bool redHasBlackChildren(rbNode<T>*, rbNode<T>*);

void logStatus(unsigned long long*, unsigned long long*, string);

template <class T>
bool allSimplePathsHaveSameBlackNodeCount(rbNode<T>*, rbNode<T>*);

template <class T>
void getSimplePathBlackCounts(rbNode<T>*, rbNode<T>*, vector<int>&, int = 0, int = 0);

/**
 * Main controller for testing the RBT class implementation.
 * 
 * @param int
 * @param char**
 * @return int
 */
int main(int argc, char** argv)
{
    int lastLog = 0;
    srand(time(NULL));  // Initialize random seed
    milliseconds elapsed, end;
    RBT<bool> tree = RBT<bool>();
    
    unsigned long long nodeGenCount = rand() % (SHRT_MAX / 2) + (SHRT_MAX / 4); //  (32,767 / 4) - (32,767 / 2)
    unsigned long long *keysInTree = new unsigned long long[nodeGenCount];
    
    cout << nodeGenCount << " nodes will be generated for red black tree unit tests." << endl;
    cout << "Insertion unit test now being performed." << endl;

    milliseconds start = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

    // Create red black tree nodes & insert into tree
    for (int i = 0; i < nodeGenCount; i++)
    {
        unsigned long long randomKey = rand() % ULLONG_MAX + 1; // 1 - 18,446,744,073,709,551,615
        bool randomData = rand() % 1; // 0 or 1
        tree.insert(randomKey, &randomData);

        end = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        elapsed = end - start;
        double elMilli = duration<double>(elapsed).count();
        int elTime = (int) elMilli; 
        
        if (lastLog != elTime && elTime % 3 == 0)
        {
            unsigned long long tmp = i;
            logStatus(&tmp, &nodeGenCount, "insertion");
            lastLog = elTime;
        }

        // Perform assertion that properties hold after insertion
        performRedBlackPropertyAssertion(&tree);
        keysInTree[i] = randomKey;
    }
    
    lastLog = 0;
    unsigned long long keyIter = 0;

    cout << "Deletion unit test now being performed." << endl;
    
    while (!tree.isEmpty())
    {
        unsigned long long removeKey = keysInTree[keyIter];
        
        tree.del(removeKey);

        // perform assertino that properties hold after deletion
        if (!tree.isEmpty())
            performRedBlackPropertyAssertion(&tree);

        end = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        elapsed = end - start;
        double elMilli = duration<double>(elapsed).count();
        int elTime = (int) elMilli;
        
        if (lastLog != elTime && elTime % 3 == 0)
        {
            logStatus(&keyIter, &nodeGenCount, "deletion");
            lastLog = elTime;
        }
        
        keyIter++;
    }

    logStatus(&keyIter, &nodeGenCount, "deletion");
    
    end = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    elapsed = end - start;
    double elFinal = duration<double>(elapsed).count();
    
    cout << "Total completion time took about " << elFinal << " seconds." << endl;
    
    delete [] keysInTree;
    
    return 0;
}

/**
 * Returns true if node 'n' has the color BLACK, and false if RED.
 * 
 * @param rbNode<T>*
 * @return bool
 */
template <class T>
bool isBlack(rbNode<T>* n)
{
    if (n->rbColor == BLACK)
        return true;
    return false;
}

/**
  * Assertion function which is used to validate the state of the red black tree is proper by 
  * asserting the 5 red black tree properties hold throughout the application.
  * 
  * @param RBT<T>
  * @return void
  */
template <class T>
void performRedBlackPropertyAssertion(RBT<T>* tree)
{
    // Property 1: Every node is either red or black
    assert(true == allRedOrBlack(tree->getRoot(), tree->getSentinel()));

    // Property 2: The root is black
    assert(true == isBlack(tree->getRoot()));
    
    // Property 3: Every leaf (sentinel) is black
    assert(true == isBlack(tree->getSentinel()));

    // Property 4: If a node is red, both its children are black
    assert(true == redHasBlackChildren(tree->getRoot(), tree->getSentinel()));

    // Property 5: For each node, all simple paths from the node
        // to descendant leaves contain the same number of black nodes.
    assert(true == allSimplePathsHaveSameBlackNodeCount(tree->getRoot(), tree->getSentinel()));
}

/**
 * Returns true if every node in the tree is RED or BLACK an false otherwise.
 * 
 * @param rbNode<T>*
 * @param rbNode<T>*
 * @return bool
 */
template <class T>
bool allRedOrBlack(rbNode<T>* root, rbNode<T>* sentinel)
{
    bool allCorrect = true;

    if (root == sentinel && sentinel->rbColor == BLACK)
        return true;
    else if (root == sentinel && sentinel->rbColor != BLACK)
        return false;
    
    allCorrect = allRedOrBlack(root->left, sentinel);
    
    if (!allCorrect)
        return allCorrect;
    
    if (root->rbColor != BLACK && root->rbColor != RED)
        return false;
        
    return allRedOrBlack(root->right, sentinel);
}

/**
 * Returns true if all red children have all children as black as there can not 
 * be any adjacent red nodes in the red black tree in order to keep the same 
 * number of black nodes in all simple paths.
 * 
 * @param rbNode<T>*
 * @param rbNode<T>*
 * @return bool
 */
template <class T>
bool redHasBlackChildren(rbNode<T>* root, rbNode<T>* sentinel)
{
    bool allCorrect = true;

    if (root == sentinel && sentinel->rbColor == BLACK)
        return true;
    
    allCorrect = allRedOrBlack(root->left, sentinel);
    
    if (!allCorrect)
        return allCorrect;
    
    if (root->rbColor == RED && root->left->rbColor != BLACK && root->right->rbColor != BLACK)
        return false;
        
    return allRedOrBlack(root->right, sentinel);
}

/**
 * Logs the current status and the percentage done of the test being worked on at the time of being called.
 * 
 * @param unsigned long long*
 * @param unsigned long long*
 * @param string
 * @return void
 */ 
void logStatus(unsigned long long* currIter, unsigned long long* treeSize, string testType)
{   
    double percentageDone = ((*currIter / (*treeSize * 1.0)) * 100);
    
    cout << fixed << setprecision(2) << percentageDone << "% of " << testType << " unit test has been completed." << endl;
}

/**
 * Validates each count contained in the pathsBlackNodesCount vector are all equal to each other
 * as this confirms every simple path has the same amount of black nodes. This will return true
 * if correct and false if any path has an incorrect black node count that doesn't match the rest.
 * 
 * @param rbNode<T>*
 * @param rbNode<T>*
 * @param int*
 * @return bool 
 */
template <class T>
bool allSimplePathsHaveSameBlackNodeCount(rbNode<T>* root, rbNode<T>* sentinel)
{
    bool sameCount = true;
    vector<int> pathsBlackNodesCounts;
    
    getSimplePathBlackCounts(root, sentinel, pathsBlackNodesCounts);
    
    int prevCount = pathsBlackNodesCounts.back();
    pathsBlackNodesCounts.pop_back();
    
    while (!pathsBlackNodesCounts.empty())
    {
        int count = pathsBlackNodesCounts.back();
        pathsBlackNodesCounts.pop_back();
        if (count != prevCount)
        {
            sameCount = false;
            break;
        }
        else
            prevCount = count;
    }

    return sameCount;
}

/**
 * Naviagates through every simple path until the path can no longer be exhausted and stores the current count of that 
 * recursive instance of black node counts into the blackLens vector reference.
 * 
 * @param rbNode<T>*
 * @param rbNode<T>* 
 * @param vector<int>&
 * @param int[]
 * @param int
 * @param int
 * @return void
 */
template <class T>
void getSimplePathBlackCounts(rbNode<T>* root, rbNode<T>* sentinel, vector<int>& blackLens, int pathLen, int blackLen)
{
    if (root == sentinel)  
        return;

    if (root->rbColor == BLACK)
        blackLen++;

    // It's a leaf, so store current black node count
    if (root->left == sentinel && root->right == sentinel)
    {
        blackLens.push_back(blackLen);
    }  
    else
    {  
        /* otherwise try both subtrees */
        getSimplePathBlackCounts(root->left, sentinel, blackLens, pathLen, blackLen);
        getSimplePathBlackCounts(root->right, sentinel, blackLens, pathLen, blackLen);
    }
}