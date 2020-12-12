# Red-Black-Tree

C++ Red Black Tree implementation with demo &amp; unit test binaries.

This Red Black Tree implementation was built using the psuedocode description of the
algorithm described by 'Introduction to Algorithms' the Third Edition. This implementation
also utilizes the sentinel construct rather than many references to NULL. By using the sentinel
construct, this tree is also built to be bi-directional as the root can be accessed by the sentinels'
left, right, and parent pointers.

## Build Instructions

Can use the makefile to build the 2 binaries; the -O3 flag is used in order
to help optimize the unit test executable.

While in the main directory simply perform the following command to build:
    make

And the following to clean:
    make clean