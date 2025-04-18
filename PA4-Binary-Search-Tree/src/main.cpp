#include <iostream>
#include <string>

#include "BinarySearchTree.h"

int main() {
    // Give us a random tree each time
    srand(time(NULL));
    // 15 elements in the tree
    constexpr int TREE_SIZE = 15;

    BinarySearchTree<std::string, int> simpleTree;
    std::string names[TREE_SIZE] {"Teresa", "Carlos", "Nkemdi", "Dante", "Alexander", "Evelyn", "Dillon", "Thomas", "Armando", "Mariel", "Furkan", "Anjali", "Jeremy", "Clayton", "Jessica"};
    for (int i = 0; i < TREE_SIZE; ++i) {
        simpleTree.insert( { names[i], rand() % TREE_SIZE } );
    }

    printTree(simpleTree);

    // Test, delete later
    printLevelByLevel(simpleTree);
}