# Red-Black-Tree

C++ Red Black Tree implementation with demo &amp; unit test binaries.

This Red Black Tree implementation was built using the psuedocode description of the
algorithm described by 'Introduction to Algorithms' the Third Edition. This implementation
also utilizes the sentinel construct rather than many references to NULL. By using the sentinel
construct, this tree is also built to be bi-directional as the root can be accessed by the sentinels'
left, right, and parent pointers.

The RBT class is a generic, templated C++ implementation which uses the sentinel construct for leaf
references. The rbNode structure which the class is built on contains rbNode pointers to a left, right
and parent reference, a char for storing the color type, an unsigned long long key variable (which is
used for storing into tree) and a templated data field pointer. This is to utilized by other appliations
such that the tree can be reused for multiple different use cases.

The RBT_DEMO binary is a simple "game" loop which presents the user the ability to delete or insert nodes
into an empty tree as well as printing out the inorder, preorder, and postorder ops of the tree. After
each deletion and insertion the tree is "pretty printed" which is a utility of the RBT class to assist
debugging scenarios that also include the node colors. The inorder, preorder, and postorder ops are also
utilities of the tree which also present the node color.

The RBT_TEST binary is a unit test class which asserts the 5 red black properties are maintained though a
randomized sequence of insertion of values followed by a randomized sequence of deletion of values.

## Build Instructions

Can use the makefile to build the 2 binaries; the -O3 flag is used in order
to help optimize the unit test executable.

While in the main directory simply perform the following command to build:
    make

And the following to clean:
    make clean