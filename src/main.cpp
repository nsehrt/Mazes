#include <iostream>

#include "binarytree.h"

int main()
{
    Randomizer rand{};
    Grid grid(32, 32, rand);

    BinaryTree::use(grid, rand);

    std::cout << grid << std::endl;

    return 0;
}