#include <iostream>

#include "binarytree.h"

int main()
{
    Randomizer rand{};
    Grid grid(4, 4);

    BinaryTree::use(grid, rand);

    std::cout << grid << std::endl;

    return 0;
}